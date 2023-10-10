#include <stdio.h>
#include "Functions.h"


extern void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN);

int main()
{
  uint64_t N = SIZE;

  DataType A[N*N];
  DataType x_1[N];
  DataType x_2[N];
  DataType y_1[N];
  DataType y_2[N];

  initArray2D(A, N, N);
  initArray(x_1, N);
  initArray(x_2, N);
  initArray(y_1, N);
  initArray(y_2, N);

  core(A, x_1, x_2, y_1, y_2, N);

  for (int i = 0; i < N; i++)
    printf( DataFormat("", "\n"), x_1[i]);

  printf("\n");

  for (int i = 0; i < N; i++)
    printf( DataFormat("", "\n"), x_2[i]);

  return 0;
}
