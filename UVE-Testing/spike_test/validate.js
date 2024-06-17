#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

// read size from command line
const size = process.argv[2] || 50;

// read csv filename from command line
const csvFilename = process.argv[3] || "results.csv";

// write csv header
fs.writeFile(csvFilename, "kernel,size,datatype,original_clang,rvv,original_gcc,uve\n", (err) => {
	if (err) throw err;
});

// kernel size map
const kernelSizeMap = {
	/*"3mm": size,
	"convolution": size,
	"covariance": size,
	"gemm": size,
	"gemver": size,
	"jacobi-1d": size*size,
	"jacobi-2d": size,
	"memcpy": size*size,
	"mvt": size,
	"saxpy": size*size,
	"sgd": 0,
	"spmv_ellpack": 0,
	"spmv_ellpack_delimiters": 0,
	"stream": size*size,*/
	"trisolv": size//,
	//"triangular_acc": size
};

// read type and size from command line
const typeMap = {
    //'B': 'byte',
	//'H': 'half-word',
    //'I': 'integer',
    'F': 'float'//,
    //'D': 'double'
};

const compileFlags = ["-O3", "-fno-tree-vectorize", "-fno-unroll-loops", "-Wall", "-pedantic"];
const linkFlags = ["-O3", "-Wall", "-pedantic", "-static"];
const clangFlagsV = ["-O3", "--sysroot=/home/afernandes/install/uve_tc/riscv64-unknown-elf", "--gcc-toolchain=/home/afernandes/install/uve_tc", "-I/home/afernandes/install/uve_tc/include", "-ffast-math", "-fno-unroll-loops", "--target=riscv64", "-march=rv64gcv", "-Rpass=loop-vectorize", "-Rpass-missed=loop-vectorize", "-Rpass-analysis=loop-vectorize"]; // "-fno-unroll-loops",
const clangFlags = ["-O3", "--sysroot=/home/afernandes/install/uve_tc/riscv64-unknown-elf", "--gcc-toolchain=/home/afernandes/install/uve_tc", "-I/home/afernandes/install/uve_tc/include", "-ffast-math",  "-fno-vectorize", "-fno-unroll-loops", "--target=riscv64", "-march=rv64gc"];
const gccPath = "/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-gcc";
const clangPath = "/home/afernandes/LLVM-Compiler/llvm-project/build/bin/clang";
const pkPath = "/home/afernandes/uve-dev/UVE-Testing/pk";
const spikePath = "/home/afernandes/uve-dev/UVE-Testing/spike";
const bin_simple_gcc = `.run_simple_gcc`;
const bin_uve = `.run_uve`;
const bin_rvv = `.run_rvv`;
const bin_simple_clang = `.run_simple_clang`;

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
		throw new Error(`An error occured while trying to run ${command}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		throw new Error(`Execution failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
	return executable;
}

