#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W);

int main(){
	int N = SIZE; int N2 = N*N;
	DataType src[N2];
	DataType dst[N2];
	DataType filter[3*3] = {1, 0, -1, 2, 0, -2, 1, 0, -1};//, 1, 0, -1, 2, 0, -2, 1, 0, -1, 1, 0, -1, 2, 0, -2, 1, 0, -1};

	initArray(src, N2);
	initZero(dst, N2);

	core(src, dst, filter, N, N);

	for (int i = 0; i < N2; ++i)
		printf(DataFormat("", "\n"), dst[i]);

	return 0;
}
