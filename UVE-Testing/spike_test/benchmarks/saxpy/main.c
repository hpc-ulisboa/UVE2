#include <stdio.h>
#include "Functions.h"

extern void core(DataType dest[SIZE], DataType src[SIZE], DataType value);

int main()
{
  DataType src[SIZE];
  DataType dest[SIZE];
  DataType value = 3;

  initArray(src);
  initZero(dest);

  core(dest, src, value);

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), dest[i]);
}