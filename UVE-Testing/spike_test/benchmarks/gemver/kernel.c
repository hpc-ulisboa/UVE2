#include "Functions.h"


#ifdef RUN_UVE
  #error "Trying to run original UVE version. This contained instructions not present in spike"
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType A[SIZE][SIZE], DataType k1[SIZE], DataType k2[SIZE],
DataType v1[SIZE], DataType v2[SIZE], DataType w[SIZE], DataType x[SIZE],
DataType y[SIZE], DataType z[SIZE], DataType alpha, DataType beta) {
  #pragma clava data uve : true
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      A[i][j] = A[i][j] + k1[i] * v1[j] + k2[i] * v2[j];

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      x[i] = x[i] + beta * A[j][i] * y[j];

  #pragma clava data uve : true
  for (int i = 0; i < SIZE; i++)
    x[i] = x[i] + z[i];

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      w[i] = w[i] +  alpha * A[i][j] * x[j];
}
#endif // RUN_SIMPLE