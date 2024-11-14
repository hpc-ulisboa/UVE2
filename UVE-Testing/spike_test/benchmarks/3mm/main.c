#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, DataType *B, DataType *C, DataType *D, DataType *E, DataType *F, DataType *G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M);

int main() {
    size_t I = SIZE-10;
    size_t J = SIZE-8;
    size_t K = SIZE;
    size_t L = SIZE+8;
    size_t M = SIZE+10;

    DataType A[I * K];
    DataType B[K * J];
    DataType C[J * M];
    DataType D[M * L];
    DataType E[I * J];
    DataType F[J * L];
    DataType G[I * L];

    initArray2D(A, I, K);
    initArray2D(B, K, J);
    initArray2D(C, J, M);
    initArray2D(D, M, L);
    initZero(E, I * J);
    initZero(F, J * L);
    initZero(G, I * L);

    core(A, B, C, D, E, F, G, I, J, K, L, M);

    for (int i = 0; i < I; i++) {
        for (int j = 0; j < L; j++)
            printf(DataFormat("", "\n"), G[i * L + j]);
    }

    return 0;
}
