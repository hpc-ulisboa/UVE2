#include <stdio.h>
#include "Functions.h"


extern void core(DataType dest[SIZE], DataType src[SIZE]);

int main()
{
  DataType src[SIZE];
  DataType dest[SIZE];

  initArray(src);
  initZero(dest);

  core(dest, src);

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), dest[i]);
}
