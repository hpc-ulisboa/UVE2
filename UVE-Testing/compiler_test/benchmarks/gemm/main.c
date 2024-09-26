#include <stdio.h>
#include "Functions.h"

extern void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, int sizeI, int sizeJ, int sizeK);

int main() {
  int sizeI = SIZE;
  int sizeJ = SIZE;
  int sizeK = SIZE;

  DataType A[sizeI*sizeK];
  DataType B[sizeK*sizeJ];
  DataType C[sizeI*sizeJ];
  DataType alpha = 2;
  DataType beta = 3;

  initArray2D(A, sizeI, sizeK);
  initArray2D(B, sizeK, sizeJ);
  initArray2D(C, sizeI, sizeJ);
  
  core(alpha, beta, C, A, B, sizeI, sizeJ, sizeK);

  for (int i = 0; i < sizeI; i++) {
    for (int j = 0; j < sizeJ; j++) {
      printf( DataFormat("", "\n"), C[i*sizeI + j]);
    }
  }

  return 0;
}
