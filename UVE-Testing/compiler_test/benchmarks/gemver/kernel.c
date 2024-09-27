#include "Functions.h"

long int start = 0, end = 0;

void core(DataType* A, DataType* u1, DataType* v1, DataType* u2, DataType* v2, DataType* w,
 DataType* x, DataType* y, DataType* z, DataType alpha, DataType beta, uint64_t sizeN) {

	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    int i,j;

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	A[i*sizeN + j] = A[i*sizeN + j] + u1[i] * v1[j] + u2[i] * v2[j];

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	x[i] = x[i] + beta * A[j*sizeN + i] * y[j];

    for (i = 0; i < sizeN; i++)
        x[i] = x[i] + z[i];

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	w[i] = w[i] + alpha * A[i*sizeN + j] * x[j];

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
