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

const llvmUVEPath = process.env.LLVM_PATH;

if (!llvmUVEPath) {	
	exitWithError("LLVM_UVE_PATH environment variable is not set");
}

const clangUVEPath = `${llvmUVEPath}/llvm-project/build/bin/clang`;

const objDumpPathUVECLANG = `${llvmUVEPath}/llvm-project/build/bin/llvm-objdump`;

const optPath = `${llvmUVEPath}/llvm-project/build/bin/opt`;

const llcPath = `${llvmUVEPath}/llvm-project/build/bin/llc`;

const passPath = `${llvmUVEPath}/StreamPass/build/libStreamAnalysis.so`;

const llvmPath = process.env.LLVM_PATH;

if (!llvmPath) {	
	exitWithError("LLVM_PATH environment variable is not set");
}

const clangPath = `${llvmPath}/llvm-project/build/bin/clang`;

const objDumpPathCLANG = `${llvmPath}/llvm-project/build/bin/llvm-objdump`;

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

const compileFlags = ["-O2", "--target=riscv64", "-march=rv64imafdc", "-mabi=lp64d", `--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`, "-fno-tree-vectorize", "-fno-unroll-loops"];
const linkFlags = [ `--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`, "--target=riscv64", "-march=rv64imafdc", "-mabi=lp64d"];
const clangIRFlags = ["-O2", "--target=riscv64", "-march=rv64imafdc", "-mabi=lp64d", `--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`,  "-fno-vectorize", "-fno-slp-vectorize", "-fdiscard-value-names", "-ffp-contract=off", "-emit-llvm", "-fno-unroll-loops"];
const optFlags = ["-enable-new-pm=0", "-load", passPath, "-loop-simplify", "-legacy-stream-analysis"];
const llcFlags = ["-march=riscv64", "--mcpu=generic-rv64", "-mattr=+d,+m,+experimental-xuve"];

//const compileFlagsRVV = ["-O2", `--sysroot=${riscvPath}/riscv64-unknown-elf`, `--gcc-toolchain=${riscvPath}`, `-I${riscvPath}/include`, "-ffast-math", "-fno-unroll-loops", "--target=riscv64", "-march=rv64imafdcv", "-Rpass=loop-vectorize", "-Rpass-missed=loop-vectorize", "-Rpass-analysis=loop-vectorize"]; // "-fno-unroll-loops",

const bin_simple = `.run_simple`;
const bin_simple_rvv = `.run_simple_rvv`;
const bin_uve = `.run_uve`;
const bin_rvv = `.run_rvv`;

// read size from command line
const size = process.argv[2] || 64;

// read csv filename from command line
const csvFilename = process.argv[3] || "results_uve_rvv.csv";

