#include "Commun.h"



#ifdef RUN_UVE
  #error "Trying to run original UVE version. This contained instructions not present in spike"
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType A[SIZE][SIZE], DataType x[SIZE], DataType y[SIZE], DataType tmp[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    y[i] = 0;
  }

  for (int i = 0; i < SIZE; i++) {
    tmp[i] = 0.0;
    for (int j = 0; j < SIZE; j++) {
      tmp[i] = tmp[i] + A[i][j] * x[j];
    }
    for (int j = 0; j < SIZE; j++) {
      y[j] = y[j] + A[i][j] * tmp[i];
    }
  }
}
#endif // RUN_SIMPLE


#ifdef RUN_CLAVA
void core(DataType A[SIZE][SIZE], DataType x[SIZE], DataType y[SIZE], DataType tmp[SIZE]) {
  #pragma clava data uve : true
  for (int i = 0; i < SIZE; i++) {
    y[i] = 0.0;
  }

  for (int i = 0; i < SIZE; i++) {
    tmp[i] = 0.0;
    for (int j = 0; j < SIZE; j++) {
      tmp[i] = tmp[i] + A[i][j] * x[j];
    }
    #pragma clava data uve : true
    for (int j = 0; j < SIZE; j++) {
      y[j] = y[j] + A[i][j] * tmp[i];
    }
  }
}
#endif // RUN_CLAVA


#ifdef RUN_BLANK
void core(DataType A[SIZE][SIZE], DataType x[SIZE], DataType y[SIZE], DataType tmp[SIZE])
{
}
#endif // RUN_BLANK
