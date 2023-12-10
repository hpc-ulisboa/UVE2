#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

#define ran (DataType)(((double)rand() / (RAND_MAX)) * (MAX - MIN) + MIN)

extern void core(void *val, void *cols, void *rowDelimiters, void *vec, void *out, uint64_t N);

void fillVal(DataType *A, int32_t N) {
    int j;
    srand48(8650341L);
    for (j = 0; j < N; j++) {
        A[j] = ran;
    }
}

void initMat(uint32_t *colind, uint32_t *rowDelimiters, int NNZ, int N) {
    uint32_t nnzAssigned = 0;
    float prob = (float)NNZ / ((float)N * (float)N);

    srand48(8675307L);

    int fillRemaining = 0;
    int i, j;

    for (i = 0; i < N; i++) {
        rowDelimiters[i] = nnzAssigned;
        for (j = 0; j < N; j++) {
            int numEntriesLeft = (N * N) - ((i * N) + j);
            int needToAssign = NNZ - nnzAssigned;
            if (numEntriesLeft <= needToAssign) {
                fillRemaining = 1;
            }
            if ((nnzAssigned < NNZ && drand48() <= prob) || fillRemaining == 1) {
                colind[nnzAssigned] = (uint32_t)j;
                nnzAssigned++;
            }
        }
    }
    rowDelimiters[N] = (uint32_t)NNZ;
}

int main() {
    int N = 494; // 494 or SIZE
    int NNZ = 1666; // 1666 or SIZE*SIZE

    DataType nzval[NNZ];
    uint32_t colind[NNZ];
    uint32_t rowptr[N + 1];
    DataType x[N];
    DataType y[N];

    initZero(y, N);
    initMat(colind, rowptr, NNZ, N);
    fillVal(nzval, NNZ);
    fillVal(x, N);

    printf("\nInput (val):\n");
    for (int i = 0; i < NNZ; ++i)
        printf(DataFormat("", " "), nzval[i]);

    printf("\nInput (cols):\n");
    for (int i = 0; i < NNZ; ++i)
        printf("%d ", colind[i]);
    
    printf("\nInput (rowDelimiters):\n");
    for (int i = 1; i < N+1; ++i)
        printf("%u\n", rowptr[i]);

    printf("\nInput (vec):\n");
    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), x[i]);
    
    core(nzval, colind, rowptr+1, x, y, N);

    printf("\n\nResult (y):\n");
    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
