#!/usr/bin/env node
const fs = require('node:fs');
const { spawnSync } = require("child_process");

// Function to exit the script with an error message
function exitWithError(message) {
    console.error(message);
    process.exit(1);
}

const riscvPath = process.env.RISCV;

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

const compileFlags = [
    "-O2",
    "--target=riscv64-unknown-elf",
    "-march=rv64imafdc",
    "-mabi=lp64d",
    `--sysroot=${riscvPath}/riscv64-unknown-elf`,
    `--gcc-toolchain=${riscvPath}`,
    `-I${riscvPath}/include`,
    "-gdwarf-4",
    "-fno-tree-vectorize",
    "-fno-unroll-loops"
];

const linkFlags = [
    "--target=riscv64-unknown-elf",
    "-march=rv64imafdc",
    "-mabi=lp64d",
    `--sysroot=${riscvPath}/riscv64-unknown-elf`,
    `--gcc-toolchain=${riscvPath}`,
    `-B${riscvPath}/bin`,
    `-fuse-ld=bfd`,
    "-Wl,--strip-debug"
];

const clangIRFlags = [
    "-O2",
    "--target=riscv64-unknown-elf",
    "-march=rv64imafdc",
    "-mabi=lp64d",
    `--sysroot=${riscvPath}/riscv64-unknown-elf`,
    `--gcc-toolchain=${riscvPath}`,
    `-I${riscvPath}/include`,
    "-fno-vectorize",
    "-fno-slp-vectorize",
    "-fdiscard-value-names",
    "-ffp-contract=off",
    "-emit-llvm",
    "-fno-unroll-loops"
];

const optFlags = ["-enable-new-pm=0", "-load", passPath, "-loop-simplify", "-legacy-stream-analysis"];
const llcFlags = ["-march=riscv64", "--mcpu=generic-rv64", "-mattr=+d,+m,+experimental-xuve"];

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
	"2mm": size,
	"3mm": size,
	"atax": size,
	"bicg": size,
	"cholesky": size,
	"convolution": size, 
	"covariance": size,
	"doitgen": 0,
	"fdtd-2d": size,
	"gemm": size,
	"gemm_blocked": size,
	"gemm_ncubed": size,
	"gemver": size,
	"gesummv": size,
	"gramschmidt": size,
	"jacobi-1d": size*size,
	"jacobi-2d": size,
	"knn": 0,
	"lu": size,
	"memcpy": size*size,
	"mvt": size, 
	"saxpy": size*size,
	"spmv_ellpack": 0,
	"spmv_ellpack_delimiters": 0,
	"stencil2d": size,
	"stream": size*size,
	"symm": size,
	"syrk": size,
	"syr2k": size,
	"trisolv": size,
	"trmm": size
	
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
	//"correlation": size
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
    function getMaxWidth(column, index) {
        return Math.max(...data.map(row => String(row[index]).length, 0));
    }

    const maxWidths = data[0].map((_, index) => getMaxWidth(data, index));

    function printHyphenRow() {
        const hyphenRow = maxWidths.map(width => '-'.repeat(width + 2));
        console.log(`|${hyphenRow.join('|')}|`);
    }

    function formatCell(value, maxWidth) {
        const padding = ' '.repeat(maxWidth - String(value).length);
        return ` ${value}${padding} `;
    }

    function printRow(row) {
        const formattedRow = row.map((value, index) => formatCell(value, maxWidths[index]));
        console.log(`|${formattedRow.join('|')}|`);
    }

    printHyphenRow();
    printRow(data[0]);

    data.slice(1).forEach((row) => {
        printHyphenRow();
        printRow(row);
    });
    printHyphenRow();
}

function executableRun(command, args) {
    const executable = spawnSync(command, args);
    if (executable.error) {
        exitWithError(`An error occurred while trying to run ${command}: ${executable.error.message}`);
    }
    if (executable.status !== 0) {
        exitWithError(`Execution failed\nStderr: ${executable.stderr.toString()}\nStdout: ${executable.stdout.toString()}`);
		// If the execution fails, we can log the error but continue with the next kernel
		//console.error(`Execution failed for command: ${command} ${args.join(" ")}\nStderr: ${executable.stderr.toString()}\nStdout: ${executable.stdout.toString()}`);
    }
    return executable;
}

function compileKernel(command, args, flag = false) {
    const executable = spawnSync(command, args);
    if (flag)
        console.log(executable.stderr.toString());

    if (executable.error) {
        exitWithError(`An error occurred while trying to compile ${command} ${args.join(" ")}: ${executable.error.message}`);
    }
    if (executable.status !== 0) {
        exitWithError(`Compilation failed\nStderr: ${executable.stderr.toString()}\nStdout: ${executable.stdout.toString()}`);
    }
}

