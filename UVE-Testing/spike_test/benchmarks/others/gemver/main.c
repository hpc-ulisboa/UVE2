#include <stdio.h>
#include "Commun.h"

extern void core(DataType A[SIZE][SIZE], DataType k1[SIZE], DataType k2[SIZE],
DataType v1[SIZE], DataType v2[SIZE], DataType w[SIZE], DataType x[SIZE],
DataType y[SIZE], DataType z[SIZE], DataType alpha, DataType beta);


int main() {
  DataType A[SIZE][SIZE];
  DataType k1[SIZE];
  DataType k2[SIZE];
  DataType v1[SIZE];
  DataType v2[SIZE];
  DataType w[SIZE];
  DataType x[SIZE];
  DataType y[SIZE];
  DataType z[SIZE];
  DataType alpha = 2;
  DataType beta = 3;

  initArray2D(A);
  initArray(k1);
  initArray(k2);
  initArray(v1);
  initArray(v2);
  initArray(w);
  initArray(x);
  initArray(y);
  initArray(z);


  core(A, k1, k2, v1, v2, w, x, y, z, alpha, beta);


  for (int i = 0; i < SIZE; i++) {
    printf( DataFormat("", "\n"), w[i]);
  }

  return 0;
}
