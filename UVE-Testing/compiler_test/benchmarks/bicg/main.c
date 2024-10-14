#include "Functions.h"
#include <stdio.h>

void core(DataType *A, DataType *s, DataType *q, DataType *p, DataType *r, uint64_t N, uint64_t M);

int main() {
    uint64_t M = SIZE;
    uint64_t N = SIZE + SIZE / 10;

    DataType A[N * M];
    DataType r[N];
    DataType p[M];
    DataType s[M];
    DataType q[N];

    initZero(s, M);
    initZero(q, N);

    for (int i = 0; i < M; i++)
        p[i] = (DataType)(i % M) / M;
    for (int i = 0; i < N; i++) {
        r[i] = (DataType)(i % N) / N;
        for (int j = 0; j < M; j++)
            A[i * M + j] = (DataType)(i * (j + 1) % N) / N;
    }

    core(A, s, q, p, r, N, M);

    for (int i = 0; i < N; i++)
        printf(DataFormat("", "\n"), s[i]);

    for (int i = 0; i < N; i++)
        printf(DataFormat("", "\n"), q[i]);

    return 0;
}
