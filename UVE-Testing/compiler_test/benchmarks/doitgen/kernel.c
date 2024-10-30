// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/kernels/doitgen
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, DataType *C4, DataType *sum, uint64_t R, uint64_t Q, uint64_t P) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int r, q, p, s;

    for (r = 0; r < R; r++)
        for (q = 0; q < Q; q++) {
            for (p = 0; p < P; p++) {
                sum[p] = 0.0;
                for (s = 0; s < P; s++){
                    sum[p] += A[(r * Q * P) + (q * P) + s] * C4[s * P + p];
                    //printf("MUL     %lf * %lf = %lf\n", A[(r * Q * P) + (q * P) + s], C4[s * P + p], A[(r * Q * P) + (q * P) + s] * C4[s * P + p]);
                }
            }
            for (p = 0; p < P; p++){
                A[(r * Q * P) + (q * P) + p] = sum[p];
                //printf("%lf\n", sum[p]);
            }
        }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}