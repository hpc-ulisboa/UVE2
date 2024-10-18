#include "Functions.h"
#include <stdio.h>

extern long int core_kernel(DataType *src1, DataType *src2, DataType *src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK);

void core(DataType* A, DataType* B, DataType* C, DataType* D, DataType* E, DataType* F, DataType* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M){

	long int a = core_kernel(A, B, E, I, J, K);
	a += core_kernel(C, D, F, J, L, M);
	a += core_kernel(E, F, G, I, L, J);

	printf("%d\n%ld\n", 0, a);
}


int main() {
    size_t I = SIZE;
    size_t J = SIZE;
    size_t K = SIZE;
    size_t L = SIZE;
    size_t M = SIZE;

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
