// https://github.com/breagen/MachSuite/tree/master/spmv/ellpack
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));
#ifdef RVV
    DataType Si;
    int i, j;
    for (i = 0; i < N; i++) {
        DataType sum = out[i];
        for (j = 0; j < L; j++) {
            Si = nzval[j + i * L] * vec[cols[j + i * L]];
            sum += Si;
        }
        out[i] = sum;
    }
#else
    for (int i = 0; i < N; i++)
        for (int j = 0; j < L; j++)
            out[i] += nzval[i * L + j] * vec[cols[i * L + j]];
#endif
    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}