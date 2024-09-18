#include <stdio.h>
#include "Functions.h"

//extern void core(DataType x[SIZE], DataType y[SIZE], DataType A);
//extern void core();
extern void core(DataType src[SIZE]);

int main() {
  DataType src[SIZE];

  initArray(src, SIZE);

  core(src);

  return 0;
}
