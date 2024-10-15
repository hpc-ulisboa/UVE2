#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *L, DataType *b, DataType *x) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    for (k = 0; k < _PB_N; k++) {
        nrm = SCALAR_VAL(0.0);
        for (i = 0; i < _PB_M; i++)
            nrm += A[i][k] * A[i][k];
        R[k][k] = SQRT_FUN(nrm);
        for (i = 0; i < _PB_M; i++)
            Q[i][k] = A[i][k] / R[k][k];
        for (j = k + 1; j < _PB_N; j++) {
            R[k][j] = SCALAR_VAL(0.0);
            for (i = 0; i < _PB_M; i++)
                R[k][j] += Q[i][k] * A[i][j];
            for (i = 0; i < _PB_M; i++)
                A[i][j] = A[i][j] - Q[i][k] * R[k][j];
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}