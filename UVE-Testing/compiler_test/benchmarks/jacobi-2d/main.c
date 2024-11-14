#include "Functions.h"
#include <stdio.h>

extern void core(DataType *src1, DataType *src2);

int main() {
    DataType A[SIZE * SIZE];
    DataType B[SIZE * SIZE];

    initArray2D(A, SIZE, SIZE);
    initArray2D(B, SIZE, SIZE);

    core(A, B);

    for (int i = 0; i < SIZE * SIZE; ++i)
        printf(DataFormat("", "\n"), A[i]);

    return 0;
}
