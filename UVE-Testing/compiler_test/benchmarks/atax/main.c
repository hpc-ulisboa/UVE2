#include "Functions.h"
#include <stdio.h>

extern void core(DataType *A, DataType *x, DataType *y, DataType *tmp,  uint64_t N, uint64_t M);

int main() {
    uint64_t M = SIZE;
    uint64_t N = SIZE+SIZE/5;

    DataType A[M * N];
    DataType x[N];
    DataType y[N];
    DataType tmp[M];

    DataType fn;
    fn = (DataType)N;

    for (int i = 0; i < N; i++){
        x[i] = 1 + (i / fn);
		y[i] = 0;
	}
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            A[i*N+j] = (DataType)((i + j) % N) / (5 * M);

    core(A, x, y, tmp, N, M);

    for (int i = 0; i < N; i++)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
