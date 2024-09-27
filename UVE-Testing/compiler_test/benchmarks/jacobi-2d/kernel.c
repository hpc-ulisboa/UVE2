#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *A, DataType *B) {
  asm volatile ("rdinstret %[s] \n":[s] "=&r"(start)); 

  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      B[i*SIZE+j] = 0.2  * (A[i*SIZE+j] + A[i*SIZE+j-1] + A[i*SIZE+j+1] + A[(i+1)*SIZE+j] + A[(i-1)*SIZE+j]);
    }
  }
  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      A[i*SIZE+j] = 0.2  * (B[i*SIZE+j] + B[i*SIZE+j-1] + B[i*SIZE+j+1] + B[(i+1)*SIZE+j] + B[(i-1)*SIZE+j]);
    }
  }

  asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
}