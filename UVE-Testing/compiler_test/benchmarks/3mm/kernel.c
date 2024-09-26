#include "Functions.h"

long int core_kernel(DataType *src1, DataType *src2, DataType *src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	int i,j,k;

    for (i = 0; i < sizeI; i++) {
        for (j = 0; j < sizeJ; j++){
        	src3[i*sizeJ+j] = 0;
        	for (k = 0; k < sizeK; k++)
            	src3[i*sizeJ+j] += src1[i*sizeK+k] * src2[k*sizeJ+j];
    	}
	}

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));

	return end - start;
}

void core(DataType* A, DataType* B, DataType* C, DataType* D, DataType* E, DataType* F, DataType* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M){

	long int a = core_kernel(A, B, E, I, J, K);
	a += core_kernel(C, D, F, J, L, M);
	a += core_kernel(E, F, G, I, L, J);

	printf("%d\n%ld\n", 0, a);
}

