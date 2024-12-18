// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/solvers/cholesky
#include "Functions.h"
#include <math.h>

long int start = 0, end = 0;

void core(DataType *A, uint64_t N) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    int i, j, k;

    for (i = 0; i < N; i++) {
        // j<i
        for (j = 0; j < i; j++) {
            for (k = 0; k < j; k++) {
                A[i*N+j] -= A[i*N+k] * A[j*N+k];
            }
            A[i*N+j] /= A[j*N+j];
        }
        // i==j case
        for (k = 0; k < i; k++) {
            A[i*N+i] -= A[i*N+k] * A[i*N+k];
        }
        A[i*N+i] = sqrt(A[i*N+i]);
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}