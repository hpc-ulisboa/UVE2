#include <stdio.h>
#include "Functions.h"

extern void core(void *A, void *u1, void *v1, void *u2, void *v2, void *w, void *x, void *y, void *z, float a, float b);


int main() {
  DataType src_1[SIZE*SIZE];
  DataType src_2[SIZE];
  DataType src_3[SIZE];
  DataType src_4[SIZE];
  DataType src_5[SIZE];
  DataType src_6[SIZE];
  DataType src_7[SIZE];
  DataType src_8[SIZE];
  DataType src_9[SIZE];
  DataType alpha = 1.5;
  DataType beta = 1.2;

  initArray2D(src_1);
  initArray(src_2);
  initArray(src_3);
  initArray(src_4);
  initArray(src_5);
  initArray(src_6);
  initArray(src_7);
  initArray(src_8);
  initArray(src_9);


  core(src_1, src_2, src_3, src_4, src_5, src_6, src_7, src_8, src_9, alpha, beta);


  for (int i = 0; i < SIZE; i++) {
    printf( DataFormat("", "\n"), src_6[i]);
  }

  return 0;
}
