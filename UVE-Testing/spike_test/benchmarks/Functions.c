#include "Functions.h"

void initArray(DataType *array) {
  const DataType values[] = DataSet;

  for (int i = 0; i < SIZE; ++i) {
    array[i] = values[i];
  }
}
void initArray2D(DataType *array) {
  const DataType values[] = DataSet;

  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      array[i*SIZE+j] = values[i*SIZE+j];
      printf("%d ", array[i*SIZE+j]);
    }
  }
}

void initZero(DataType *array) {
  for (int i = 0; i < SIZE; ++i) {
    array[i] = 0;
  }
}