#include "Functions.h"
#include <stdio.h>

extern void core(DataType *sol, DataType *orig, DataType *C, int row_size, int col_size, int height_size, DataType *sum0, DataType *sum1, DataType *mul0, DataType *mul1);

int main() {
    int row_size = SIZE*SIZE;
    int col_size = SIZE;
    int height_size = SIZE;

    DataType sol[row_size*col_size];
    DataType orig[row_size*col_size];
    DataType C[2];
    // 3D discrete Laplacian
    C[0] = 6; C[1] = -1;

    DataType sum0[row_size-2];
    DataType sum1[row_size-2];
    DataType mul0[row_size-2];
    DataType mul1[row_size-2];
    
    initArray2D(orig, row_size, col_size*height_size);
    initZero(sol, row_size*col_size*height_size);

    core(sol, orig, C, row_size, col_size, height_size, sum0, sum1, mul0, mul1);

    for (int i = 0; i < row_size*col_size*height_size; i++) {
        printf(DataFormat("", "\n"), sol[i]);
    }

    return 0;
}
