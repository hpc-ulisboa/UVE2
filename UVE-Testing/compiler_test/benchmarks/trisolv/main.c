#include "Functions.h"
#include <stdio.h>

extern void core(DataType *x, DataType *y, DataType *z);

int main() {
    DataType L[SIZE * SIZE];
    DataType b[SIZE];
    DataType x[SIZE];

    initArray2D(L, SIZE, SIZE);
    initArray(b, SIZE);
    initArray(x, SIZE);

    for (int i = 0; i < SIZE; i++) {
        b[i] /= 500000;
        x[i] /= 500000;
        for (int j = 0; j < SIZE; j++)
            L[i * SIZE + j] *= 100;
    }

    core(L, b, x);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), x[i]);
    }

    return 0;
}
