#include "Functions.h"
#include <stdio.h>

extern void core(int N, int M, DataType alpha, DataType beta, DataType *C /* N * N */, DataType *A /* N * M */);

int main() {
    int N = 3;
    int M = 2;

    DataType A[N * M];
    DataType C[N * N];
    DataType a = 1.5;
    DataType b = 1.2;

    initArray2D(A, N, M);
    initArray2D(C, N, N);

    printf("C:\n");
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j)
			printf(DataFormat("", " "), C[i*N+j]);
		printf("\n");
	}

	printf("\nA:\n");
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j)
			printf(DataFormat("", " "), A[i*M+j]);
		printf("\n");
	}

	printf("\n");

    core(N, M, a, b, C, A);

    for (int i = 0; i < N; ++i) {
		for (int j = 0; j <= i; ++j)
        	printf(DataFormat("", " "), C[i*N+j]);
		printf("\n");
	}

    return 0;
}
