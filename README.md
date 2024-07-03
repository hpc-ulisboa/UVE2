# UVE 2.0
### Unlimited Vector Extension with Data Streaming Support

Check our paper here: https://ieeexplore.ieee.org/document/9499750.

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
          ├── clean.js              # Clean directories from run logs
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
   * `TYPE 1` - byte (hexadecimal int)
   * `TYPE 2` - half-word (short int)
   * `TYPE 3` - word (int)
   * `TYPE 4` - word (float)
   * `TYPE 5` - double (DEFAULT)

* `SIZE` - size of the dataset (usually a matrix SIZE $\times$ SIZE)
   * `SIZE 64` - 64 $\times$ 64 matrix (DEFAULT)

Every UVE kernel is made to support datatype `double`, but can easily be altered to support other datatypes. Keep this in mind when running the validation script, which should be updated accordingly.

Alternatively, each benchmark directory contains the executables with and without UVE (`run_uve` and `run_simple`, respectively, with `SIZE=50` and `TYPE=5`), which can be ran on Spike with the following commands:

```sh
$ (path_to_spike) (path_to_pk) (executable_name)
$ (path_to_spike) -d (path_to_pk) (executable_name) # spike debugger
```

It is recommended that, when using the debugger, the output is redirected to a file, for example:

```sh
$ (path_to_spike) -d (path_to_pk) (executable_name) &> log
```
