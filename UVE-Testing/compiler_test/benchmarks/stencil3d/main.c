#include "Functions.h"
#include <stdio.h>

extern void core(DataType *sol, DataType *orig, DataType *C, int row_size, int col_size, int height_size);

int main() {
    int row_size = SIZE*SIZE;
    int col_size = SIZE;
    int height_size = SIZE;

    DataType sol[row_size*col_size];
    DataType orig[row_size*col_size];
    DataType C[2];
    // 3D discrete Laplacian
    C[0] = 6; C[1] = -1;
    
    initArray2D(orig, row_size, col_size*height_size);
    initZero(sol, row_size*col_size*height_size);

    core(sol, orig, C, row_size, col_size, height_size);

    for (int i = 0; i < row_size*col_size*height_size; i++) {
        printf(DataFormat("", "\n"), sol[i]);
    }

    return 0;
}
