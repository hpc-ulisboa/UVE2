#include <stdio.h>
#include "Functions.h"


extern void core(void *src1, void *src2, void *src3, void *src4, void *src5, uint64_t sizeN);

int main()
{
  uint64_t N = SIZE;

  DataType src1[N*N];
  DataType src2[N];
  DataType src3[N];
  DataType src4[N];
  DataType src5[N];

  initArray2D(src1, N, N);
  initArray(src2, N);
  initArray(src3, N);
  initArray(src4, N);
  initArray(src5, N);

  core(src1, src2, src3, src4, src5, N);

  for (int i = 0; i < N*N; i++)
    printf( DataFormat("", "\n"), src2[i]);

  printf("\n");

  for (int i = 0; i < N*N; i++)
    printf( DataFormat("", "\n"), src4[i]);

  return 0;
}
