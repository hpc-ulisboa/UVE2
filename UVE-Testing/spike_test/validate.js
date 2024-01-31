#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

//const kernels = ["saxpy", "memcpy", "jacobi-1d", "jacobi-2d", "3mm", "trisolv", "stream", "mvt", "gemver", "gemm", "convolution", "sgd", "covariance", "spmv_ellpack", "spmv_ellpack_delimiters"];

//const kernels = [ "floyd-warshall" ];
//const kernels = [ "knn" ];
//const kernels = [ "syrk" ];

const kernels = ["trisolv"];

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

const compileFlags = ["-O2", "-Wall", "-pedantic", "-DTYPE=5", "-DSIZE=50"];
const linkFlags = ["-O2", "-Wall", "-pedantic", "-static"];
const compilerPath = "/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-gcc";
const pkPath = "/home/afernandes/uve-dev/UVE-Testing/pk";
const spikePath = "/home/afernandes/uve-dev/UVE-Testing/spike";
const bin_simple = "./run_simple";
const bin_uve = "./run_uve";

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

function aproximateEqual(stdout1, stdout2, kernel) {
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

	if (str1.length !== str2.length) {
		console.log(`Tests did not generate same amount of values`);
		return false;
	}

	const start1 = parseInt(str1[1]);
	const start2 = parseInt(str2[1]);
	const end1 = parseInt(str1[2]);
	const end2 = parseInt(str2[2]);

	for (i = 3; i < str1.length; i++) {
		const value1 = parseFloat(str1[i]);
		const value2 = parseFloat(str2[i]);

		const diff = Math.abs(value1 - value2);
		if (diff > 0.1) {
			console.error(`Values were ${str1[i]} and ${str2[i]} with difference of ${diff} at index ${i-3}`);
			flag = false;
		}
	}

	const insns1 = end1-start1;
	const insns2 = end2-start2;
	const diffInsns = insns1-insns2;

	// difference in percentage
	const diffInsnsP = (diffInsns/insns1)*100;

	if (diffInsns > 0)
		console.log(`UVE executed ${diffInsns} less instructions`);
	else if (diffInsns < 0)
		console.log(`UVE executed ${-diffInsns} more instructions`);

	const data = [
		['Original', 'UVE', 'Difference', 'Difference (%)'],
		[insns1, insns2, diffInsns, diffInsnsP.toFixed(2)]
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

	
	compileKernel(compilerPath, [...compileFlags, "-DRUN_UVE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(compilerPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_uve}`]);

	/* Compile and link each kernel file */
	compileKernel(compilerPath, [...compileFlags, "-DRUN_SIMPLE", "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);
	compileKernel(compilerPath, [...linkFlags, "Functions.o", `kernel.o`, `main.o`, "-o", `benchmarks/${kernel}/${bin_simple}`]);


	/* Run each kernel file */
	const execSimple = executableRun(spikePath, [pkPath, `benchmarks/${kernel}/${bin_simple}`, kernel]);
	const execUVE = executableRun(spikePath, [pkPath, `benchmarks/${kernel}/${bin_uve}`, kernel]);

	/* Test if generated values are similar */

	if (aproximateEqual(execSimple.stdout.toString(), execUVE.stdout.toString(), kernel)) {
		console.log(`Kernel ${kernel} is similar enough`);
	} else {
		console.error(`Kernel ${kernel}: Did not generate result similar enough`);
		//break;
	}

	/* Delete executables for next kernel
	const del = spawnSync("rm", ["-f", bin_simple, bin_uve, 'main.o', 'kernel.o', 'Functions.o']);
	if (del.error) {
		console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
		break;
	}*/

}