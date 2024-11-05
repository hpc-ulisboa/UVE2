// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/datamining/covariance
#include "Functions.h"
#include <math.h>

long int start = 0, end = 0;

void core(int M, int N, DataType float_n, DataType *data, DataType *corr, DataType *mean, DataType *stddev, DataType eps) {
    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j, k;

    for (j = 0; j < M; j++) {
        mean[j] = 0.0;
        for (i = 0; i < N; i++)
            mean[j] += data[i * M + j];
        mean[j] /= float_n;
    }

    for (j = 0; j < M; j++) {
        stddev[j] = 0.0;
        for (i = 0; i < N; i++)
            stddev[j] += (data[i * M + j] - mean[j]) * (data[i * M + j] - mean[j]);
        stddev[j] /= float_n;
        stddev[j] = sqrt(stddev[j]);
        /* The following in an inelegant but usual way to handle
           near-zero std. dev. values, which below would cause a zero-
           divide. */
        stddev[j] = stddev[j] <= eps ? 1.0 : stddev[j];
    }

    /* Center and reduce the column vectors. */
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++) {
            data[i * M + j] -= mean[j];
            data[i * M + j] /= sqrt(float_n) * stddev[j];
        }

    /* Calculate the m * m correlation matrix. */
    for (i = 0; i < M - 1; i++) {
        corr[i * M + i] = 1.0;
        for (j = i + 1; j < M; j++) {
            corr[i * M + j] = 0.0;
            for (k = 0; k < N; k++)
                corr[i * M + j] += (data[k * M + i] * data[k * M + j]);
            corr[j * M + i] = corr[i * M + j];
        }
    }
    corr[(M - 1) * M + M - 1] = 1.0;

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
