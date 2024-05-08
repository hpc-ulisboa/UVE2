#include "Functions.h"
#include <stdio.h>

extern void core(DataType *a, DataType *A);

int main() {
    DataType A[SIZE * SIZE];
    DataType a[SIZE];

    initArray2D(A, SIZE, SIZE);
    initArray(a, SIZE);

    core(a, A);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), a[i]);
    }

    return 0;
}
