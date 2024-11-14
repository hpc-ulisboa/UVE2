#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *a, DataType *b, DataType *c, DataType *d, DataType *e, DataType *f, DataType *g, DataType *h, DataType *i, DataType *j, DataType *res) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    for (int k = 0; k < SIZE; k++) {
        j[k] = a[k] + b[k];
        //res[k] = a[k] + b[k] + c[k] + d[k] + e[k] + f[k] + g[k] + h[k] + i[k];
        res[k] += j[k];
        res[k] += j[k];
        res[k] += j[k];
        res[k] += j[k];
        res[k] += j[k];
        res[k] += j[k];
        //res[k] += j[k];
        //res[k] += j[k];
    }
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}