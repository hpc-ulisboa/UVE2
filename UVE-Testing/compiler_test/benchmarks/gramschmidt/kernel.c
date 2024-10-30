// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/solvers/gramschmidt
#include "Functions.h"
#include <math.h>

long int start = 0, end = 0;

void core(int M, int N, DataType *A, DataType *R, DataType *Q) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, j, k;

    for (k = 0; k < N; k++) {
        R[k*N+k] = 0.0;
        for (i = 0; i < M; i++){
            R[k*N+k] += A[i*N+k] * A[i*N+k];
        }
        R[k*N+k] = sqrt(R[k*N+k]);
        for (i = 0; i < M; i++)
            Q[i*N+k] = A[i*N+k] / R[k*N+k];
        for (j = k + 1; j < N; j++) {
            R[k*N+j] = 0.0;
            for (i = 0; i < M; i++){
                R[k*N+j] += Q[i*N+k] * A[i*N+j];
            }
            for (i = 0; i < M; i++){
                A[i*N+j] -= Q[i*N+k] * R[k*N+j];
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}