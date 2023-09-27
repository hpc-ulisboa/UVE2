#include <stdio.h>
#include "Functions.h"

extern void core(void *A, void *u1, void *v1, void *u2, void *v2, void *w, void *x, void *y, void *z, DataType a, DataType b, uint64_t N);


int main() {
  uint64_t N = SIZE;
  DataType A[N*N];
  DataType u1[N];
  DataType v1[N];
  DataType u2[N];
  DataType v2[N];
  DataType w[N];
  DataType x[N];
  DataType y[N];
  DataType z[N];
  DataType alpha = 1.5;
  DataType beta = 1.2;

  initArray2D(A, N, N);
  initArray(u1, N);
  initArray(v1, N);
  initArray(u2, N);
  initArray(v2, N);
  initArray(w, N);
  initArray(x, N);
  initArray(y, N);
  initArray(z, N);


  core(A, u1, v1, u2, v2, w, x, y, z, alpha, beta, N);


  for (int i = 0; i < N; i++) {
    printf( DataFormat("", "\n"), w[i]);
  }

  return 0;
}
