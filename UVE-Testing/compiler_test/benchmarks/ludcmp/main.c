#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, DataType *b, DataType *x, DataType *y, int N, DataType *w);

int main() {
    DataType A[SIZE * SIZE];
    DataType b[SIZE];
    DataType x[SIZE];
    DataType y[SIZE];
    DataType w[SIZE];

    for (int i = 0; i < SIZE; i++) {
        x[i] = 0;
        y[i] = 0;
        b[i] = (i + 1) / (DataType)(SIZE) / 2.0 + 4;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j <= i; j++)
            A[i*SIZE+j] = (DataType)(-j % SIZE) / SIZE + 1;
        for (int j = i + 1; j < SIZE; j++) {
            A[i*SIZE+j] = 0;
        }
        A[i*SIZE+i] = 1;
    }

    core(A, b, x, y, SIZE, w);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), x[i]);
    }

    return 0;
}