// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/stencils/adi
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *u, DataType *v, DataType *p, DataType *q, int N, int STEPS, DataType a, DataType b, DataType c, DataType d, DataType e, DataType f, DataType DX, DataType DY, DataType DT, DataType B1, DataType B2, DataType mul1, DataType mul2) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int t, i, j;

    for (t = 1; t <= STEPS; t++) {
        // Column Sweep
        for (i = 1; i < N - 1; i++) {
            v[0*N+i] = 1.0;
            p[i*N+0] = 0.0;
            q[i*N+0] = v[0*N+i];
            for (j = 1; j < N - 1; j++) {
                p[i*N+j] = -c / (a * p[i*N+j - 1] + b);
                q[i*N+j] = (-d * u[j*N+i - 1] + (1.0 + 2.0 * d) * u[j*N+i] - f * u[j*N+i + 1] - a * q[i*N+j - 1]) / (a * p[i*N+j - 1] + b);
            }

            v[N - 1*N+i] = 1.0;
            for (j = N - 2; j >= 1; j--) {
                v[j*N+i] = p[i*N+j] * v[j + 1*N+i] + q[i*N+j];
            }
        }
        // Row Sweep
        for (i = 1; i < N - 1; i++) {
            u[i*N+0] = 1.0;
            p[i*N+0] = 0.0;
            q[i*N+0] = u[i*N+0];
            for (j = 1; j < N - 1; j++) {
                p[i*N+j] = -f / (d * p[i*N+j - 1] + e);
                q[i*N+j] = (-a * v[i - 1*N+j] + (1.0 + 2.0 * a) * v[i*N+j] - c * v[i + 1*N+j] - d * q[i*N+j - 1]) / (d * p[i*N+j - 1] + e);
            }
            u[i*N+N - 1] = 1.0;
            for (j = N - 2; j >= 1; j--) {
                u[i*N+j] = p[i*N+j] * u[i*N+j + 1] + q[i*N+j];
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}