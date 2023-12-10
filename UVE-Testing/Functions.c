#include "Functions.h"

void initArray(DataType *array, int N) {
  const DataType values[] = DataSet;

  for (int i = 0; i < N; ++i) {
    array[i] = values[i];
  }
}

void initArray2D(DataType *array, int N, int M) {
  const DataType values[] = DataSet;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      array[i*N+j] = values[i*M+j];
    }
  }
}

void initZero(DataType *array, int N) {
  for (int i = 0; i < N; ++i) {
    array[i] = 0;
  }
}

void initConstant(DataType *array, int N, DataType m) {
  for (int i = 0; i < N; ++i) {
    array[i] = m;
  }
}