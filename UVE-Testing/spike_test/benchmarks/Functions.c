#include "Functions.h"

void initArray(DataType array[SIZE]) {
  const DataType values[] = DataSet;

  for (int i = 0; i < SIZE; i++) {
    array[i] = values[i];
  }
}
void initArray2D(DataType array[SIZE][SIZE]) {
  const DataType values[] = DataSet;

  for (int j = 0; j < SIZE; j++) {
    for (int i = 0; i < SIZE; i++) {
      array[j][i] = values[j * SIZE + i];
    }
  }
}

void initZero(DataType array[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    array[i] = 0;
  }
}