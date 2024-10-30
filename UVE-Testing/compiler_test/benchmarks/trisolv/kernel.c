// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/solvers/trisolv
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *L, DataType *b, DataType *x) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    for (int i = 0; i < SIZE; ++i) {
        x[i] = b[i];
        for (int j = 0; j < i; ++j) {
            x[i] -= L[i * SIZE + j] * x[j];
        }
        x[i] /= L[i * SIZE + i];
    }
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}