#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

extern void core(int N, DataType alpha, DataType beta, DataType *tmp, DataType *A, DataType *B, DataType *x, DataType *y);

int main() {
    DataType A[SIZE * SIZE];
    DataType B[SIZE * SIZE];
    DataType x[SIZE];
    DataType y[SIZE];
    DataType tmp[SIZE];

    DataType a = 1.5;
    DataType b = 1.2;

    for (int i = 0; i < SIZE; i++) {
        x[i] = (DataType)(i % SIZE) / SIZE;
        for (int j = 0; j < SIZE; j++) {
            A[i*SIZE+j] = (DataType)((i * j + 1) % SIZE) / SIZE;
            B[i*SIZE+j] = (DataType)((i * j + 2) % SIZE) / SIZE;
        }
    }

    core(SIZE, a, b, tmp, A, B, x, y);
    
    for (int i = 0; i < SIZE; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
