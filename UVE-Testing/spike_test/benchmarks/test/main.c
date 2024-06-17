#include <stdio.h>
#include "Functions.h"

extern void core(DataType x[SIZE], DataType y[SIZE], DataType A);

int main()
{
  DataType src[SIZE];
  DataType dest[SIZE];
  DataType value = 3;

  initArray(src, SIZE);
  initZero(dest, SIZE);

  core(dest, src, value);

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), dest[i]);

  return 0;
}