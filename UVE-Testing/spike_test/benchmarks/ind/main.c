#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>

#define ran (TYPE)(((double) rand() / (RAND_MAX)) * (MAX-MIN) + MIN)

extern void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L);

void fillVal(DataType *nzval, int *colind, DataType *x, int32_t N, int32_t L){
	int j, cur_indx, i;
    srand48(8650341L);
	for (i = 0; i < N; i++){
        x[i] = ran;
        cur_indx = 0;
        for(j=0; j < L; j++){
            cur_indx = (DataType)(((double) rand() / (RAND_MAX)) * ((L-1) - cur_indx) + cur_indx);
            if(cur_indx < L){
		        nzval[i*L + j] = ran;
                colind[i*L +j] = cur_indx;
            }
        }
	}
}

int main() {
    int N = SIZE, L = SIZE;

    int colind[N*L];
    DataType nzval[N*L];
    DataType x[N];
    DataType y[N];
    int i;

    initZero(y, N);

    core(nzval, colind, x, y, N, L);

    for (int i = 0; i < N; ++i)
        printf(DataFormat("", "\n"), out[i]);

    return 0;
}
