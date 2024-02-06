
### Attempting to compile and run kernel saxpy...

UVE executed 324 less instructions
RVV executed 261 less instructions
UVE executed 63 less instructions than RVV
| Original | RVV | Difference | Difference (%) |
|----------|-----|------------|----------------|
| 353      | 92  | 261        | 73.94          |
| Original | UVE | Difference | Difference (%) |
| 353      | 29  | 324        | 91.78          |
| RVV      | UVE | Difference | Difference (%) |
| 92       | 29  | 63         | 68.48          |
Kernel saxpy is similar enough

### Attempting to compile and run kernel memcpy...

UVE executed 234 less instructions
RVV executed 182 less instructions
UVE executed 52 less instructions than RVV
| Original | RVV | Difference | Difference (%) |
|----------|-----|------------|----------------|
| 253      | 71  | 182        | 71.94          |
| Original | UVE | Difference | Difference (%) |
| 253      | 19  | 234        | 92.49          |
| RVV      | UVE | Difference | Difference (%) |
| 71       | 19  | 52         | 73.24          |
Kernel memcpy is similar enough

### Attempting to compile and run kernel jacobi-1d...

UVE executed 901 less instructions
RVV executed 766 less instructions
UVE executed 135 less instructions than RVV
| Original | RVV | Difference | Difference (%) |
|----------|-----|------------|----------------|
| 967      | 201 | 766        | 79.21          |
| Original | UVE | Difference | Difference (%) |
| 967      | 66  | 901        | 93.17          |
| RVV      | UVE | Difference | Difference (%) |
| 201      | 66  | 135        | 67.16          |
Kernel jacobi-1d is similar enough

### Attempting to compile and run kernel jacobi-2d...

UVE executed 75533 less instructions
RVV executed 64590 less instructions
UVE executed 10943 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 79027    | 14437 | 64590      | 81.73          |
| Original | UVE   | Difference | Difference (%) |
| 79027    | 3494  | 75533      | 95.58          |
| RVV      | UVE   | Difference | Difference (%) |
| 14437    | 3494  | 10943      | 75.80          |
Kernel jacobi-2d is similar enough

### Attempting to compile and run kernel 3mm...

UVE executed 2506216 less instructions
RVV executed 412467 more instructions
UVE executed 2918683 less instructions than RVV
| Original | RVV     | Difference | Difference (%) |
|----------|---------|------------|----------------|
| 2712305  | 3124772 | -412467    | -15.21         |
| Original | UVE     | Difference | Difference (%) |
| 2712305  | 206089  | 2506216    | 92.40          |
| RVV      | UVE     | Difference | Difference (%) |
| 3124772  | 206089  | 2918683    | 93.40          |
Kernel 3mm is similar enough

### Attempting to compile and run kernel trisolv...

UVE: Values were -3798025834736546.500000 and -3798025834736547.500000 with difference of 1 at index 46
UVE executed 8484 less instructions
RVV executed 1431 more instructions
UVE executed 9915 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 9274     | 10705 | -1431      | -15.43         |
| Original | UVE   | Difference | Difference (%) |
| 9274     | 790   | 8484       | 91.48          |
| RVV      | UVE   | Difference | Difference (%) |
| 10705    | 790   | 9915       | 92.62          |
Kernel trisolv: Did not generate result similar enough

### Attempting to compile and run kernel stream...

UVE executed 1279 less instructions
RVV executed 1019 less instructions
UVE executed 260 less instructions than RVV
| Original | RVV | Difference | Difference (%) |
|----------|-----|------------|----------------|
| 1365     | 346 | 1019       | 74.65          |
| Original | UVE | Difference | Difference (%) |
| 1365     | 86  | 1279       | 93.70          |
| RVV      | UVE | Difference | Difference (%) |
| 346      | 86  | 260        | 75.14          |
Kernel stream is similar enough

### Attempting to compile and run kernel mvt...

