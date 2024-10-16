#include "Functions.h"
#include <stdio.h>

extern void core(DataType *u, DataType *v, DataType *p, DataType *q, int N, int STEPS, DataType a, DataType b, DataType c, DataType d, DataType e, DataType f, DataType DX, DataType DY, DataType DT, DataType B1, DataType B2, DataType mul1, DataType mul2);

int main() {

    int STEPS = SIZE < 20 ? SIZE : SIZE/2;

    DataType u[SIZE * SIZE];
    DataType v[SIZE * SIZE];
    DataType p[SIZE * SIZE];
    DataType q[SIZE * SIZE];
    initArray2D(u, SIZE, SIZE);
    initZero(v, SIZE*SIZE);
    initZero(p, SIZE*SIZE);
    initZero(q, SIZE*SIZE);

    
    DataType DX, DY, DT;
    DataType B1, B2;
    DataType mul1, mul2;
    DataType a, b, c, d, e, f;
    DX = 1.0 / (DataType)SIZE;
    DY = 1.0 / (DataType)SIZE;
    DT = 1.0 / (DataType)STEPS;
    B1 = 2.0;
    B2 = 1.0;
    mul1 = B1 * DT / (DX * DX);
    mul2 = B2 * DT / (DY * DY);

    a = -mul1 / 2.0;
    b = 1.0 + mul1;
    c = a;
    d = -mul2 / 2.0;
    e = 1.0 + mul2;
    f = d;

    core(u, v, p, q, SIZE, STEPS, a, b, c, d, e, f, DX, DY, DT, B1, B2, mul1, mul2);

    for (int i = 0; i < SIZE * SIZE; i++) {
        printf(DataFormat("", "\n"), u[i]);
    }

    return 0;
}
