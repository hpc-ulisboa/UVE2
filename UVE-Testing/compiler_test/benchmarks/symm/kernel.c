#include "Functions.h"

long int start = 0, end = 0;

void core(int N, int M, DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, DataType *temp2) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j, k;
	//DataType temp2[N];

    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            temp2[j] = 0.0;
            for (k = 0; k < i; k++) {
                C[k*N+j] += alpha * B[i*N+j] * A[i*M+k];
                temp2[j] += B[k*N+j] * A[i*M+k];
            }
            C[i*N+j] = beta * C[i*N+j] + alpha * B[i*N+j] * A[i*M+i] + alpha * temp2[j];
        }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}