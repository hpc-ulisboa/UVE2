// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/blas/gesummv
#include "Functions.h"

long int start = 0, end = 0;

void core(int N, DataType alpha, DataType beta, DataType *A, DataType *B, DataType *tmp, DataType *x, DataType *y) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j, k;

    for (i = 0; i < N; i++) {
        tmp[i] = 0;
        y[i] = 0;
        for (j = 0; j < N; j++) {
            tmp[i] = A[i*N+j] * x[j] + tmp[i];
            y[i] = B[i*N+j] * x[j] + y[i];
        }
        y[i] = alpha * tmp[i] + beta * y[i];
    }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}