#include "Functions.h"

long int start = 0, end = 0;

void core(int N, int M, DataType alpha, DataType beta, DataType *C /* N * N */, DataType *A /* N * M */, DataType *B /* N * M */) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

	int i, j, k;

    for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++)
            C[i*N+j] *= beta;
        for (k = 0; k < M; k++)
            for (j = 0; j <= i; j++)
                C[i*N+j] += A[j*M+k] * alpha * B[i*M+k] + B[j*M+k] * alpha * A[i*M+k];
    }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}