#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src1, DataType *src2);

int main() {
  DataType A[SIZE*SIZE];
  DataType B[SIZE*SIZE];

  initArray2D(A);
  initArray2D(B);

  core(A, B);

  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j)
      printf( DataFormat("", "\n"), A[i*SIZE+j]);

  return 0;
}
