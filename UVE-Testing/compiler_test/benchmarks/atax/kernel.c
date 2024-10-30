// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/kernels/atax
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, DataType *x, DataType *y, DataType *tmp, uint64_t N, uint64_t M) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j;

    for (i = 0; i < M; i++) {
        tmp[i] = 0.0;
        for (j = 0; j < N; j++)
            tmp[i] += A[i*N+j] * x[j];
        for (j = 0; j < N; j++)
            y[j] += A[i*N+j] * tmp[i];
    }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}