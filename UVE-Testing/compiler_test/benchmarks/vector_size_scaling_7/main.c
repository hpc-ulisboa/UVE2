#include "Functions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern void core(DataType *a, DataType *b, DataType *c, DataType *d, DataType *e, DataType *f, DataType *g, DataType *h, DataType *i, DataType *j, DataType *res);

int main() {

    DataType a[SIZE];
    DataType b[SIZE];
    DataType c[SIZE];
    DataType d[SIZE];
    DataType e[SIZE];
    DataType f[SIZE];
    DataType g[SIZE];
    DataType h[SIZE];
    DataType i[SIZE];
    DataType j[SIZE];
    DataType res[SIZE];

    // Initialize the arrays with random values
    for (int k = 0; k < SIZE; k++) {
        a[k] = rand() % 100;
        b[k] = rand() % 100;
        c[k] = rand() % 100;
        d[k] = rand() % 100;
        e[k] = rand() % 100;
        f[k] = rand() % 100;
        g[k] = rand() % 100;
        h[k] = rand() % 100;
        i[k] = rand() % 100;
        j[k] = rand() % 100;
    }
    initZero(res, SIZE);

    core(a, b, c, d, e, f, g, h, i, j, res);

    for (int k = 0; k < SIZE; k++)
        printf(DataFormat("", "\n"), res[k]);

    return 0;
}