// write csv header
fs.writeFile(csvFilename, "kernel,size,datatype,original_llvm19,rvv,original_llvm16,uve\n", (err) => {
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
	//"sgd": 0,
	"spmv_ellpack": 0,
	"spmv_ellpack_delimiters": 0,
	"stream": size*size,
	"trisolv": size,

	"2mm": size,
	"atax": size,
	"bicg": size,
	"cholesky": size,
	"doitgen": 0,
	"fdtd-2d": size,
	"gemm_blocked": size,
	"gemm_ncubed": size,
	"gesummv": size,
	"gramschmidt": size,
	"knn": 0,
	"lu": size,
	"stencil2d": size,
	"symm": size,
	"syrk": size,
	"syr2k": size,
	"trmm": size,*/
	
	//"sgd": 0,
	//"durbin": size*size,
	//"seidel-2d": size,
	//"adi": size
	//"heat-3d": size
	//"sort": size
	//"fft": size
	//"kmp": 0
	//"stencil3d": size
	//"correlation": size
	//"ludcmp": size
	//"vector_size_scaling": size
	"vector_size_scaling_1": size,
	"vector_size_scaling_2": size,
	"vector_size_scaling_3": size,
	"vector_size_scaling_4": size,
	"vector_size_scaling_5": size,
	"vector_size_scaling_6": size,
	//"vector_size_scaling_7": size,
	//"vector_size_scaling_8": size,
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

function aproximateEqual(stdout1, stdout2, stdout3, stdout4, kernel, t, s, dir) {
	let flag = true;
	/* Write log files */
	fs.writeFile(`${dir}/simple_uve.txt`, stdout1, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/simple_rvv.txt`, stdout2, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/uve.txt`, stdout3, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`${dir}/rvv.txt`, stdout4, (err) => {
		if (err) throw err;
	});

	/* Compare values */
	const original_uve = stdout1.split("\n");
	const original_rvv = stdout2.split("\n");
	const uve = stdout3.split("\n");
	const rvv = stdout4.split("\n");

	if (original_uve.length !== uve.length  || original_rvv.length !== rvv.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	const start1 = parseInt(original_uve[0]);
	const start2 = parseInt(uve[0]);
	const start3 = parseInt(original_rvv[0]);
	const start4 = parseInt(rvv[0]);
	const end1 = parseInt(original_uve[1]);
	const end2 = parseInt(uve[1]);
	const end3 = parseInt(original_rvv[1]);
	const end4 = parseInt(rvv[1]);

	for (i = 2; i < original_uve.length; i++) {
		const value1 = parseFloat(original_uve[i]);
		const value2 = parseFloat(uve[i]);
		const value3 = parseFloat(original_rvv[i]);
		const value4 = parseFloat(rvv[i]);

		const diff = Math.abs(value1 - value2);
		const diff2 = Math.abs(value3 - value4);
		if (diff > 0.1) {
			console.error(`UVE: Values were ${original_uve[i]} and ${uve[i]} with difference of ${diff} at index ${i-2}`);
			flag = false;
		}
		if (diff2 > 0.1) {
			console.error(`RVV: Values were ${original_rvv[i]} and ${rvv[i]} with difference of ${diff2} at index ${i-2}`);
			flag = false;
		}
	}

	const insnsO_UVE = end1-start1;
	const insnsUVE = end2-start2;
	const insnsO_RVV = end3-start3;
	const insnsRVV = end4-start4;
	const diffInsnsUVE = insnsUVE-insnsO_UVE;
	const diffInsnsRVV = insnsRVV-insnsO_RVV;
	const diffInsnsUVE_RVV = insnsUVE - insnsRVV;

	// difference in percentage
	const diffInsnsPuve = (diffInsnsUVE/insnsO_UVE)*100;
	const diffInsnsPrvv = (diffInsnsRVV/insnsO_RVV)*100;
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
		[insnsO_RVV, insnsRVV, diffInsnsRVV, diffInsnsPrvv.toFixed(2)],
		['Original', 'UVE', 'Difference', 'Difference (%)'],
		[insnsO_UVE, insnsUVE, diffInsnsUVE, diffInsnsPuve.toFixed(2)],
		['RVV', 'UVE', 'Difference', 'Difference (%)'],
		[insnsRVV, insnsUVE, diffInsnsUVE_RVV, diffInsnsPuve_rvv.toFixed(2)]
	];

	// Call the function to adjust table width and print the table
	adjustTableWidth(data);

	console.log(`\n`);

	// export to csv
	// kernel, size, datatype, original, rvv, uve
	const csv = `${kernel},${s},${typeMap[t]},${insnsO_RVV},${insnsRVV},${insnsO_UVE},${insnsUVE}\n`;
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
		compileKernel(clangUVEPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
		compileKernel(clangUVEPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
		compileKernel(clangUVEPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);

		/* Link and create no UVE executable */
		compileKernel(clangUVEPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_simple}`, "-lm"]);

		/* Create objdump file */

		objDump = spawnSync(objDumpPathUVECLANG, ["-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/simple.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Generate kernel IR with LLVM 16 */
		compileKernel(clangUVEPath, [...clangIRFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-S", "-o", `kernel.ll`]);

		/* Apply UVE pass */
		compileKernel(optPath, [...optFlags, `kernel.ll`, "-S", "-o", `UVEkernel.ll`]);

		/* LL to Assembly */
		compileKernel(llcPath, [...llcFlags, `UVEkernel.ll`]);	

		/* Assembly to object */
		compileKernel(clangUVEPath, [...compileFlags, "-c", "-o", "UVEkernel.o", `UVEkernel.s`]);

		/* Create objdump file */

		objDump = spawnSync(objDumpPathCLANG, ["-d", "UVEkernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/uve.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		/* Link everyting */
		compileKernel(clangUVEPath, [...linkFlags, "Functions.o", `UVEkernel.o`, `main.o`, "-o", `${dir}/${bin_uve}`, "-lm"]);

		/* Compile for RVV with LLVM 19
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-c"], true);
		compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_simple_rvv}`]);

		objDump = spawnSync(`${objDumpPathCLANG}`, ["--mattr=rv64gc",  "-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();ß
		fs.writeFile(`${dir}/simple.dump`, stdoutO, (err) => {
			if (err) throw err;
		});

		compileKernel(clangPath, [...compileFlagsRVV, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-c"], true);
		compileKernel(clangPath, [...compileFlagsRVV, `-D${type}_TYPE`, `-DSIZE=${s}`, "Functions.o", `kernel.o`, `main.o`, "-o", `${dir}/${bin_rvv}`]);

		objDump = spawnSync(`${objDumpPathCLANG}`, ["--mattr=rv64gcv",  "-d", "kernel.o"]);
		stdoutO = objDump.stdout.toString();
		fs.writeFile(`${dir}/rvv.dump`, stdoutO, (err) => {
			if (err) throw err;
		});*/

		/* Run each kernel file */
		const execSimpleUVE = executableRun(spikePath, [pkPath, `${dir}/${bin_simple}`, kernel]);
		const execSimpleRVV = executableRun(spikePath, [pkPath, `${dir}/${bin_simple_rvv}`, kernel]);
		const execUVE = executableRun(spikePath, [pkPath, `${dir}/${bin_uve}`, kernel]);
		const execRVV = executableRun(spikePath, ["--isa=rv64gcv_Zicntr_zvl512b_zve64d", pkPath, `${dir}/${bin_rvv}`, kernel]);

		/* Test if generated values are similar */

		if (aproximateEqual(execSimpleUVE.stdout.toString(), execSimpleRVV.stdout.toString(), execUVE.stdout.toString(), execRVV.stdout.toString(), kernel, type, s, dir)) {
			console.log(`Kernel ${kernel} is similar enough`);
		} else {
			console.error(`Kernel ${kernel}: Did not generate result similar enough`);
			//break;
		}

		// Delete object and assembly files for next execution
		const del = spawnSync("rm", ['-f', 'main.o', 'kernel.o' , 'UVEkernel.o', 'kernel.ll', 'UVEkernel.ll', /*'UVEkernel.s',*/ 'Functions.o']);
		if (del.error) {
			console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
			break;
		}
	}
}