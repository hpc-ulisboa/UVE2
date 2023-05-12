#include <stdio.h>
#include "Functions.h"


extern void core(DataType A[SIZE][SIZE], DataType B[SIZE][SIZE], DataType ct);

int main()
{
  DataType A[SIZE][SIZE];
  DataType B[SIZE][SIZE];
  DataType ct = 0.33333;

  initArray2D(A);
  initArray2D(B);

  core(A, B, ct);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf( DataFormat("", "\n"), A[i][j]);
      printf( DataFormat("", "\n"), B[i][j]);
    }
  }
}
