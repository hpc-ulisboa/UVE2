#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

extern void core(DataType *nzval, uint32_t *cols, DataType *vec, DataType *out, int32_t M, int32_t K);

static void initVals(uint32_t *colind, DataType *nzval, DataType *x, int M, int K) {
    const uint32_t cols_data[] = colsSpmvEllpack;
    const DataType nzval_data[] = nzvalSpmvEllpack;
    const DataType x_data[] = vecSpmvEllpack;
    
    for (int i = 0; i < M * K; i++) {
        colind[i] = cols_data[i];
        nzval[i] = nzval_data[i];
    }

    for (int i = 0; i < M; i++)
        x[i] = x_data[i];
}

int main() {
    int M = 494, K = 10; // rows of Sparse Matrix, max non-zero elements per row

    uint32_t colind[M * K];
    DataType nzval[M * K];
    DataType x[M];
    DataType y[M];

    initVals(colind, nzval, x, M, K);
    initZero(y, M);

/*	printf("\nInput (cols):\n");
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
*/
    core(nzval, colind, x, y, M, K);

	//printf("\n\nResult (out):\n");
    for (int i = 0; i < M; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
