#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *val, uint32_t *cols, uint32_t *rowDelimiters, DataType *vec, DataType *out, uint64_t N, uint64_t K) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    DataType t;
    int cur_nnz;

    for (int i = 0; i < N; i++) {
        out[i] = 0.0;
        cur_nnz = rowDelimiters[i];

        for (int j = 0; j < cur_nnz; j++) {
            out[i] += val[i * K + j] * vec[cols[i * K + j]];
        }
    }

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}