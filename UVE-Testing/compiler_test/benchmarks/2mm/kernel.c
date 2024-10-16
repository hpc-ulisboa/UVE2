#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, DataType *B, DataType *C, DataType *D, DataType *tmp, DataType alpha, DataType beta, uint64_t I, uint64_t J, uint64_t K, uint64_t L) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i, j, k;

    /* D := alpha*A*B*C + beta*D */

    for (i = 0; i < I; i++)
        for (j = 0; j < J; j++)
            for (k = 0; k < K; ++k)
                tmp[i * J + j] += alpha * A[i * K + k] * B[k * J + j];
    for (i = 0; i < I; i++) {
        for (j = 0; j < L; j++){
            D[i * L + j] *= beta;
            for (k = 0; k < J; ++k)
                D[i * L + j] += tmp[i * J + k] * C[k * I + j];
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
