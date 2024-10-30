// https://github.com/breagen/MachSuite/tree/master/gemm/ncubed
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *m1, DataType *m2, DataType *prod, int row_size, int col_size) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, j, k;

    for (i = 0; i < row_size; i++) {
        for (j = 0; j < col_size; j++) {
            for (k = 0; k < row_size; k++) {
                prod[i * col_size + j] += m1[i * col_size + k] * m2[k * col_size + j];
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
