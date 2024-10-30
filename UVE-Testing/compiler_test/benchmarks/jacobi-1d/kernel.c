// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/stencils/jacobi-1d
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
 
    for (int i = 1; i < SIZE - 1; ++i)
        B[i] = ct * (A[i - 1] + A[i] + A[i + 1]);

    for (int i = 1; i < SIZE - 1; ++i)
        A[i] = ct * (B[i - 1] + B[i] + B[i + 1]);

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
