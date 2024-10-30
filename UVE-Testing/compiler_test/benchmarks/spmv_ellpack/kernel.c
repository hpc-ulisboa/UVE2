// https://github.com/breagen/MachSuite/tree/master/spmv/ellpack
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < L; j++) 
            out[i] += nzval[i * L + j] * vec[cols[i * L + j]];

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}