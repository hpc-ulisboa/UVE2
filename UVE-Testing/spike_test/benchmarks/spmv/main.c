#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

// Define the input range to operate over
#define MIN 0.
#define MAX 10.0

#define ran (DataType)(((double)rand() / (RAND_MAX)) * (MAX - MIN) + MIN)

extern void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L);

void fillVal(DataType *nzval, uint32_t *colind, DataType *x, int32_t N, int32_t L) {
    int j, cur_indx, i;
    srand48(8650341L);
    for (i = 0; i < N; i++) {
        x[i] = ran;
        cur_indx = 0;
        for (j = 0; j < L; j++) {
            cur_indx = (DataType)(((double)rand() / (RAND_MAX)) * ((L - 1) - cur_indx) + cur_indx);
            if (cur_indx < L) {
                nzval[i * L + j] = ran;
                colind[i * L + j] = cur_indx;
            }
        }
    }
}

int main() {
    int N = SIZE, L = SIZE;

    uint32_t colind[N * L];
    DataType nzval[N * L];
    DataType x[N];
    DataType y[N];

    initZero(y, N);
    fillVal(nzval, colind, x, N, L);

	printf("\nInput (cols):\n");
    for (int i = 0; i < N; ++i){
		for (int j = 0; j < L; ++j)
        	printf("%d ", colind[i*L+j]);
		printf("\n");
	}
	
	printf("\nInput (nzval):\n");
    for (int i = 0; i < N; ++i){
		for (int j = 0; j < L; ++j)
        	printf(DataFormat("", " "), nzval[i*L+j]);
		printf("\n");
	}

	printf("\nInput (vec):\n");
	for (int i = 0; i < N; ++i)
		printf(DataFormat("", "\n"), x[i]);

	printf("\nInput (out):\n");
    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), y[i]);

    core(nzval, colind, x, y, N, L);

	printf("\n\nResult (out):\n");
    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
