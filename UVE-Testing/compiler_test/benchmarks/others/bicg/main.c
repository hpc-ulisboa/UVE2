#include <stdio.h>
#include "Commun.h"

extern void core(DataType A[SIZE][SIZE], DataType s[SIZE], DataType q[SIZE], DataType p[SIZE], DataType r[SIZE]);


int main() {
  DataType A[SIZE][SIZE];
  DataType s[SIZE];
  DataType q[SIZE];
  DataType p[SIZE];
  DataType r[SIZE];

  initArray2D(A);
  initArray(s);
  initArray(q);
  initArray(p);
  initArray(r);


  core(A, s, q, p, r);

  for (int i = 0; i < SIZE; i++) {
    printf( DataFormat("", "\n"), s[i]);
    printf( DataFormat("", "\n"), q[i]);
  }

  return 0;
}
