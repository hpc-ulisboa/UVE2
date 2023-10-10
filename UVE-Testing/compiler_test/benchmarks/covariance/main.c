#include <stdio.h>
#include "Functions.h"

extern void core(void *src1, void *src2, void *src3);


int main() {
  DataType src_1[SIZE*SIZE];
  DataType src_2[SIZE*SIZE];
  DataType src_3[SIZE];

  initArray2D(src_1, SIZE, SIZE);
  initArray2D(src_2, SIZE, SIZE);
  initZero(src_3, SIZE);


  core(src_1, src_2, src_3);

  // cov
  for (int i = 0; i < SIZE; i++) {
    printf( DataFormat("", "\n"), src_2[i]);
  }

  return 0;
}
