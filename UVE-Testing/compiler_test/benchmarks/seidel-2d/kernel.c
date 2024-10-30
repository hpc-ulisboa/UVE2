// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/stencils/seidel-2d
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, uint64_t N, uint64_t STEPS) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

    int t, i, j;

    for (t = 0; t <= STEPS - 1; t++)
        for (i = 1; i <= N - 2; i++)
            for (j = 1; j <= N - 2; j++)
	            A[i*N+j] = (A[(i-1)*N+j-1] + A[(i-1)*N+j] + A[(i-1)*N+j+1] + A[i*N+j-1] + A[i*N+j] + A[i*N+j+1] + A[(i+1)*N+j-1] + A[(i+1)*N+j] + A[(i+1)*N+j+1])/9.0;
   
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}