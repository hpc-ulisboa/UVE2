#include "Commun.h"



#ifdef RUN_UVE
  #error "Trying to run original UVE version. This contained instructions not present in spike"
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType A[SIZE][SIZE], DataType s[SIZE], DataType q[SIZE], DataType p[SIZE], DataType r[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    s[i] = 0.0;
  }

  for (int i = 0; i < SIZE; i++) {
    q[i] = 0.0;
    for (int j = 0; j < SIZE; j++) {
  	  s[j] = s[j] + r[i] * A[i][j];
	  }
    for (int j = 0; j < SIZE; j++) {
  	  q[i] = q[i] + A[i][j] * p[j];
	  }
  }
}
#endif // RUN_SIMPLE


#ifdef RUN_CLAVA
void core(DataType A[SIZE][SIZE], DataType s[SIZE], DataType q[SIZE], DataType p[SIZE], DataType r[SIZE]) {
  #pragma clava data uve : true
  for (int i = 0; i < SIZE; i++) {
    s[i] = 0.0;
  }

  for (int i = 0; i < SIZE; i++) {
    q[i] = 0.0;
    #pragma clava data uve : true
    for (int j = 0; j < SIZE; j++) {
  	  s[j] = s[j] + r[i] * A[i][j];
	  }
    for (int j = 0; j < SIZE; j++) {
  	  q[i] = q[i] + A[i][j] * p[j];
	  }
  }
}
#endif // RUN_CLAVA


#ifdef RUN_BLANK
void core(DataType A[SIZE][SIZE], DataType s[SIZE], DataType q[SIZE], DataType p[SIZE], DataType r[SIZE])
{
}
#endif // RUN_BLANK