UVE executed 33092 less instructions
RVV executed 5299 more instructions
UVE executed 38391 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 35609    | 40908 | -5299      | -14.88         |
| Original | UVE   | Difference | Difference (%) |
| 35609    | 2517  | 33092      | 92.93          |
| RVV      | UVE   | Difference | Difference (%) |
| 40908    | 2517  | 38391      | 93.85          |
Kernel mvt is similar enough

### Attempting to compile and run kernel gemver...

UVE executed 66306 less instructions
RVV executed 18154 less instructions
UVE executed 48152 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 71313    | 53159 | 18154      | 25.46          |
| Original | UVE   | Difference | Difference (%) |
| 71313    | 5007  | 66306      | 92.98          |
| RVV      | UVE   | Difference | Difference (%) |
| 53159    | 5007  | 48152      | 90.58          |
Kernel gemver is similar enough

### Attempting to compile and run kernel gemm...

UVE executed 1082238 less instructions
RVV executed 856652 less instructions
UVE executed 225586 less instructions than RVV
| Original | RVV    | Difference | Difference (%) |
|----------|--------|------------|----------------|
| 1153013  | 296361 | 856652     | 74.30          |
| Original | UVE    | Difference | Difference (%) |
| 1153013  | 70775  | 1082238    | 93.86          |
| RVV      | UVE    | Difference | Difference (%) |
| 296361   | 70775  | 225586     | 76.12          |
Kernel gemm is similar enough

### Attempting to compile and run kernel convolution...

UVE executed 143182 less instructions
RVV executed 159253 less instructions
UVE executed 16071 more instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 170843   | 11590 | 159253     | 93.22          |
| Original | UVE   | Difference | Difference (%) |
| 170843   | 27661 | 143182     | 83.81          |
| RVV      | UVE   | Difference | Difference (%) |
| 11590    | 27661 | -16071     | -138.66        |
Kernel convolution is similar enough

### Attempting to compile and run kernel sgd...

UVE executed 6352731 less instructions
RVV executed 586134 less instructions
UVE executed 5766597 less instructions than RVV
| Original | RVV     | Difference | Difference (%) |
|----------|---------|------------|----------------|
| 7031115  | 6444981 | 586134     | 8.34           |
| Original | UVE     | Difference | Difference (%) |
| 7031115  | 678384  | 6352731    | 90.35          |
| RVV      | UVE     | Difference | Difference (%) |
| 6444981  | 678384  | 5766597    | 89.47          |
Kernel sgd is similar enough

### Attempting to compile and run kernel covariance...

UVE executed 587421 less instructions
RVV executed 65874 less instructions
UVE executed 521547 less instructions than RVV
| Original | RVV    | Difference | Difference (%) |
|----------|--------|------------|----------------|
| 623430   | 557556 | 65874      | 10.57          |
| Original | UVE    | Difference | Difference (%) |
| 623430   | 36009  | 587421     | 94.22          |
| RVV      | UVE    | Difference | Difference (%) |
| 557556   | 36009  | 521547     | 93.54          |
Kernel covariance is similar enough

### Attempting to compile and run kernel spmv_ellpack...

UVE executed 46932 less instructions
RVV executed 9381 more instructions
UVE executed 56313 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 51390    | 60771 | -9381      | -18.25         |
| Original | UVE   | Difference | Difference (%) |
| 51390    | 4458  | 46932      | 91.33          |
| RVV      | UVE   | Difference | Difference (%) |
| 60771    | 4458  | 56313      | 92.66          |
Kernel spmv_ellpack is similar enough

### Attempting to compile and run kernel spmv_ellpack_delimiters...

UVE executed 21394 less instructions
RVV executed 679 more instructions
UVE executed 22073 less instructions than RVV
| Original | RVV   | Difference | Difference (%) |
|----------|-------|------------|----------------|
| 24881    | 25560 | -679       | -2.73          |
| Original | UVE   | Difference | Difference (%) |
| 24881    | 3487  | 21394      | 85.99          |
| RVV      | UVE   | Difference | Difference (%) |
| 25560    | 3487  | 22073      | 86.36          |
Kernel spmv_ellpack_delimiters is similar enough
