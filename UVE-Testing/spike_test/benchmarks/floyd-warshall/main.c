#include <stdio.h>
#include "Functions.h"


extern void core(DataType *path);

int main()
{
	DataType path[SIZE*SIZE];

  initArray2D(path, SIZE, SIZE);

  core(path);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf( DataFormat("", "\n"), path[i*SIZE+j]);
    }
  }

  return 0;
}
