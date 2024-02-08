#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

//const kernels = ["saxpy", "memcpy", "jacobi-1d", "jacobi-2d", "3mm", "trisolv", "stream", "mvt", "gemver", "gemm", "convolution", "sgd", "covariance", "spmv_ellpack", "spmv_ellpack_delimiters"];

//const kernels = [ "floyd-warshall" ];
//const kernels = [ "knn" ];
//const kernels = [ "syrk" ];

const kernels = ["jacobi-1d"];

/* DTYPE: dataset datatype
 * DTYPE 1: byte (hexadecimal int)
 * DTYPE 2: half-word (short int)
 * DTYPE 3: word (int)
 * DTYPE 4: word (float)
 * DTYPE 5: double (DEFAULT)
 * 
 * DSIZE: size of the dataset (usually a matrix SIZE*SIZE)
 * DSIZE 64: 64x64 matrix (DEFAULT)
*/

// read type and size from command line
const typeMap = {
    'b': 'byte',
    'h': 'half-word',
    'i': 'integer',
    'f': 'float',
    'd': 'double'
};
// read character from command line
const type = process.argv[2] || 'd';
console.log(`Type: ${type}`);
// check if typeN is "b" or "h" or "i" or "f" or "d"
if (!(type == 'b' || type == 'h' || type == 'i' || type == 'f' || type == 'd')) {
	console.error("Invalid type. Please use b, h, i, f or d");
	process.exit(1);
}
const size = process.argv[3] || 50;

console.log(`Running with type \x1b[36m${typeMap[type]}\x1b[0m and size \x1b[36m${size}\x1b[0m\n`);

const compileFlags = ["-O2", "-Wall", "-pedantic", `-D${type.toUpperCase()}_TYPE`, `-DSIZE=${size}`];
const linkFlags = ["-O2", "-Wall", "-pedantic", "-static"];
const clangFlags = ["-O2", "--sysroot=/home/afernandes/install/uve_tc/riscv64-unknown-elf", "--gcc-toolchain=/home/afernandes/install/uve_tc", "-I/home/afernandes/install/uve_tc/include", "--target=riscv64", "-march=rv64gcv", "-Rpass=loop-vectorize", "-Rpass-missed=loop-vectorize", "-Rpass-analysis=loop-vectorize", `-D${type.toUpperCase()}_TYPE`, `-DSIZE=${size}`];
const gccPath = "/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-gcc";
const clangPath = "/home/afernandes/LLVM-Compiler/llvm-project/build/bin/clang";
const pkPath = "/home/afernandes/uve-dev/UVE-Testing/pk";
const spikePath = "/home/afernandes/uve-dev/UVE-Testing/spike";
const bin_simple = `runs_${type}_${size}/run_simple`;
const bin_uve = `runs_${type}_${size}/run_uve`;
const bin_rvv = `runs_${type}_${size}/run_rvv`;

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

