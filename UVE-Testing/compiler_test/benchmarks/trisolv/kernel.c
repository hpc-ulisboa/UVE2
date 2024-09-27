#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *L, DataType *b, DataType *x) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    for (int i = 0; i < SIZE; ++i) {
        x[i] = b[i];
        for (int j = 0; j < i; ++j) {
            //printf("read x[%d] = %f\n", i, x[i]);
            //printf("x[%d] -= [(L[%d][%d]=%f) * (x[%d]=%f)]=%f\n", i, i, j, L[i*SIZE+j], j, x[j], L[i*SIZE+j] * x[j]);
            x[i] -= L[i * SIZE + j] * x[j];
        }
        //printf("x[%d] = [(x[%d]=%f) / (L[%d][%d]=%f)] = %f\n", i, i, x[i], i, i, L[i*SIZE+i], x[i]/L[i*SIZE+i]);
        x[i] = x[i] / L[i * SIZE + i];
        //printf("store x[%d] = %f\n", i, x[i]);
    }
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}