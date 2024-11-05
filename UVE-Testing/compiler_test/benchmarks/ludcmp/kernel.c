// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/solvers/lu
#include "Functions.h"

long int start = 0, end = 0;

// void core(DataType *A, int N) {
void core(DataType *A, DataType *b, DataType *x, DataType *y, int N, DataType *w) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, j, k;

    for (i = 0; i < N; i++) {
        for (j = 0; j < i; j++) {
            w[i] = A[i*N+j];
            for (k = 0; k < j; k++) {
                w[i] -= A[i*N+k] * A[k*N+j];
            }
            A[i*N+j] = w[i] / A[j*N+j];
        }
        for (j = i; j < N; j++) {
            w[i] = A[i*N+j];
            for (k = 0; k < i; k++) {
                w[i] -= A[i*N+k] * A[k*N+j];
            }
            A[i*N+j] = w[i];
        }
    }

    for (i = 0; i < N; i++) {
        w[i] = b[i];
        for (j = 0; j < i; j++)
            w[i] -= A[i*N+j] * y[j];
        y[i] = w[i];
    }

    for (i = N - 1; i >= 0; i--) {
        w[i] = y[i];
        for (j = i + 1; j < N; j++)
            w[i] -= A[i*N+j] * x[j];
        x[i] = w[i] / A[i*N+i];
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}