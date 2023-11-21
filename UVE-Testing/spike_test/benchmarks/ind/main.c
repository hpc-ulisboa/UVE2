#include "Functions.h"
#include <stdio.h>

extern void core(DataType *dest, DataType *src, int *ind);

int main() {
    DataType src[SIZE*SIZE];
    DataType dest[SIZE];
    int ind[SIZE];

    initArray(src, SIZE*SIZE);
    initZero(dest, SIZE);

    for (int i = 0; i < SIZE; i++)
        ind[i] = i * 2;

    core(dest, src, ind);

    for (int i = 0; i < SIZE; i++)
        printf(DataFormat("", "\n"), dest[i]);

    return 0;
}
