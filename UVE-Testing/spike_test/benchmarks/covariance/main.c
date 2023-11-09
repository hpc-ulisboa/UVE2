#include <stdio.h>
#include "Functions.h"

extern void core(int sizeM, int sizeN, DataType float_n, DataType *data, DataType *cov, DataType *mean);


int main() {
  int sizeM = SIZE;
  int sizeN = SIZE;

  DataType data[sizeN*sizeM];
  DataType cov[sizeM*sizeM];
  DataType mean[sizeM];

  initArray2D(data, sizeN, sizeM);
  initArray2D(cov, sizeM, sizeM);
  initZero(mean, sizeM);

  double n = sizeN;

  core(sizeM, sizeN, n, data, cov, mean);

  // cov
  for (int i = 0; i < sizeM*sizeM; i++) {
    printf( DataFormat("", "\n"), cov[i]);
  }

  return 0;
}
