#include "Functions.h"
#include <stdio.h>

extern void core(int M, int N, DataType float_n, DataType *data, DataType *corr, DataType *mean, DataType *stddev, DataType eps);

int main() {
    int M = SIZE;
    int N = SIZE;

    DataType data[N * M];
    DataType corr[M * M];
    DataType mean[M];
    DataType stddev[M];
    DataType eps = 0.1;

    initArray2D(data, N, M);
    initZero(corr, M * M);
    initZero(mean, M);
    initZero(stddev, M);

    DataType float_n = N;

    core(M, N, float_n, data, corr, mean, stddev, eps);

    for (int i = 0; i < M; i++) {
        //printf(DataFormat("", "\n"), stddev[i]);
        for (int j = 0; j < M; j++)
             printf(DataFormat("", "\n"), corr[i * M + j]);
        //  printf("\n");
    }

    /*print mean
    for (int i = 0; i < M; i++) {
      printf( DataFormat("", "\n"), mean[i]);
    }*/

    /*print data
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            printf(DataFormat("", "\n"), data[i * M + j]);
    }*/

    return 0;
}
