#include <stdio.h>
#include "Functions.h"

//extern void core(DataType x[SIZE], DataType y[SIZE], DataType A);
//extern void core();
extern void core(DataType dest[SIZE], DataType src[SIZE]);

int main() {
  /*DataType src[SIZE];
  DataType dest[SIZE];
  DataType value = 3;

  initArray(src, SIZE);
  initZero(dest, SIZE);

  core(dest, src, value);

  for (int i = 0; i < SIZE; i++)
    printf( DataFormat("", "\n"), dest[i]);
  */
  //core();

  DataType src[SIZE];
  signed char dest[SIZE];

  initArray(src, SIZE);
  initZero(dest, SIZE);

  // print SIZE
  printf("SIZE: %d\n", SIZE);

  printf("Source array:\n");

  for (int i = 0; i < SIZE; i++)
    printf( "%i ", src[i]);

  core(dest, src);

  printf("\nDestination array:\n");

  for (int i = 0; i < SIZE; i++)
    printf( "%d ", (int)dest[i]);

  return 0;
}
