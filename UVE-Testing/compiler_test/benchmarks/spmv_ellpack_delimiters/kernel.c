 // https://github.com/breagen/MachSuite/tree/master/spmv/ellpack (adapted)
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *val, uint32_t *cols, uint32_t *rowDelimiters, DataType *vec, DataType *out, uint64_t N, uint64_t K) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
    int cur_nnz;
#ifdef RVV
    DataType Si;
    int i, j;
    for (i = 0; i < N; i++) {
        DataType sum = out[i];
        cur_nnz = rowDelimiters[i];
        for (j = 0; j < cur_nnz; j++) {
            Si = val[i * K + j] * vec[cols[i * K + j]];
            sum += Si;
        }
        out[i] = sum;
    }
#else
    for (int i = 0; i < N; i++) {
        cur_nnz = rowDelimiters[i];
        for (int j = 0; j < cur_nnz; j++) {
            out[i] += val[i * K + j] * vec[cols[i * K + j]];
        }
    }
#endif
    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}