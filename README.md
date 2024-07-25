# UVE 1.0
### Unlimited Vector Extension with Data Streaming Support

### This branch refers to the old specification of UVE

Check our paper here: https://ieeexplore.ieee.org/document/10563191

If you use this in your work, please cite as:

   > A. Fernandes, L. Crespo, N. Neves, P. Tomás, N. Roma and G. Falcao, "Functional Validation of the RISC-V Unlimited Vector Extension," in IEEE Embedded Systems Letters, doi: 10.1109/LES.2024.3416820.

Or in BibTeX:

```bibtex
@ARTICLE{10563191,
  author={Fernandes, Ana and Crespo, Luís and Neves, Nuno and Tomás, Pedro and Roma, Nuno and Falcao, Gabriel},
  journal={IEEE Embedded Systems Letters}, 
  title={Functional Validation of the RISC-V Unlimited Vector Extension}, 
  year={2024},
  volume={},
  number={},
  pages={1-1},
  keywords={Vectors;Registers;Instruction sets;Three-dimensional displays;Mathematical models;Codes;Jacobian matrices;Data Streaming;RISC-V;Instruction Set Simulator;ISA SIMD Extensions;Unlimited Vector Extension},
  doi={10.1109/LES.2024.3416820}}
```

For the initial specification (UVE 1.0) and performance evaluation, check our paper here: https://ieeexplore.ieee.org/document/9499750.

Alternatively you can also take a look at the ISCA presentation: https://youtu.be/8sXGF7nNw_8.

If you use this in your work, please cite as:

   > J. M. Domingos, N. Neves, N. Roma and P. Tomás, "Unlimited Vector Extension with Data Streaming Support," 2021 ACM/IEEE 48th Annual International Symposium on Computer Architecture (ISCA), 2021, pp. 209-222, doi: 10.1109/ISCA52012.2021.00025.

Or in BibTeX:

```bibtex
@inproceedings{domingos2021,
  author={Domingos, Joao Mario and Neves, Nuno and Roma, Nuno and Tomás, Pedro},
  booktitle={2021 ACM/IEEE 48th Annual International Symposium on Computer Architecture (ISCA)}, 
  title={Unlimited Vector Extension with Data Streaming Support}, 
  year={2021},
  volume={},
  number={},
  pages={209-222},
  doi={10.1109/ISCA52012.2021.00025}}
```
Compilation support is currently under development, check our paper here: https://ieeexplore.ieee.org/document/9772328.

If you use this in your work, please cite as:

   > N. Neves, J. M. Domingos, N. Roma, P. Tomás and G. Falcao, "Compiling for Vector Extensions With Stream-Based Specialization," in IEEE Micro, vol. 42, no. 5, pp. 49-58, 1 Sept.-Oct. 2022, doi: 10.1109/MM.2022.3173405.

Or in BibTeX:

```bibtex
@article{neves2022,
  author={Neves, Nuno and Domingos, Joao Mario and Roma, Nuno and Tomás, Pedro and Falcao, Gabriel},
  journal={IEEE Micro}, 
  title={Compiling for Vector Extensions With Stream-Based Specialization}, 
  year={2022},
  volume={42},
  number={5},
  pages={49-58},
  doi={10.1109/MM.2022.3173405}}
```

Note that this is a work-in-progress version and some bugs may still exist.

## Directory layout
```
   .
   ├── documentation.pdf            # Documentation
   ├── spike                        # Modified RISC-V ISA Simulator (Spike) with UVE support
   │   ├── ...
   │   ├── build                    # Build directory
   │   ├── riscv                    # RISC-V source code
   │   │   ├── descriptors.cc       # UVE descriptors source code
   │   │   ├── descriptors.h        # UVE descriptors header file
   │   │   ├── helpers.h            # Helper functions header file
   │   │   ├── insns
   |   |   |   └── ...              # RISC-V Instructions source code
   │   │   ├── streaming_unit.cc    # Streaming Unit source code
   │   │   ├── streaming_unit.h     # Streaming Unit header file
   │   │   └── ...
   │   └── ...
   └── UVE-Testing                  # Testing directory
       ├── ...
       ├── Dataset.h                # Float and integer datasets
       ├── Functions.c              # Benchmark support source code
       ├── Functions.h              # Benchmark support header file
       ├── generateData.js          # Dataset generation script
       └── spike_test
          ├── benchmarks
          │   ├── saxpy             # Saxpy benchmark
          │   │   ├── kernel.c      # Kernel source code
          │   │   ├── main.c        # Source code
          │   │   ├── run_simple    # (automatically generated) Executable (without UVE)
          │   │   ├── run_uve       # (automatically generated) Executable (with UVE)
          │   │   ├── simple.log    # (automatically generated) Execution log (without UVE)
          │   │   └── uve.log       # (automatically generated) Execution log (with UVE)
          │   └── ...               # Other benchmarks
          ├── results               # Results directory
          ├── clean.js              # Clean directories from run logs script
          └── validate.js           # Benchmark validation script
```
## Setup

A cross-compiler familiar with UVE is needed, install it from here: https://github.com/hpc-ulisboa/UVE/tree/uve_compiler.

You should also install the RISC-V Proxy Kernel, available at: https://github.com/riscv-software-src/riscv-pk.

In order to install Spike, run these commands, assuming that the `RISCV` environment variable is set to the RISC-V tools install path:

```sh
$ apt-get install device-tree-compiler
$ mkdir spike/build
$ cd spike/build
$ ../configure --prefix=$RISCV
$ sudo make -j$(nproc) -B
$ sudo make install
```

## Compiling and running the benchmarks

Run the `validate.js` script, after editing the relevant path variables and choosing the desired benchmarks from the available ones, in folder `UVE-testing/spike-test/benchmarks`. Compilation flags can be altered in this script:

* `TYPE` - dataset datatype
   * `TYPE B` - byte (hexadecimal int)
   * `TYPE H` - half-word (short int)
   * `TYPE I` - word (int)
   * `TYPE F` - word (float)
   * `TYPE D` - double (DEFAULT)

You can also define the size of the dataset:

* `SIZE` - size of the dataset (usually a matrix SIZE $\times$ SIZE)
   * `SIZE 50` - 50 $\times$ 50 matrix (DEFAULT)

For example, you can run:

```sh
$ node validate.js 20
```

There is also a `clean.js` script to delete all the files generated by the validation script.

Every UVE kernel is made to support each datatype, with the exception of 'sgd', 'spmv_ellpack', and 'spmv_ellpack_delimiters' (double only), as well as jacobi-1d' and 'jacobi-2d' (double and float only).

The validation script creates executable files in each benchmark directory, which can be ran on Spike as following:

```sh
$ (path_to_spike) (path_to_pk) (executable_name)
$ (path_to_spike) -d (path_to_pk) (executable_name) # spike debugger
$ (path_to_spike) -l (path_to_pk) (executable_name) # spike instruction log
```

It is recommended that, when using the debugger/log, the output is redirected to a file, for example:

```sh
$ (path_to_spike) -l (path_to_pk) (executable_name) &> log
```
