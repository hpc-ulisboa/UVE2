#include <stdio.h>
#include "Functions.h"

extern void core(void* A, void* B, void* C, void* D, void* E, void* F, void* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M);

int main() {
  size_t I = SIZE;
  size_t J = SIZE;
  size_t K = SIZE;
  size_t L = SIZE;
  size_t M = SIZE;

  DataType A[I*K];
  DataType B[K*J];
  DataType C[J*M];
  DataType D[M*L];
  DataType E[I*J];
  DataType F[J*L];
  DataType G[I*L];

  initArray2D(A, I, K);
  initArray2D(B, K, J);
  initArray2D(C, J, M);
  initArray2D(D, M, L);

  core(A, B, C, D, E, F, G, I, J, K, L, M);

  for (int i = 0; i < I; i++) {
    for (int j = 0; j < L; j++)
      printf( DataFormat("", "\n"), G[i*SIZE+j]);
  }

  return 0;
}
