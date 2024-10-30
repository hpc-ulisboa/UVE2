// https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1/tree/master/linear-algebra/kernels/3mm
#include "Functions.h"

long int core_kernel(DataType *src1, DataType *src2, DataType *src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	int i,j,k;

    for (i = 0; i < sizeI; i++)
        for (j = 0; j < sizeJ; j++)
        	for (k = 0; k < sizeK; k++)
            	src3[i*sizeJ+j] += src1[i*sizeK+k] * src2[k*sizeJ+j];

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));

	return end - start;
}
