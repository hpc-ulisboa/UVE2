// https://github.com/breagen/MachSuite/tree/master/stencil/stencil3d
#include "Functions.h"

#define INDX(_row_size, _col_size, _i, _j, _k) ((_i) + _row_size * ((_j) + _col_size * (_k)))

long int start = 0, end = 0;

void core(DataType *sol, DataType *orig, DataType *C, int row_size, int col_size, int height_size, DataType *sum0, DataType *sum1, DataType *mul0, DataType *mul1) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, j, k;

    // Handle boundary conditions by filling with original values
    for (j = 0; j < col_size; j++) {
        for (k = 0; k < row_size; k++) {
            sol[INDX(row_size, col_size, k, j, 0)] = orig[INDX(row_size, col_size, k, j, 0)];
            sol[INDX(row_size, col_size, k, j, height_size - 1)] = orig[INDX(row_size, col_size, k, j, height_size - 1)];
        }
    }
    for (i = 1; i < height_size - 1; i++) {
        for (k = 0; k < row_size; k++) {
            sol[INDX(row_size, col_size, k, 0, i)] = orig[INDX(row_size, col_size, k, 0, i)];
            sol[INDX(row_size, col_size, k, col_size - 1, i)] = orig[INDX(row_size, col_size, k, col_size - 1, i)];
        }
    }
    for (i = 1; i < height_size - 1; i++) {
        for (j = 1; j < col_size - 1; j++) {
            sol[INDX(row_size, col_size, 0, j, i)] = orig[INDX(row_size, col_size, 0, j, i)];
            sol[INDX(row_size, col_size, row_size - 1, j, i)] = orig[INDX(row_size, col_size, row_size - 1, j, i)];
        }
    }

    // Stencil computation
    for (i = 1; i < height_size - 1; i++) {
        for (j = 1; j < col_size - 1; j++) {
            for (k = 1; k < row_size - 1; k++) {
                sum0[k] = orig[INDX(row_size, col_size, k, j, i)];
                sum1[k] = orig[INDX(row_size, col_size, k, j, i + 1)] +
                          orig[INDX(row_size, col_size, k, j, i - 1)] +
                          orig[INDX(row_size, col_size, k, j + 1, i)] +
                          orig[INDX(row_size, col_size, k, j - 1, i)] +
                          orig[INDX(row_size, col_size, k + 1, j, i)] +
                          orig[INDX(row_size, col_size, k - 1, j, i)];
                mul0[k] = sum0[k] * C[0];
                mul1[k] = sum1[k] * C[1];
                sol[INDX(row_size, col_size, k, j, i)] = mul0[k] + mul1[k];
            }
        }
    }
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}