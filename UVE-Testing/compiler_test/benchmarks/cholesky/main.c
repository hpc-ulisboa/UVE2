#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, uint64_t N);

int main() {
    DataType A[SIZE * SIZE];

    initArray2D(A, SIZE, SIZE);

    core(A, SIZE);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), A[i]);
    }

    return 0;
}
