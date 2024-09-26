#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src1, DataType *src2);

int main() {
  DataType A[SIZE*SIZE];
  DataType B[SIZE*SIZE];

  initArray2D(A, SIZE, SIZE);
  initArray2D(B, SIZE, SIZE);

  core(A, B);

  if (SIZE*SIZE <= 2500)
    for (int i = 0; i < SIZE*SIZE; ++i)
        printf( DataFormat("", "\n"), A[i]);
  else
    for (int i = 0; i < SIZE*SIZE; i+=SIZE/20)
        printf( DataFormat("", "\n"), A[i]);
  

  return 0;
}
