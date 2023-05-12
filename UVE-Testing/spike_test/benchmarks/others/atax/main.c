#include <stdio.h>
#include "Commun.h"

extern void core(DataType A[SIZE][SIZE], DataType x[SIZE], DataType y[SIZE], DataType tmp[SIZE]);


int main() {
  DataType A[SIZE][SIZE];
  DataType x[SIZE];
  DataType y[SIZE];
  DataType tmp[SIZE];

  initArray2D(A);
  initArray(x);
  initArray(y);
  initArray(tmp);


  core(A, x, y, tmp);

  for (int i = 0; i < SIZE; i++) {
    printf( DataFormat("", "\n"), y[i]);
  }

  return 0;
}
