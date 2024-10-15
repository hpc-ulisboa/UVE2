#include "Functions.h"
#include <stdio.h>

extern void core(DataType *r, DataType *y, DataType *sum, uint64_t N);

int main() {
    DataType r[SIZE];
    DataType y[SIZE];
    DataType sum[SIZE];

    for (int i = 0; i < SIZE; i++)
        r[i] = (SIZE + 1 - i);

    initZero(y, SIZE);
    initZero(sum, SIZE);

    core(r, y, sum, SIZE);

    for (int i = 0; i < SIZE; i++)
        printf(DataFormat("", "\n"), y[i]);

    return 0;
}
