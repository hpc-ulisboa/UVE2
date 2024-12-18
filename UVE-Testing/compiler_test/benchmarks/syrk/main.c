#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

extern void core(int N, int M, DataType alpha, DataType beta, DataType *C, DataType *A);

int main() {
    int N = SIZE+SIZE/5;
    int M = SIZE;

    // Dynamically allocate memory for large arrays
    DataType *A = (DataType *)malloc(N * M * sizeof(DataType));
    DataType *C = (DataType *)malloc(N * N * sizeof(DataType));

    DataType a = 1.5;
    DataType b = 1.2;

    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++)
            A[i*M+j] = (DataType)((i * j + 1) % N) / N;
        for (int j = 0; j < N; j++)
            C[i*N+j] = (DataType)((i * j + 3) % N) / M;
	}

    core(N, M, a, b, C, A);

    for (int i = 0; i < N; ++i)
		for (int j = 0; j <= i; ++j)
        	printf(DataFormat("", "\n"), C[i*N+j]);
	
	// Free dynamically allocated memory
    free(A);
    free(C);

    return 0;
}
