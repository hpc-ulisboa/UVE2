#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *a, DataType *b, DataType *c, DataType *d, DataType *e, DataType *f, DataType *g, DataType *h, DataType *i, DataType *j, DataType *res) {
    // 1 Operation
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int i = 0; i < SIZE; i++)
        res[i] = a[i] + b[i];
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("1: %ld\n", end-start);

    // 2 Operations
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int i = 0; i < SIZE; i++)
        res[i] = a[i] + b[i] + c[i];
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("2: %ld\n", end-start);

    // 3 Operations
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int i = 0; i < SIZE; i++)
        res[i] = a[i] + b[i] + c[i] + d[i];
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("3: %ld\n", end-start);

    // 4 Operations
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int k = 0; k < SIZE; k++) {
        res[k] = a[k] + b[k] + c[k] + d[k] + e[k];
    }
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("4: %ld\n", end-start);

    // 5 Operations
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int k = 0; k < SIZE; k++) {
        res[k] = a[k] + b[k] + c[k] + d[k] + e[k] + f[k];
    }
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("5: %ld\n", end-start);
}