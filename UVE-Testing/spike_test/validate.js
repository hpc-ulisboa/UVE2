#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

//const kernels = ["saxpy", "memcpy", "jacobi-1d", "jacobi-2d", "3mm", "trisolv", "stream", "mvt", "gemver", "gemm", "convolution", "sgd", "covariance", "spmv_ellpack", "spmv_ellpack_delimiters"];

//const kernels = [ "floyd-warshall" ];
//const kernels = [ "knn" ];
//const kernels = [ "syrk" ];

const kernels = ["saxpy"];

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

const compileFlags = [/*"-O3", */"-Wall", "-pedantic", "-DTYPE=5", "-DSIZE=4"];
const linkFlags = [/*"-O3", */"-Wall", "-pedantic", "-static"];
const compilerPath = "/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-gcc";
const pkPath = "/home/afernandes/uve-dev/UVE-Testing/pk";
const spikePath = "/home/afernandes/uve-dev/UVE-Testing/spike";
const bin_simple = "./run_simple";
const bin_uve = "./run_uve";
const bin_blank = "./run_blank";

function adjustTableWidth(data) {
	// Function to calculate the maximum width of each column
	function getMaxWidth(column, index) {
		return Math.max(...data.map(row => String(row[index]).length, 0));
	}

	// Calculate the maximum width for each column
	const maxWidths = data[0].map((_, index) => getMaxWidth(data, index));

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
	printRow(data[0].map(header => '-'.repeat(header.length)));

	// Print each data row with adjusted width
	data.forEach((row, index) => {
		if (index === 1) {
			printRow(data[0].map(header => '-'.repeat(header.length)));
		}
		printRow(row);
	});
}

function executableRun(command, args) {
	const executable = spawnSync(command, args);
	if (executable.error) {
		throw new Error(`An error occured while trying to run ${command}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		throw new Error(`Issue in Kernel ${command}: Execution failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
	return executable;
}

function compileKernel(command, args) {
	const executable = spawnSync(command, args);
	if (executable.error) {
		throw new Error(`An error occured while trying to compile ${command}: ${executable.error.message}`);
	}
	if (executable.status != 0) {
		throw new Error(`Issue in Kernel ${command}: Compilation failed\nStderr: ${executable.stderr}\nStdout: ${executable.stdout}`);
	}
}

function aproximateEqual(stdout1, stdout2, stdout3, kernel) {
	let flag = true;
	/* Write log files */
	fs.writeFile(`benchmarks/${kernel}/simple.log`, stdout1, (err) => {
		if (err) throw err;
	});

	fs.writeFile(`benchmarks/${kernel}/uve.log`, stdout2, (err) => {
		if (err) throw err;
	});

	/* Compare values */
	const str1 = stdout1.split("\n");
	const str2 = stdout2.split("\n");
	const str3 = stdout3.split("\n");

	if (str1.length !== str2.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	let i;

	for (i = 0; i < str1.length - 5; i++) {
		const value1 = parseFloat(str1[i]);
		const value2 = parseFloat(str2[i]);

		const diff = Math.abs(value1 - value2);
		if (diff > 0.1) {
			console.error(`Values were ${str1[i]} and ${str2[i]} with difference of ${diff} at index ${i}`);
			flag = false;
		}
	}

	const ticks1 = parseInt(str1[i]);
	const ticks2 = parseInt(str2[i]);
	const cycles1 = parseInt(str1[i + 1]);
	const cycles2 = parseInt(str2[i + 1]);
	const instr1 = parseInt(str1[i + 2]);
	const instr2 = parseInt(str2[i + 2]);
	const cpi1 = parseFloat(str1[i + 3]);
	const cpi2 = parseFloat(str2[i + 3]);

	i = str3.length - 5;
	const ticks3 = parseInt(str3[i]);
	const cycles3 = parseInt(str3[i + 1]);
	const instr3 = parseInt(str3[i + 2]);
	const cpi3 = parseFloat(str3[i + 3]);

	const diffTicks = ticks1 - ticks2;
	const diffCycles = cycles1 - cycles2;
	const diffInstr = instr1 - instr2;

	// differences in percentage
	const diffTicksP = ((diffTicks / ticks1) * 100).toFixed(5);
	const diffCyclesP = ((diffCycles / cycles1) * 100).toFixed(5);
	const diffInstrP = ((diffInstr / instr1) * 100).toFixed(5);

	if (diffTicks > 0)
		console.log(`UVE was ${diffTicks} ticks faster`);
	else if (diffTicks < 0)
		console.log(`UVE was ${-diffTicks} ticks slower`);

	if (diffCycles > 0)
		console.log(`UVE was ${diffCycles} cycles faster`);
	else if (diffCycles < 0)
		console.log(`UVE was ${-diffCycles} cycles slower`);

	if (diffInstr > 0)
		console.log(`UVE executed ${diffInstr} less instructions`);
	else if (diffInstr < 0)
		console.log(`UVE executed ${-diffInstr} more instructions`);

	const data = [
		['Value', 'Original - Blank', 'UVE - Blank', 'Blank', 'Difference', 'Difference (%)'],
		['ticks', ticks1-ticks3, ticks2-ticks3, ticks3, diffTicks, diffTicksP],
		['cycles', cycles1-cycles3, cycles2-cycles3, cycles3, diffCycles, diffCyclesP],
		['instructions', instr1-instr3, instr2-instr3, instr3, diffInstr, diffInstrP],
		['cpi', cpi1, cpi2, cpi3, '-', '-']
	];

	// Call the function to adjust table width and print the table
	adjustTableWidth(data);

	return flag;
}

for (let kernel of kernels) {
	console.log(`\n### Attempting to compile and run kernel ${kernel}...\n`);

	/* Compile Functions source files */
	compileKernel(compilerPath, [...compileFlags, "-I..", "../Functions.c", "-c"]);
	compileKernel(compilerPath, [...compileFlags, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);

	/* Compile and link each kernel file */
	compileKernel(compilerPath, [...compileFlags, "-DRUN_BLANK", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(compilerPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_blank}`]);

	compileKernel(compilerPath, [...compileFlags, "-DRUN_UVE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(compilerPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_uve}`]);

	compileKernel(compilerPath, [...compileFlags, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(compilerPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_simple}`]);

	/* Run each kernel file */
	const execBlank = executableRun(spikePath, [pkPath, '-s', `benchmarks/${kernel}/${bin_blank}`, kernel]);
	const execSimple = executableRun(spikePath, [pkPath, '-s', `benchmarks/${kernel}/${bin_simple}`, kernel]);
	const execUVE = executableRun(spikePath, [pkPath, '-s', `benchmarks/${kernel}/${bin_uve}`, kernel]);

	/* Test if generated values are similar */

	if (aproximateEqual(execSimple.stdout.toString(), execUVE.stdout.toString(), execBlank.stdout.toString(), kernel)) {
		console.log(`Kernel ${kernel} is similar enough`);
	} else {
		console.error(`Kernel ${kernel}: Did not generate result similar enough`);
		//break;
	}

	/* Delete executables for next kernel
	const del = spawnSync("rm", ["-f", bin_simple, bin_uve, bin_blank, 'main.o', 'kernel.o', 'Functions.o']);
	if (del.error) {
		console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
		break;
	}*/

}