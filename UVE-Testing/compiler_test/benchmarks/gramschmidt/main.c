#include "Functions.h"
#include <stdio.h>

extern void core(int M, int N, DataType *A, DataType *R, DataType *Q);

int main() {
    int N = SIZE;
    int M = SIZE + SIZE / 5;

    DataType A[M * N];
    DataType R[N * N];
    DataType Q[M * N];

    initArray2D(A, M, N);
    initZero(R, N*N);
    initZero(Q, M*N);

    core(M, N, A, R, Q);

    for (int i = 0; i < N*N; i++) {
        printf(DataFormat("", "\n"), R[i]);
    }

    for (int i = 0; i < M*N; i++) {
        printf(DataFormat("", "\n"), Q[i]);
    }

    return 0;
}