function aproximateEqual(stdout1, stdout2, kernel, t, s, dir) {
    let flag = true;
    fs.writeFile(`${dir}/simple_uve.txt`, stdout1, (err) => {
        if (err) throw err;
    });

    fs.writeFile(`${dir}/uve.txt`, stdout2, (err) => {
        if (err) throw err;
    });

    const original = stdout1.trim().split("\n");
    const uve = stdout2.trim().split("\n");

    if (original.length !== uve.length) {
        console.log(`Tests did not generate same amount of values (original: ${original.length}, uve: ${uve.length})`);
        return false;
    }

    const start1 = parseInt(original[0]);
    const start2 = parseInt(uve[0]);
    const end1 = parseInt(original[1]);
    const end2 = parseInt(uve[1]);

    for (let i = 2; i < original.length; i++) {
        const value1 = parseFloat(original[i]);
        const value2 = parseFloat(uve[i]);

        const diff = Math.abs(value1 - value2);
        if (diff > 0.1) {
            console.error(`UVE: Values were ${original[i]} and ${uve[i]} with difference of ${diff} at index ${i-2}`);
            flag = false;
        }
    }

    const insnsO = end1 - start1;
    const insnsUVE = end2 - start2;
    const diffInsnsUVE = insnsUVE - insnsO;
    const diffInsnsPuve = (diffInsnsUVE / insnsO) * 100;

    if (diffInsnsUVE < 0)
        console.log(`UVE executed ${-diffInsnsUVE} fewer instructions`);
    else if (diffInsnsUVE > 0)
        console.log(`UVE executed ${diffInsnsUVE} more instructions`);

    const data = [
        ['Original', 'UVE', 'Difference', 'Difference (%)'],
        [insnsO, insnsUVE, diffInsnsUVE, diffInsnsPuve.toFixed(2)],
    ];

    adjustTableWidth(data);
    console.log(`\n`);

    const csv = `${kernel},${s},${typeMap[t]},${insnsO},${insnsUVE}\n`;
    fs.appendFile(csvFilename, csv, (err) => {
        if (err) throw err;
    });

    return flag;
}

// loop over kernelSizeMap keys
for (let kernel in kernelSizeMap) {
    for (let type in typeMap) {
        let dir; 
        let s = kernelSizeMap[kernel];
        
        if (s === 0) {
            dir = `benchmarks/${kernel}`;
        } else {
            dir = `benchmarks/${kernel}/runs_${type.toLowerCase()}_${size}`;
            if (!fs.existsSync(dir)) {
                fs.mkdirSync(dir, { recursive: true });
            }
        }

        console.log(`\n### Attempting to compile and run kernel ${kernel} (size: ${s}, type: ${typeMap[type]}) ...\n`);

        /* Compile source files */
        compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", "../Functions.c", "-c"]);
        compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/main.c`, "-c"]);
        compileKernel(clangPath, [...compileFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-c"]);

        /* Link and create non-UVE executable */
        compileKernel(clangPath, [...linkFlags, "Functions.o", "kernel.o", "main.o", "-o", `${dir}/${bin_simple}_uve`, "-lm"]);

        /* Create objdump file */
        let objDump = spawnSync(objDumpPathCLANG, ["-d", "kernel.o"]);
        fs.writeFile(`${dir}/simple.dump`, objDump.stdout.toString(), (err) => {
            if (err) throw err;
        });

        /* Generate kernel IR with LLVM */
        compileKernel(clangPath, [...clangIRFlags, `-D${type}_TYPE`, `-DSIZE=${s}`, "-I..", `benchmarks/${kernel}/kernel.c`, "-S", "-o", `kernel.ll`]);

        /* Apply UVE pass */
        compileKernel(optPath, [...optFlags, `kernel.ll`, "-S", "-o", `UVEkernel.ll`]);

        /* LL to Assembly */
        compileKernel(llcPath, [...llcFlags, `UVEkernel.ll`]);  

        /* Assembly to object */
        compileKernel(clangPath, [...compileFlags, "-c", "-o", "UVEkernel.o", `UVEkernel.s`]);

        /* Create objdump file */
        objDump = spawnSync(objDumpPathCLANG, ["-d", "UVEkernel.o"]);
        fs.writeFile(`${dir}/uve.dump`, objDump.stdout.toString(), (err) => {
            if (err) throw err;
        });

        /* Link UVE executable */
        compileKernel(clangPath, [...linkFlags, "Functions.o", "UVEkernel.o", "main.o", "-o", `${dir}/${bin_uve}`, "-lm"]);

        /* Run each kernel on Spike with PK */
        const execSimpleUVE = executableRun(spikePath, [/*"-l",`--log=${dir}/simple.log`,*/ pkPath, `${dir}/${bin_simple}_uve`, kernel]);
        const execUVE = executableRun(spikePath, [/*"-l",`--log=${dir}/uve.log`,*/ pkPath, `${dir}/${bin_uve}`, kernel]);

        /* Test if generated values match */
        if (aproximateEqual(execSimpleUVE.stdout.toString(), execUVE.stdout.toString(), kernel, type, s, dir)) {
            console.log(`Kernel ${kernel} is similar enough`);
        } else {
            console.error(`Kernel ${kernel}: Did not generate result similar enough`);
        }

        // Clean intermediate object and IR files
        const del = spawnSync("rm", ['-f', 'main.o', 'kernel.o', 'UVEkernel.o', 'kernel.ll', 'UVEkernel.ll', 'Functions.o']);
        if (del.error) {
            console.error(`Kernel ${kernel}: An error occurred while deleting files: ${del.error.message}`);
            break;
        }
    }
}