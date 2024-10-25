#include "Functions.h"
#include <stdio.h>

extern void core(DataType *a, int N);

int main() {
    DataType a[SIZE];

    initArray(a, SIZE);

    core(a, SIZE);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), a[i]);
    }

    return 0;
}