function aproximateEqual(stdout1, stdout2, stdout3, kernel) {
	let flag = true;
	/* Write log files */
	fs.writeFile(`benchmarks/${kernel}/runs_${type}_${size}/simple.log`, stdout1, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`benchmarks/${kernel}/runs_${type}_${size}/uve.log`, stdout2, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`benchmarks/${kernel}/runs_${type}_${size}/rvv.log`, stdout3, (err) => {
		if (err) throw err;
	});

	/* Compare values */
	const str1 = stdout1.split("\n");
	const str2 = stdout2.split("\n");
	const str3 = stdout3.split("\n");

	if (str1.length !== str2.length || str1.length !== str3.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	const start1 = parseInt(str1[0]);
	const start2 = parseInt(str2[0]);
	const start3 = parseInt(str3[0]);
	const end1 = parseInt(str1[1]);
	const end2 = parseInt(str2[1]);
	const end3 = parseInt(str3[1]);

	for (i = 2; i < str1.length; i++) {
		const value1 = parseFloat(str1[i]);
		const value2 = parseFloat(str2[i]);
		const value3 = parseFloat(stdout3.split("\n")[i]);

		const diff = Math.abs(value1 - value2);
		const diff2 = Math.abs(value1 - value3);
		if (diff > 0.1) {
			console.error(`UVE: Values were ${str1[i]} and ${str2[i]} with difference of ${diff} at index ${i-2}`);
			flag = false;
		}
		if (diff2 > 0.1) {
			console.error(`RVV: Values were ${str1[i]} and ${str3[i]} with difference of ${diff2} at index ${i-2}`);
			flag = false;
		}
	}

	const insns1 = end1-start1;
	const insns2 = end2-start2;
	const insns3 = end3-start3;
	const diffInsnsUVE = insns2-insns1;
	const diffInsnsRVV = insns3-insns1;
	const diffInsnsUVE_RVV = insns2 - insns3;

	// difference in percentage
	const diffInsnsPuve = (diffInsnsUVE/insns1)*100;
	const diffInsnsPrvv = (diffInsnsRVV/insns1)*100;
	const diffInsnsPuve_rvv = (diffInsnsUVE_RVV/insns3)*100;

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
		[insns1, insns3, diffInsnsRVV, diffInsnsPrvv.toFixed(2)],
		['Original', 'UVE', 'Difference', 'Difference (%)'],
		[insns1, insns2, diffInsnsUVE, diffInsnsPuve.toFixed(2)],
		['RVV', 'UVE', 'Difference', 'Difference (%)'],
		[insns3, insns2, diffInsnsUVE_RVV, diffInsnsPuve_rvv.toFixed(2)]
	];

	// Call the function to adjust table width and print the table
	adjustTableWidth(data);

	console.log(`\n`);

	return flag;
}

for (let kernel of kernels) {
	console.log(`\n### Attempting to compile and run kernel ${kernel}...\n`);

	// if the directory does not exist, create it
	if (!fs.existsSync(`benchmarks/${kernel}/runs_${type}_${size}`)) {
		fs.mkdirSync(`benchmarks/${kernel}/runs_${type}_${size}`);
	}

	/* Compile Functions source files */
	compileKernel(gccPath, [...compileFlags, "-I..", "../Functions.c", "-c"]);
	compileKernel(gccPath, [...compileFlags, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);

	
	compileKernel(gccPath, [...compileFlags, "-DRUN_UVE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(gccPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_uve}`]);

	/* Compile and link each kernel file */
	compileKernel(gccPath, [...compileFlags, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(gccPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_simple}`]);

	/* Compile for RVV with clang */
	compileKernel(clangPath, [...clangFlags, "-I..", "../Functions.c", "-c"]);
	compileKernel(clangPath, [...clangFlags, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
	compileKernel(clangPath, [...clangFlags, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"], true);
	compileKernel(clangPath, [...clangFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_rvv}`]);

	/* Run each kernel file */
	const execSimple = executableRun(spikePath, [pkPath, `benchmarks/${kernel}/${bin_simple}`, kernel]);
	const execUVE = executableRun(spikePath, [pkPath, `benchmarks/${kernel}/${bin_uve}`, kernel]);
	const execRVV = executableRun(spikePath, ["--isa=rv64gcv",  "--varch=vlen:512,elen:64", pkPath, `benchmarks/${kernel}/${bin_rvv}`, kernel]);

	const objDump = spawnSync("/home/afernandes/LLVM-Compiler/llvm-project/build/bin/llvm-objdump", ["--mattr=rv64gcv",  "-d", "kernel.o"]);
	const stdoutO = objDump.stdout.toString();
	fs.writeFile(`benchmarks/${kernel}/rvv.dump`, stdoutO, (err) => {
		if (err) throw err;
	});

	/* Test if generated values are similar */

	if (aproximateEqual(execSimple.stdout.toString(),  execUVE.stdout.toString(), execRVV.stdout.toString(), kernel)) {
		console.log(`Kernel ${kernel} is similar enough`);
	} else {
		console.error(`Kernel ${kernel}: Did not generate result similar enough`);
		//break;
	}

	// Delete executables for next kernel
	const del = spawnSync("rm", ["-f", bin_simple, bin_uve, 'main.o', 'kernel.o', 'Functions.o']);
	if (del.error) {
		console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
		break;
	}

}