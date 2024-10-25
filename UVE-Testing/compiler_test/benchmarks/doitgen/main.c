#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

extern void core(DataType *A, DataType *C4, DataType *sum, uint64_t R, uint64_t Q, uint64_t P);

int main() {
    uint64_t Q = 8;
    uint64_t R = 10;
    uint64_t P = 12;

    DataType *A = (DataType *)malloc(R * Q * P * sizeof(DataType));
    DataType *C4 = (DataType *)malloc(P * P * sizeof(DataType));
    DataType *sum = (DataType *)malloc(P * sizeof(DataType));

    for (int i = 0; i < R; i++)
        for (int j = 0; j < Q; j++)
            for (int k = 0; k < P; k++)
                A[i*Q*P+j*P+k] = (DataType)((i * j + k) % P) / P;
    for (int i = 0; i < P; i++)
        for (int j = 0; j < P; j++)
            C4[i*P+j] = (DataType)(i * j % P) / P;

    core(A, C4, sum, R, Q, P);

    for (int i = 0; i < R*Q*P; i++)
        printf(DataFormat("", "\n"), A[i]);

    //for (int i = 0; i < P; i++)
    //    printf(DataFormat("", "\n"), sum[i]);

    free(A);
    free(C4);
    free(sum);

    return 0;
}
