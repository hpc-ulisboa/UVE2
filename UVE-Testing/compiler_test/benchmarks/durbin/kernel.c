#include "Functions.h"
#include <math.h>

long int start = 0, end = 0;

void core(DataType *r, DataType *y, DataType *sum, uint64_t N) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    
    int i, k;

    DataType z[N];
    DataType alpha;
    DataType beta;

    y[0] = -r[0];
    beta = 1.0;
    alpha = -r[0];

    for (k = 1; k < N; k++) {
        beta = (1 - alpha * alpha) * beta;
        for (i = 0; i < k; i++) {
            sum[k] += r[k - i - 1] * y[i];
        }
        alpha = -(r[k] + sum[k]) / beta;

        for (i = 0; i < k; i++) {
            z[i] = y[i] + alpha * y[k - i - 1];
        }
        for (i = 0; i < k; i++) {
            y[i] = z[i];
        }
        y[k] = alpha;
    }
    
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}