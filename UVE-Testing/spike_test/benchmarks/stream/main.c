#include <stdio.h>
#include "Functions.h"

extern void core(void *src1, void *src2, void *src3, uint64_t sizeN, DataType cscalar);
extern void core(void *src1, void *src2, void *src3, uint64_t sizeN, float cscalar);

int main() {

    DataType scalar = 3.0;

    DataType src_1[SIZE];
    DataType src_2[SIZE];
    DataType src_3[SIZE];

    initArray(src_1, SIZE);
    initArray(src_2, SIZE);
    initArray(src_3, SIZE);

    core(src_1, src_2, src_3, SIZE, scalar);

    for (int i = 0; i < SIZE; i++)
        printf( DataFormat("", "\n"), src_1[i]);


    return 0;
}
