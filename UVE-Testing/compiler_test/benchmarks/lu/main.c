#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, int N);

int main() {
    DataType A[SIZE * SIZE];

    initArray2D(A, SIZE, SIZE);

    for (int i = 0; i < SIZE*SIZE; i++) {
        A[i] /= 10;
    }

    core(A, SIZE);

    for (int i = 0; i < SIZE*SIZE; i++) {
        printf(DataFormat("", "\n"), A[i]);
    }

    return 0;
}