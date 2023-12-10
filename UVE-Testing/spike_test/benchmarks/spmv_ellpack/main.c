#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

#define ran (DataType)(((double)rand() / (RAND_MAX)) * (MAX - MIN) + MIN)

extern void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t M, int32_t K);

void fillVal(DataType *nzval, uint32_t *colind, DataType *x, int32_t M, int32_t K) {
    int j, cur_indx, i;
    srand48(8650341L);
    for (i = 0; i < M; i++) {
        x[i] = ran;
        cur_indx = 0;
        for (j = 0; j < K; j++) {
            cur_indx = (DataType)(((double)rand() / (RAND_MAX)) * ((K - 1) - cur_indx) + cur_indx);
            printf("cur_indx = %d\n", cur_indx);
            if (cur_indx < K) {
                printf("Under K!\n");
                nzval[i * K + j] = ran;
                colind[i * K + j] = cur_indx;
            }
        }
    }
}

int main() {
    //int M = 494, K = 10; // rows of Sparse Matrix, max non-zero elements per row
    int M = 12, K = 3;

    uint32_t colind[M * K];
    DataType nzval[M * K];
    DataType x[M];
    DataType y[M];

    initZero(y, M);
    fillVal(nzval, colind, x, M, K);

	printf("\nInput (cols):\n");
    for (int i = 0; i < M; ++i){
		for (int j = 0; j < K; ++j)
        	printf("%d ", colind[i*K+j]);
		printf("\n");
	}
	
	printf("\nInput (nzval):\n");
    for (int i = 0; i < M; ++i){
		for (int j = 0; j < K; ++j)
        	printf(DataFormat("", " "), nzval[i*K+j]);
		printf("\n");
	}

	printf("\nInput (vec):\n");
	for (int i = 0; i < M; ++i)
		printf(DataFormat("", "\n"), x[i]);

	printf("\nInput (out):\n");
    for (int i = 0; i < M; ++i)
        printf(DataFormat("", "\n"), y[i]);

    core(nzval, colind, x, y, M, K);

	printf("\n\nResult (out):\n");
    for (int i = 0; i < M; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
