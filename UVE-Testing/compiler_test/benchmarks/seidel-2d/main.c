#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, uint64_t N, uint64_t STEPS);

int main() {
    uint64_t STEPS = SIZE/4;

    DataType A[SIZE * SIZE];

    initArray2D(A, SIZE, SIZE);

    core(A, SIZE, STEPS);

    for (int i = 0; i < SIZE*SIZE; i++) {
        printf(DataFormat("", "\n"), A[i]);
    }

    return 0;
}
