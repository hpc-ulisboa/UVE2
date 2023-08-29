#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src1, DataType *src2, DataType ct);

int main() {
  DataType A[SIZE*SIZE];
  DataType B[SIZE*SIZE];
  DataType ct = 1;

  initArray2D(A);
  initArray2D(B);

  core(A, B, ct);

  for (int i = 0; i < SIZE; ++i){
      for (int j = 0; j < SIZE; ++j)
        printf( DataFormat("", " "), A[i*SIZE+j]);
      printf("\n");
  }

  return 0;
}
