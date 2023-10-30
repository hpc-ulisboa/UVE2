#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src, DataType *dst, DataType *filter);

int main(){
	DataType src[SIZE*SIZE];
	DataType dst[SIZE*SIZE];
	DataType filter[3*3] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	initArray2D(src, SIZE, SIZE);
	initZero(dst, SIZE*SIZE);

	core(src, dst, filter);

	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
			printf(DataFormat("", "\n"), dst[i*SIZE+j]);

	return 0;
}
