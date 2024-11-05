#include "Functions.h"
#include <stdio.h>

extern void core(DataType *sol, DataType *orig, DataType *filter, int row_size, int col_size);

int main() {
    int row_size = SIZE*1.2;
    int col_size = SIZE;

    DataType sol[row_size*col_size];
    DataType orig[row_size*col_size];
    DataType filter[3*3];

    initArray2D(orig, row_size, col_size);
    initArray(filter, 3*3);
    initZero(sol, row_size*col_size);

    core(sol, orig, filter, row_size, col_size);

    for (int i = 0; i < row_size*col_size; i++) {
        printf(DataFormat("", "\n"), sol[i]);
    }

    return 0;
}
