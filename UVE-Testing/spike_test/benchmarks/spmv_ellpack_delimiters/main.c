#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef D_TYPE
#error *Benchmark only available for double precision*
#endif

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

extern void core(void *val, void *cols, void *rowDelimiters, void *vec, void *out, uint64_t N, uint64_t K);

static void initVals(uint32_t *colind, DataType *nzval, uint32_t *rowDelimiters, DataType *x, int M, int K) {
    const uint32_t cols_data[] = colsSpmvEllpack;
    const DataType nzval_data[] = nzvalSpmvEllpack;
    const uint32_t row_data[] = rowDelimitersSpmvEllpack;
    const DataType x_data[] = vecSpmvEllpack;
    
    for (int i = 0; i < M * K; i++) {
        colind[i] = cols_data[i];
        nzval[i] = nzval_data[i];
    }

    for (int i = 0; i < M; i++) {
        x[i] = x_data[i];
        rowDelimiters[i] = row_data[i];
    }
}

int main() {
    int M = 494, K = 10; // rows of Sparse Matrix, max non-zero elements per row

    uint32_t colind[M * K];
    DataType nzval[M * K];
    uint32_t rowDelimiters[M];
    DataType x[M];
    DataType y[M];

    initVals(colind, nzval, rowDelimiters, x, M, K);
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
    core(nzval, colind, rowDelimiters, x, y, M, K);

	//printf("\n\nResult (out):\n");
    for (int i = 0; i < M; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
