# UVE 2.0
Unlimited Vector Extension with Data Streaming Support

Check our paper here: https://ieeexplore.ieee.org/document/9499750.

Alternatively you can also take a look at the ISCA presentation: https://youtu.be/8sXGF7nNw_8.

If you use this in your work, please cite as:

   > J. M. Domingos, N. Neves, N. Roma and P. Tomás, "Unlimited Vector Extension with Data Streaming Support," 2021 ACM/IEEE 48th Annual International Symposium on Computer Architecture (ISCA), 2021, pp. 209-222, doi: 10.1109/ISCA52012.2021.00025.

For work related to the compiler, check our paper here: https://ieeexplore.ieee.org/document/9772328.

If you use this in your work, please cite as:

   > N. Neves, J. M. Domingos, N. Roma, P. Tomás and G. Falcao, "Compiling for Vector Extensions With Stream-Based Specialization," in IEEE Micro, vol. 42, no. 5, pp. 49-58, 1 Sept.-Oct. 2022, doi: 10.1109/MM.2022.3173405.

To see the sources and documentation, refer to the following branches:

    uve_source: gem5 source with our modifications
    uve_examples_docs: documentation, examples and codes used for benchmarking
    uve_compiler: set of scripts and source code to patch and compile gcc with the UVE extension ISA

Note that this is a work-in-progress version and some bugs may still exist.

## Setup

A cross-compiler familiar with UVE is needed, install it from here: https://github.com/hpc-ulisboa/UVE/tree/uve_compiler.

In order to install Spike, run these commands, assuming that the RISCV environment variable is set to the RISC-V tools install path:
```
$ cd spike/build
$ ../configure --prefix=$RISCV
$ sudo make -j$(nproc) -B
$ sudo make install
```

## Compiling and running the benchmarks

Run the `validate.js` script, after editing the relevant path variables and choosing the desired benchmarks from the available ones, in folder `UVE-testing/spike-test/benchmarks``.