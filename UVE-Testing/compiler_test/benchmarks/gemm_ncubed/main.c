#include "Functions.h"
#include <stdio.h>

extern void core(DataType *m1, DataType *m2, DataType *prod, int row_size, int col_size);

int main() {
    DataType m1[SIZE * SIZE];
    DataType m2[SIZE * SIZE];
    DataType prod[SIZE * SIZE];

    initZero(prod, SIZE * SIZE);
    initArray2D(m1, SIZE, SIZE);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m2[i * SIZE + j] = (DataType)(i % SIZE) / SIZE;
        }
    }

    core(m1, m2, prod, SIZE, SIZE);

    for (int i = 0; i < SIZE * SIZE; i++) {
        printf(DataFormat("", "\n"), prod[i]);
    }

    return 0;
}
