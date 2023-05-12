#include <stdio.h>
#include "Commun.h"

extern void core(DataType A[SIZE][SIZE], DataType B[SIZE][SIZE], DataType C[SIZE][SIZE], DataType alpha, DataType beta);

int main() {
  DataType A[SIZE][SIZE];
  DataType B[SIZE][SIZE];
  DataType C[SIZE][SIZE];
  DataType alpha = 2;
  DataType beta = 3;

  initArray2D(A);
  initArray2D(B);
  initArray2D(C);

  core(A, B, C, alpha, beta);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf( DataFormat("", "\n"), C[i][j]);
    }
  }

  return 0;
}
