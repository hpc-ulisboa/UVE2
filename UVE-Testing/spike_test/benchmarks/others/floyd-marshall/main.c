#include <stdio.h>
#include "Commun.h"


extern void core(DataType path[SIZE][SIZE]);

int main()
{
	DataType path[SIZE][SIZE];

  initArray2D(path);

  core(path);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf( DataFormat("", "\n"), path[i][j]);
    }
  }

  return 0;
}
