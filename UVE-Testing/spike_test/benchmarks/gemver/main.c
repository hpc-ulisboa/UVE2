#include <stdio.h>
#include "Functions.h"

extern void core(void *A, void *u1, void *v1, void *u2, void *v2, void *w, void *x, void *y, void *z, DataType a, DataType b, uint64_t N);


int main() {
  uint64_t N = SIZE;
  DataType src_1[N*N];
  DataType src_2[N];
  DataType src_3[N];
  DataType src_4[N];
  DataType src_5[N];
  DataType src_6[N];
  DataType src_7[N];
  DataType src_8[N];
  DataType src_9[N];
  DataType alpha = 1.5;
  DataType beta = 1.2;

  initArray2D(src_1, N, N);
  initArray(src_2, N);
  initArray(src_3, N);
  initArray(src_4, N);
  initArray(src_5, N);
  initArray(src_6, N);
  initArray(src_7, N);
  initArray(src_8, N);
  initArray(src_9, N);


  core(src_1, src_2, src_3, src_4, src_5, src_6, src_7, src_8, src_9, alpha, beta, N);


  for (int i = 0; i < N; i++) {
    printf( DataFormat("", "\n"), src_6[i]);
  }

  return 0;
}
