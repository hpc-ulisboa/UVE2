#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

extern void core(int N, int M, DataType alpha, DataType *A, DataType *B);

int main() {
    int N = 24;
    int M = 20;

    // Dynamically allocate memory for large arrays
    DataType *A = (DataType *)malloc(M * M * sizeof(DataType));
    DataType *B = (DataType *)malloc(M * N * sizeof(DataType));

    DataType a = 1.5;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < i; j++) {
            A[i*M+j] = (DataType)((i + j) % M) / M;
        }
        A[i*M+i] = 1.0;
        for (int j = 0; j < N; j++) {
            B[i*N+j] = (DataType)((N + (i - j)) % N) / N;
        }
    }

    core(N, M, a, A, B);

    for (int i = 0; i < M*N; ++i)
        printf(DataFormat("", "\n"), B[i]);

    // Free dynamically allocated memory
    free(A);
    free(B);

    return 0;
}
