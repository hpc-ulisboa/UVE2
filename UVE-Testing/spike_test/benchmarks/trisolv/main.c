#include "Functions.h"
#include <stdio.h>

extern void core(DataType L[SIZE][SIZE], DataType b[SIZE], DataType x[SIZE]);

int main() {
    float L[SIZE * SIZE];
    float b[SIZE];
    float x[SIZE];

    
    initArray2D(L);
    initArray(b);
    initArray(x);
    
/*
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j)
            L[i * SIZE + j] = i + 1;
        b[i] = i + 1;
        x[i] = i + 1;
    }

    // print all the arrays

    printf("L:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf(DataFormat("", "\t"), L[i * SIZE + j]);
        printf("\n");
    }

    printf("\nb:\n");
    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\t"), b[i]);
    }
    printf("\n");

    printf("\nx:\n");
    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\t"), x[i]);
    }

    printf("\n\nResults (x):\n");
*/

    core(L, b, x);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), x[i]);
    }

    return 0;
}
