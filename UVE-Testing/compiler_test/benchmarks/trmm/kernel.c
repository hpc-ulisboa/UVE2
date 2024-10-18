#include "Functions.h"

long int start = 0, end = 0;

void core(int N, int M, DataType alpha, DataType *A, DataType *B) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j, k;

    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            for (k = i + 1; k < M; k++)
                B[i * N + j] += A[k * M + i] * B[k * N + j];
            B[i * N + j] *= alpha;
        }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}