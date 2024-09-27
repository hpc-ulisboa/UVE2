#include "Functions.h"

long int start = 0, end = 0;

void core(DataType* A, DataType* x_1, DataType* x_2, DataType* y_1, DataType* y_2, uint64_t sizeN) {
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_1[i] = x_1[i] + A[i*sizeN+j] * y_1[j];

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_2[i] = x_2[i] + A[j*sizeN+i] * y_2[j];

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}