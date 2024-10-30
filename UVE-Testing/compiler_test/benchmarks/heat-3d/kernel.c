// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/stencils/heat-3d
#include "Functions.h"

#define IDX(i, j, k, N) ((i) * (N) * (N) + (j) * (N) + (k))

long int start = 0, end = 0;

void core(DataType *A, DataType *B, int N, int STEPS) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int t, i, j, k;

    for (t = 1; t <= STEPS; t++) {
        for (i = 1; i < N - 1; i++) {
            for (j = 1; j < N - 1; j++) {
                for (k = 1; k < N - 1; k++) {
                    B[IDX(i,j,k,N)] = 0.125 * (A[IDX(i+1,j,k,N)] - 2.0) * A[IDX(i,j,k,N)] + A[IDX(i-1,j,k,N)] + 0.125 * (A[IDX(i,j+1,k,N)] - 2.0) * A[IDX(i,j,k,N)] + A[IDX(i,j-1,k,N)] + 0.125 * (A[IDX(i,j,k+1,N)] - 2.0) * A[IDX(i,j,k,N)] + A[IDX(i,j,k-1,N)] + A[IDX(i,j,k,N)];
                }
            }
        }
        for (i = 1; i < N - 1; i++) {
            for (j = 1; j < N - 1; j++) {
                for (k = 1; k < N - 1; k++) {
                    A[IDX(i,j,k,N)] = 0.125 * (B[IDX(i+1,j,k,N)] - 2.0) * B[IDX(i,j,k,N)] + B[IDX(i-1,j,k,N)] + 0.125 * (B[IDX(i,j+1,k,N)] - 2.0) * B[IDX(i,j,k,N)] + B[IDX(i,j-1,k,N)] + 0.125 * (B[IDX(i,j,k+1,N)] - 2.0) * B[IDX(i,j,k,N)] + B[IDX(i,j,k-1,N)] + B[IDX(i,j,k,N)];
                }
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}