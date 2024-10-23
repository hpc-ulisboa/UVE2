#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

extern void core(int N, int M, DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, DataType *temp2);

int main() {
    int N = SIZE+SIZE/5;
    int M = SIZE;

    // Dynamically allocate memory for large arrays
    DataType *A = (DataType *)malloc(M * M * sizeof(DataType));
    DataType *B = (DataType *)malloc(M * N * sizeof(DataType));
    DataType *C = (DataType *)malloc(M * N * sizeof(DataType));
    DataType *temp2 = (DataType *)malloc(N * sizeof(DataType));

    DataType a = 1.5;
    DataType b = 1.2;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) {
            C[i * N + j] = (DataType)((i + j) % 100) / M;
            B[i * N + j] = (DataType)((N + i - j) % 100) / M;
        }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j <= i; j++)
            A[i * M + j] = (DataType)((i + j) % 100) / M;
        for (int j = i + 1; j < M; j++)
            A[i * M + j] = -999; // regions of arrays that should not be used
    }

    core(N, M, a, b, C, A, B, temp2);

    for (int i = 0; i < M*N; ++i)
        printf(DataFormat("", "\n"), C[i]);

    // Free dynamically allocated memory
    free(A);
    free(C);
    free(B);
    free(temp2);

    return 0;
}
