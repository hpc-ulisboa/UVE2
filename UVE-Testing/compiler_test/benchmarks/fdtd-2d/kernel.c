#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *ex, DataType *ey, DataType *hz, DataType *fict, int NX, int NY, int TMAX) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int t, i, j;

    for (t = 0; t < TMAX; t++) {
        for (j = 0; j < NY; j++)
            ey[0*NY+j] = fict[t];
        for (i = 1; i < NX; i++)
            for (j = 0; j < NY; j++)
                ey[i*NY+j] = ey[i*NY+j] - 0.5 * (hz[i*NY+j] - hz[i - 1*NY+j]);
        for (i = 0; i < NX; i++)
            for (j = 1; j < NY; j++)
                ex[i*NY+j] = ex[i*NY+j] - 0.5 * (hz[i*NY+j] - hz[i*NY+j - 1]);
        for (i = 0; i < NX - 1; i++)
            for (j = 0; j < NY - 1; j++)
                hz[i*NY+j] = hz[i*NY+j] - 0.7 * (ex[i*NY+j + 1] - ex[i*NY+j] + ey[(i + 1)*NY+j] - ey[i*NY+j]);
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}