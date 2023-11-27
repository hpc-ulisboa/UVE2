#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

#define ran (DataType)(((double)rand() / (RAND_MAX)) * (MAX - MIN) + MIN)

extern void core(void *nzval, uint32_t *colind, uint32_t *rowptr, void *x, void *y, int32_t N);

void fillVal(DataType *A, int32_t N) {
    int j;
    srand48(8650341L);
    for (j = 0; j < N; j++) {
        A[j] = ran;
    }
}

void initMat(int *colind, int *rowDelimiters, int NNZ, int N) {
    int nnzAssigned = 0;
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
                colind[nnzAssigned] = j;
                nnzAssigned++;
            }
        }
    }
    rowDelimiters[N] = NNZ;
}

int main() {
    int N = SIZE;
    int NNZ = SIZE * SIZE;

    DataType nzval[NNZ];
    uint32_t colind[NNZ];
    uint32_t rowptr[N + 1];
    DataType x[N];
    DataType y[N];

    initZero(y, N);
    fillVal(nzval, NNZ);
    fillVal(x, N);

    /*
        printf("\nInput (colind):\n");
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j)
                printf("%d ", colind[i*N+j]);
            printf("\n");
        }

        printf("\nInput (nzval):\n");
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j)
                printf(DataFormat("", " "), nzval[i*N+j]);
            printf("\n");
        }

        printf("\nInput (x):\n");
        for (int i = 0; i < N; ++i)
            printf(DataFormat("", "\n"), x[i]);

        printf("\nInput (y):\n");
        for (int i = 0; i < N; ++i)
            printf(DataFormat("", "\n"), y[i]);
    */
    core(nzval, colind, rowptr, x, y, N);

    // printf("\n\nResult (y):\n");
    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
