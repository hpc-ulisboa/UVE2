#include "Functions.h"
#include <stdio.h>

extern void core(int N, int M, DataType alpha, DataType beta, DataType *C /* N * N */, DataType *A /* N * M */);

int main() {
    int N = SIZE;
    int M = SIZE;

    DataType A[N * M];
    DataType C[N * N];
    DataType a = 1.5;
    DataType b = 1.2;

    initArray2D(A, N, M);
    initArray2D(C, N, N);

    core(N, M, a, b, C, A);

    for (int i = 0; i < N; ++i)
		for (int j = 0; j <= i; ++j)
        	printf(DataFormat("", "\n"), C[i*N+j]);

    return 0;
}
