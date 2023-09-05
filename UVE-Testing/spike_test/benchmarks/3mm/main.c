#include <stdio.h>
#include "Functions.h"

extern void core(DataType *src1, DataType *src2, DataType *src3);


int main() {
  DataType src1[SIZE*SIZE];
  DataType src2[SIZE*SIZE];
  DataType src3[SIZE*SIZE];

  initArray2D(src1);
  initArray2D(src2);
  initArray2D(src3);

  core(src1, src2, src3);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++)
      printf( DataFormat("", "\n"), src3[i*SIZE+j]);
  }

  return 0;
}
