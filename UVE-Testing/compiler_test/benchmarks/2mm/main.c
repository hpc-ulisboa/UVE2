#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, DataType *B, DataType *C, DataType *D, DataType *tmp, DataType alpha, DataType beta, uint64_t I, uint64_t J, uint64_t K, uint64_t L);

int main() {
    size_t I = SIZE-8;
    size_t J = SIZE-4;
    size_t K = SIZE;
    size_t L = SIZE+4;

    DataType A[I * K];
    DataType B[K * J];
    DataType C[J * I];
    DataType D[I * L];
    DataType tmp[I][J];

    DataType alpha = 1.5;
    DataType beta = 1.2;

    for (int i = 0; i < I; i++)
        for (int j = 0; j < K; j++)
            A[i*K+j] = (DataType)((i * j + 1) % I) / I;
    for (int i = 0; i < K; i++)
        for (int j = 0; j < J; j++)
            B[i*J+j] = (DataType)(i * (j + 1) % J) / J;
    for (int i = 0; i < J; i++)
        for (int j = 0; j < L; j++)
            C[i*I+j] = (DataType)((i * (j + 3) + 1) % L) / L;
    for (int i = 0; i < I; i++)
        for (int j = 0; j < L; j++)
            D[i*L+j] = (DataType)(i * (j + 2) % K) / K;

    initZero(tmp, I * J);

    core(A, B, C, D, tmp, alpha, beta, I, J, K, L);

    for (int i = 0; i < I; i++)
        for (int j = 0; j < L; j++)
            printf(DataFormat("", "\n"), D[i * L + j]);

    return 0;
}
