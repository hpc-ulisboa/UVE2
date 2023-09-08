#include <stdio.h>
#include "Functions.h"


extern void core(DataType dest[SIZE], DataType src1[SIZE], DataType src2[SIZE]);

int main()
{
  DataType src1[SIZE];
  DataType src2[SIZE];
  DataType dest[SIZE];

  DataType m = 10;

  initArray(src1, SIZE);
  initConstant(src2, SIZE, m);
  initArray(dest, SIZE);

  core(dest, src1, src2);

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), src1[i]);

  printf("\n\n");

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), src2[i]);

  printf("\n\n");

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), dest[i]);

  return 0;
}
