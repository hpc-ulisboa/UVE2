#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

// Function to exit the script with an error message
function exitWithError(message) {
	console.error(message);
	process.exit(1);
}

const riscvPath = process.env.RISCV; // /

if (!riscvPath) {
	exitWithError("RISCV environment variable is not set");
}

const llvmPath = process.env.LLVM_PATH;

if (!llvmPath) {
	exitWithError("LLVM_PATH environment variable is not set");
}

const clangPath = `${llvmPath}/llvm-project/build/bin/clang`;

const objDumpPathCLANG = `${llvmPath}/llvm-project/build/bin/llvm-objdump`;

const optPath = `${llvmPath}/llvm-project/build/bin/opt`;

const llcPath = `${llvmPath}/llvm-project/build/bin/llc`;

const passPath = `${llvmPath}/StreamPass/build/libStreamAnalysis.so`;

const pkPath = process.env.PK_PATH;

if (!pkPath) {
	exitWithError("PK_PATH environment variable is not set");
}

const spikePath = process.env.SPIKE_PATH;

if (!spikePath) {
	exitWithError("SPIKE_PATH environment variable is not set");
}

/*const gccPath = process.env.GCC_PATH; 

if (!gccPath) {
	exitWithError("GCC_PATH environment variable is not set");
}*/

const compileFlags = ["-O2", "--target=riscv64", "-march=rv64imafdc", "-march=rv64gcxuve0p1", "-menable-experimental-extensions", "-mabi=lp64d", `--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`, "-fno-tree-vectorize", "-fno-unroll-loops"];
const linkFlags = [`--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`, "--target=riscv64", "-march=rv64imafdc", "-march=rv64gcxuve0p1", "-menable-experimental-extensions", "-mabi=lp64d"];

const bin_simple = `.run_simple`;
const bin_uve = `.run_uve`;

// read size from command line
const size = process.argv[2] || 64;

// read csv filename from command line
const csvFilename = process.argv[3] || "results.csv";

// write csv header
fs.writeFile(csvFilename, "kernel,size,datatype,original,uve\n", (err) => {
	if (err) throw err;
});

// kernel size map
const kernelSizeMap = {
	"3mm": size,
	"convolution": size,
	"covariance": size,
	"gemm": size,
	"gemver": size,
	"jacobi-1d": size*size,
	"jacobi-2d": size,
	"memcpy": size*size,
	"mvt": size,
	"saxpy": size*size,
	//"sgd": 0,
	"spmv_ellpack": 0,
	"spmv_ellpack_delimiters": 0,
	"stream": size*size,
	"trisolv": size
};

// read type and size from command line
const typeMap = {
	//'B': 'byte',
	//'H': 'half-word',
	//'I': 'integer',
	//'F': 'float',
	'D': 'double'
};

function adjustTableWidth(data) {
	// Function to calculate the maximum width of each column
	function getMaxWidth(column, index) {
		return Math.max(...data.map(row => String(row[index]).length, 0));
	}

	// Calculate the maximum width for each column
	const maxWidths = data[0].map((_, index) => getMaxWidth(data, index));

	// Function to print a row with hyphens filling each column
	function printHyphenRow() {
		const hyphenRow = maxWidths.map(width => '-'.repeat(width + 2)); // +2 for padding on each side
		console.log(`|${hyphenRow.join('|')}|`);
	}

	// Function to format a cell with padding based on the calculated width
	function formatCell(value, maxWidth) {
		const padding = ' '.repeat(maxWidth - String(value).length);
		return ` ${value}${padding} `;
	}

	// Function to print a row with adjusted width
	function printRow(row) {
		const formattedRow = row.map((value, index) => formatCell(value, maxWidths[index]));
		console.log(`|${formattedRow.join('|')}|`);
	}

	// Print the table header
	printHyphenRow();
	printRow(data[0]);

	// Print each data row with adjusted width
	data.slice(1).forEach((row, index) => {
		printHyphenRow();
		printRow(row);
	});
	printHyphenRow();

}

