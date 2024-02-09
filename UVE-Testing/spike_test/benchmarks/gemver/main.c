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

  initArray2D(A, N, N);
  // smaller values not to overflow
  initArray(u1, N);
  initArray(v1, N);
  initArray(u2, N);
  initArray(v2, N);
  initArray(w, N);
  initArray(x, N);
  initArray(y, N);
  initArray(z, N);

  #if defined(F_TYPE)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      A[i*N+j] = A[i*N+j] / 100;
    u1[i] = u1[i] / 100;
    v1[i] = v1[i] / 100;
    u2[i] = u2[i] / 100;
    v2[i] = v2[i] / 100;
    w[i] = w[i] / 100;
    x[i] = x[i] / 100;
    y[i] = y[i] / 100;
    z[i] = z[i] / 100;
  }
  #endif
  #if defined (D_TYPE) || defined (F_TYPE)
  DataType alpha = 1.5;
  DataType beta = 1.2;
  #else
  DataType alpha = 2;
  DataType beta = 3;
  #endif

  core(A, u1, v1, u2, v2, w, x, y, z, alpha, beta, N);

  for (int i = 0; i < N; i++) {
    printf( DataFormat("", "\n"), w[i]);
  }

  return 0;
}
