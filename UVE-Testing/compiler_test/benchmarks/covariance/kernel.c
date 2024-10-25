#include "Functions.h"

long int start = 0, end = 0;

void core(int sizeM, int sizeN, DataType datat_n, DataType *data, DataType *cov, DataType *mean) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    int i, j, k;

    // kernel 1
    for (j = 0; j < sizeM; j++) {
        mean[j] = 0;
        for (i = 0; i < sizeN; i++)
            mean[j] += data[i * sizeM + j];
        mean[j] /= datat_n;
    }

    // kernel 2
    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeM; j++)
            data[i * sizeM + j] -= mean[j];

    // kernel 3
    for (i = 0; i < sizeM; i++)
        for (j = i; j < sizeM; j++) {
            cov[i * sizeM + j] = 0;
            for (k = 0; k < sizeN; k++)
                cov[i * sizeM + j] += data[k * sizeM + i] * data[k * sizeM + j];
            cov[i * sizeM + j] /= (DataType)(datat_n - 1.0);
            cov[j * sizeM + i] = cov[i * sizeM + j];
        }
    
    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