function compileKernel(command, args, flag = false) {
	//console.log(`${command} ${args.join(" ")}`);
	const executable = spawnSync(command, args);
	if (flag)
		console.log(executable.stderr.toString());

	if (executable.error) {
		throw new Error(`An error occured while trying to compile ${command} ${args.join(" ")}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		throw new Error(`Compilation failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
}

function aproximateEqual(stdout1, stdout2, stdout3, stdout4, kernel, t, s, dir) {
	let flag = true;
	/* Write log files */
	fs.writeFile(`${dir}/simple_gcc.txt`, stdout1, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/uve.txt`, stdout2, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/simple_clang.txt`, stdout3, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/rvv.txt`, stdout4, (err) => {
		if (err) throw err;
	});

	/* Compare values */
	const original_gcc = stdout1.split("\n");
	const uve = stdout2.split("\n");
	const original_clang = stdout3.split("\n");
	const rvv = stdout4.split("\n");

	if (original_gcc.length !== uve.length || original_gcc.length !== original_clang.length || original_gcc.length !== rvv.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	const start1 = parseInt(original_gcc[0]);
	const start2 = parseInt(uve[0]);
	const start3 = parseInt(original_clang[0]);
	const start4 = parseInt(rvv[0]);
	const end1 = parseInt(original_gcc[1]);
	const end2 = parseInt(uve[1]);
	const end3 = parseInt(original_clang[1]);
	const end4 = parseInt(rvv[1]);

	for (i = 2; i < original_gcc.length; i++) {
		const value1 = parseFloat(original_gcc[i]);
		const value2 = parseFloat(uve[i]);

		const value3 = parseFloat(original_clang[i]);
		const value4 = parseFloat(rvv[i]);

		const diff = Math.abs(value1 - value2);
		const diff2 = Math.abs(value3 - value4);
		if (diff > 0.1) {
			console.error(`UVE: Values were ${original_gcc[i]} and ${uve[i]} with difference of ${diff} at index ${i-2}`);
			flag = false;
		}
		if (diff2 > 0.1) {
			console.error(`RVV: Values were ${original_clang[i]} and ${rvv[i]} with difference of ${diff2} at index ${i-2}`);
			flag = false;
		}
	}

	const insnsOGCC = end1-start1;
	const insnsUVE = end2-start2;
	const insnsOCLANG = end3-start3;
	const insnsRVV = end4-start4;
	const diffInsnsUVE = insnsUVE-insnsOGCC;
	const diffInsnsRVV = insnsRVV-insnsOCLANG;
	const diffInsnsUVE_RVV = insnsUVE - insnsRVV;

	// difference in percentage
	const diffInsnsPuve = (diffInsnsUVE/insnsOGCC)*100;
	const diffInsnsPrvv = (diffInsnsRVV/insnsOCLANG)*100;
	const diffInsnsPuve_rvv = (diffInsnsUVE_RVV/insnsRVV)*100;

	if (diffInsnsUVE < 0)
		console.log(`UVE executed ${-diffInsnsUVE} less instructions`);
	else if (diffInsnsUVE > 0)
		console.log(`UVE executed ${diffInsnsUVE} more instructions`);

	if (diffInsnsRVV < 0)
		console.log(`RVV executed ${-diffInsnsRVV} less instructions`);
	else if (diffInsnsRVV > 0)
		console.log(`RVV executed ${diffInsnsRVV} more instructions`);

	if (diffInsnsUVE_RVV < 0)
		console.log(`UVE executed ${-diffInsnsUVE_RVV} less instructions than RVV`);
	else if (diffInsnsUVE_RVV > 0)
		console.log(`UVE executed ${diffInsnsUVE_RVV} more instructions than RVV`);

	const data = [
		['Original', 'RVV', 'Difference', 'Difference (%)'],
		[insnsOCLANG, insnsRVV, diffInsnsRVV, diffInsnsPrvv.toFixed(2)],
		['Original', 'UVE', 'Difference', 'Difference (%)'],
		[insnsOGCC, insnsUVE, diffInsnsUVE, diffInsnsPuve.toFixed(2)],
		['RVV', 'UVE', 'Difference', 'Difference (%)'],
		[insnsRVV, insnsUVE, diffInsnsUVE_RVV, diffInsnsPuve_rvv.toFixed(2)]
	];

	// Call the function to adjust table width and print the table
	adjustTableWidth(data);

	console.log(`\n`);

	// export to csv
	// kernel, size, datatype, original_clang, rvv, original_gcc, uve
	const csv = `${kernel},${s},${typeMap[t]},${insnsOCLANG},${insnsRVV},${insnsOGCC},${insnsUVE}\n`;
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
		if (kernel === "sgd" || kernel === "spmv_ellpack" || kernel === "spmv_ellpack_delimiters") {
			if (type === "D") {
				dir = `benchmarks/${kernel}`;
			} else {
				continue;
			}
		} else {
			if ((kernel === "jacobi-1d" || kernel === "jacobi-2d") && type !== "F" && type !== "D") {
				continue;
			}
			dir = `benchmarks/${kernel}/runs_${type.toLowerCase()}_${size*size}`;
			// if the directory does not exist, create it
			if (!fs.existsSync(dir)) {
				fs.mkdirSync(dir);
			}
		}

		const sizeCSV = s === size ? s*s : s;
		console.log(`\n### Attempting to compile and run kernel ${kernel} (size: ${sizeCSV}, type: ${typeMap[type]}) ...\n`);

		/* Compile Functions source files */
		compileKernel(gccPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
		compileKernel(gccPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);

		
		compileKernel(gccPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_UVE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
		compileKernel(gccPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_uve}`]);

		let objDump = spawnSync("/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-objdump", ["-d", "kernel.o"]);
		let stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/uve.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Compile and link each kernel file */
		compileKernel(gccPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
		compileKernel(gccPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_simple_gcc}`]);

		objDump = spawnSync("/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-objdump", ["-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/simple_gcc.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Compile for RVV with clang */
		compileKernel(clangPath, [...clangFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
		compileKernel(clangPath, [...clangFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
		compileKernel(clangPath, [...clangFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"], true);
		compileKernel(clangPath, [...clangFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_simple_clang}`]);

		objDump = spawnSync("/home/afernandes/LLVM-Compiler/llvm-project/build/bin/llvm-objdump", ["--mattr=rv64gc",  "-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/simple_clang.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		compileKernel(clangPath, [...clangFlagsV, `-D${type}_TYPE`, `-DSIZE=${s}`, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"], true);
		compileKernel(clangPath, [...clangFlagsV, `-D${type}_TYPE`, `-DSIZE=${s}`, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_rvv}`]);

		objDump = spawnSync("/home/afernandes/LLVM-Compiler/llvm-project/build/bin/llvm-objdump", ["--mattr=rv64gcv",  "-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/rvv.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Run each kernel file */
		const execSimpleGCC = executableRun(spikePath, [pkPath, `${dir}/${bin_simple_gcc}`, kernel]);
		const execUVE = executableRun(spikePath, [pkPath, `${dir}/${bin_uve}`, kernel]);
		const execSimpleClang = executableRun(spikePath, [pkPath, `${dir}/${bin_simple_clang}`, kernel]);
		const execRVV = executableRun(spikePath, ["--isa=rv64gcv",  "--varch=vlen:512,elen:64", pkPath, `${dir}/${bin_rvv}`, kernel]);

		/* Test if generated values are similar */

		if (aproximateEqual(execSimpleGCC.stdout.toString(),  execUVE.stdout.toString(), execSimpleClang.stdout.toString(), execRVV.stdout.toString(), kernel, type, sizeCSV, dir)) {
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