// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/kernels/bicg
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, DataType *s, DataType *q, DataType *p, DataType *r, uint64_t N, uint64_t M) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            s[j] += r[i] * A[i*M+j];
            q[i] += A[i*M+j] * p[j];
        }
    }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}