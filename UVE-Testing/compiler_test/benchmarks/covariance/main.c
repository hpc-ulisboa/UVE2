#include <stdio.h>
#include "Functions.h"

extern void core(int sizeM, int sizeN, DataType datat_n, DataType *data, DataType *cov, DataType *mean);


int main() {
  int sizeM = SIZE;
  int sizeN = SIZE;

  DataType data[sizeN*sizeM];
  DataType cov[sizeM*sizeM];
  DataType mean[sizeM];

  initArray2D(data, sizeN, sizeM);
  initArray2D(cov, sizeM, sizeM);
  initZero(mean, sizeM);

  DataType n = sizeN;

  core(sizeM, sizeN, n, data, cov, mean);

  // cov
  //printf("\ncov:\n");
  for (int i = 0; i < sizeM; i++) {
    for (int j = 0; j < sizeM; j++)
      printf( DataFormat("", "\n"), cov[i*sizeM + j]);
    //printf("\n");
  }

  /*print mean
  for (int i = 0; i < sizeM; i++) {
    printf( DataFormat("", "\n"), mean[i]);
  }*/

  return 0;
}
