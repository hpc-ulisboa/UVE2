// https://github.com/breagen/MachSuite/tree/master/gemm/blocked
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *m1, DataType *m2, DataType *prod, int row_size, int block_size) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, k, j, jj, kk;

    for (jj = 0; jj < row_size; jj += block_size) {
        for (kk = 0; kk < row_size; kk += block_size) {
            for (i = 0; i < row_size; ++i) {
                for (k = 0; k < block_size; ++k) {
                    for (j = 0; j < block_size; ++j) {
                        prod[i * row_size + j + jj] += m1[i * row_size + k + kk] * m2[(k + kk) * row_size + j + jj];
                        //printf("MUL   %lf * %lf = %lf\n", m1[i * row_size + k + kk], m2[(k + kk) * row_size + j + jj], m1[i * row_size + k + kk] * m2[(k + kk) * row_size + j + jj]);
                        //printf("ADD = %.5lf\n", prod[i * row_size + j + jj]);
                        //printf("jj = %d, kk = %d, i = %d, k = %d, j = %d\n", jj, kk, i, k, j);
                    }
                    //printf("\n");
                }
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
 