#!/usr/bin/env node

const { spawnSync }= require("child_process");

//const kernels = [ "saxpy", "memcpy", "jacobi-1d" ];
//const kernels = [ "trisolv" ];
const kernels = [ "jacobi-2d" ];
const compileFlags = [ "-Wall", "-pedantic", "-DTYPE=3", "-DSIZE=4" ];
const linkFlags = [ "-Wall", "-pedantic", "-static" ];
const compilerPath = "/home/afernandes/install/uve_tc/bin/riscv64-unknown-elf-gcc";
const pkPath = "./pk";
const spikePath = "./spike";
const bin_simple = "./run_simple";
const bin_uve = "./run_uve";


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

function aproximateEqual(stdout1, stdout2) {
  const str1 = stdout1.split("\n");
  const str2 = stdout2.split("\n");
  if (str1.length !== str2.length) {
    console.log(`Tests did not generate same amount of values`);
    return false;
  }

  for (let i = 0; i < str1.length; i++) {
    const value1 = parseFloat(str1[i]);
    const value2 = parseFloat(str2[i]);

    const diff = Math.abs(value1 - value2);
    if (diff > 0.0001) {
      console.error(`Values were ${str1[i]} and ${str2[i]} with difference of ${diff} at index ${i}`);
      return false;
    }
  }

  return true;
}


for (let kernel of kernels) {
  console.log(`\n### Attempting to compile and run kernel ${kernel}...\n`);
  /* Compile Functions source files */
  compileKernel(compilerPath, [...compileFlags, "-O3", "benchmarks/Functions.c", "-c"]);
  compileKernel(compilerPath, [...compileFlags, "-O3", "-Ibenchmarks/", `benchmarks/${kernel}/main.c`, "-c"]);
  /* Compile and link each kernel file */
  compileKernel(compilerPath, [...compileFlags, "-DRUN_SIMPLE", "-Ibenchmarks/", "-O0", `benchmarks/${kernel}/kernel.c`, "-c" ]);
  compileKernel(compilerPath, [...linkFlags, "-O0", "Functions.o", `kernel.o`, `main.o`, "-o", bin_simple]);
  
  compileKernel(compilerPath, [...compileFlags, "-DRUN_UVE", "-Ibenchmarks/", "-O0", `benchmarks/${kernel}/kernel.c`, "-c" ]);
  compileKernel(compilerPath, [...linkFlags, "-O0", "Functions.o", `kernel.o`, `main.o`, "-o", bin_uve]);

  /* Run each kernel file */
  const execSimple = executableRun(spikePath, [pkPath, bin_simple]);
  const execUVE = executableRun(spikePath, [pkPath, bin_uve]);

  /* Test if generated values are similar */
  if (aproximateEqual(execSimple.stdout.toString(), execUVE.stdout.toString())) {
    console.log(`Kernel ${kernel} is similar enough`);
  } else {
    console.error(`Kernel ${kernel}: Did not generate result similar enough`);
    break;
  }

  // Delete executables for next kernel
  /*const del = spawnSync("rm", ["-f", bin_simple, bin_uve, 'main.o', 'kernel.o', 'Functions.o']);
  if (del.error) {
    console.error(`Kernel ${kernel}: An error occured while deleting files for next execution: ${del.error.message}`);
    break;
  }*/

}