function executableRun(command, args) {
	const executable = spawnSync(command, args);
	if (executable.error) {
		exitWithError(`An error occured while trying to run ${command}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		exitWithError(`Execution failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
	return executable;
}

function compileKernel(command, args, flag = false) {
	//console.log(`${command} ${args.join(" ")}`);
	const executable = spawnSync(command, args);
	if (flag)
		console.log(executable.stderr.toString());

	if (executable.error) {
		exitWithError(`An error occured while trying to compile ${command} ${args.join(" ")}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		exitWithError(`Compilation failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
}

function aproximateEqual(stdout1, stdout2, kernel, t, s, dir) {
	let flag = true;
	/* Write log files */
	fs.writeFile(`${dir}/simple_uve.txt`, stdout1, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/uve.txt`, stdout2, (err) => {
		if (err) throw err;
	});

	/* Compare values */
	const original = stdout1.split("\n");
	const uve = stdout2.split("\n");

	if (original.length !== uve.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	const start1 = parseInt(original[0]);
	const start2 = parseInt(uve[0]);
	const end1 = parseInt(original[1]);
	const end2 = parseInt(uve[1]);

	for (i = 2; i < original.length; i++) {
		const value1 = parseFloat(original[i]);
		const value2 = parseFloat(uve[i]);

		const diff = Math.abs(value1 - value2);
		if (diff > 0.1) {
			console.error(`UVE: Values were ${original[i]} and ${uve[i]} with difference of ${diff} at index ${i - 2}`);
			flag = false;
		}
	}

	const insnsO = end1 - start1;
	const insnsUVE = end2 - start2;
	const diffInsnsUVE = insnsUVE - insnsO;

	// difference in percentage
	const diffInsnsPuve = (diffInsnsUVE / insnsO) * 100;

	if (diffInsnsUVE < 0)
		console.log(`UVE executed ${-diffInsnsUVE} less instructions`);
	else if (diffInsnsUVE > 0)
		console.log(`UVE executed ${diffInsnsUVE} more instructions`);

	const data = [
		['Original', 'UVE', 'Difference', 'Difference (%)'],
		[insnsO, insnsUVE, diffInsnsUVE, diffInsnsPuve.toFixed(2)],
	];

	// Call the function to adjust table width and print the table
	adjustTableWidth(data);

	console.log(`\n`);

	// export to csv
	// kernel, size, datatype, original, uve
	const csv = `${kernel},${s},${typeMap[t]},${insnsO},${insnsUVE}\n`;
	fs.appendFile(csvFilename, csv, (err) => {
		if (err) throw err;
	});

	return flag;
}

// loop over kernelSizeMap keys
for (let kernel in kernelSizeMap) {
	// loop over typeMap keys
	for (let type in typeMap) {
		let dir; let s = kernelSizeMap[kernel];
		// check if sgd, spmv_ellpack or spmv_ellpack_delimiters are wanted
		if (s === 0) {
			dir = `benchmarks/${kernel}`;
		} else {
			dir = `benchmarks/${kernel}/runs_${type.toLowerCase()}_${size}`;
			// if the directory does not exist, create it
			if (!fs.existsSync(dir)) {
				fs.mkdirSync(dir);
			}
		}

		console.log(`\n### Attempting to compile and run kernel ${kernel} (size: ${s}, type: ${typeMap[type]}) ...\n`);

		/* Compile source files */
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);

		/* Link and create no UVE executable */
		compileKernel(clangPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_simple}_uve`, "-lm"]);

		/* Create objdump file */

		objDump = spawnSync(objDumpPathCLANG, ["-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/simple.dump`, stdoutO, (err) => {
			if (err) throw err;
		});
		/* Compile source files */
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_UVE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);

		/* Link and create UVE executable */
		compileKernel(clangPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_uve}`, "-lm"]);
		/* Create objdump file */

		objDump = spawnSync(objDumpPathCLANG, ["-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/uve.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Link everyting */
		compileKernel(clangPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_uve}`, "-lm"]);

		/* Run each kernel file */
		const execSimpleUVE = executableRun(spikePath, [pkPath, `${dir}/${bin_simple}_uve`, kernel]);
		const execUVE = executableRun(spikePath, [pkPath, `${dir}/${bin_uve}`, kernel]);

		/* Test if generated values are similar */

		if (aproximateEqual(execSimpleUVE.stdout.toString(), execUVE.stdout.toString(), kernel, type, s, dir)) {
			console.log(`Kernel ${kernel} is similar enough`);
		} else {
			console.error(`Kernel ${kernel}: Did not generate result similar enough`);
			//break;
		}

		// Delete executables for next kernel
		const del = spawnSync("rm", ['-f', 'main.o', 'kernel.o', 'Functions.o']);
		if (del.error) {
			console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
			break;
		}
	}
}