#include <stdio.h>
#include "Functions.h"

extern void core(void* src1, void* src2, void* src3, unsigned long int sizeI, unsigned long int sizeJ, unsigned long int sizeK);


int main() {
  DataType src1[SIZE*SIZE];
  DataType src2[SIZE*SIZE];
  DataType src3[SIZE*SIZE];

  initArray2D(src1);
  initArray2D(src2);
  initArray2D(src3);

  core(src1, src2, src3, SIZE, SIZE, SIZE);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++)
      printf( DataFormat("", "\n"), src3[i*SIZE+j]);
  }

  return 0;
}
