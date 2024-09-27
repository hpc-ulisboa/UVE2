#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *val, uint32_t *cols, uint32_t *rowDelimiters, DataType *vec, DataType *out, uint64_t N, uint64_t K) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    DataType t;
    int cur_nnz;

    for (int i = 0; i < N; i++) {
        t = 0.0;
        cur_nnz = rowDelimiters[i];

        //printf("i: %d, cur_nnz: %d\n", i, cur_nnz);

        for (int j = 0; j < cur_nnz; j++) {
            //printf("val[%d]: %lf, cols[%d]: %d, vec[%d]: %lf\n", j, val[j], j, cols[j], cols[j], vec[cols[j]]);
            //t += val[j] * vec[cols[j]];
            t += val[i * K + j] * vec[cols[i * K + j]];
        }
        out[i] += t;

        //printf("out[%d]: %lf\n", i, out[i]);
    }

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}