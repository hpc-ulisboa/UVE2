#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    int j;
    for (j=0; j<sizeN; j++)
        c[j] = a[j];
    
    for (j=0; j<sizeN; j++)
        b[j] = scalar*c[j];
    
    for (j=0; j<sizeN; j++)
        c[j] = a[j]+b[j];

    for (j=0; j<sizeN; j++)
        a[j] = b[j]+scalar*c[j];

    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}