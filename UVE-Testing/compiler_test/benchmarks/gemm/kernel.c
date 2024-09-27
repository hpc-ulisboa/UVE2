#include "Functions.h"

long int start = 0, end = 0;

void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
  
  // C (sizeI x sizeJ) * beta  += alpha * A (sizeI x sizeK) * B (sizeK x sizeJ)

  asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

  int i, j, k;
  
  for (i = 0; i < sizeI; i++) {
    for (j = 0; j < sizeJ; j++)
	    C[i * sizeI + j] *= beta;
    for (k = 0; k < sizeK; k++) {
       for (j = 0; j < sizeJ; j++)
	      C[i * sizeI + j] += alpha * A[i * sizeI + k] * B[k*sizeK + j];
    }
  }

  asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
}

