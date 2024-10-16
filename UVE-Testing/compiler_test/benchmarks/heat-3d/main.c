#include "Functions.h"
#include <stdio.h>

extern void core(DataType *ex, DataType *ey, DataType *hz, DataType *fict, int NX, int NY, int TMAX);

int main() {

    int NX = SIZE;
    int NY = SIZE + SIZE / 3;
    int TMAX = SIZE - SIZE / 3;

    DataType ex[NX * NY];
    DataType ey[NX * NY];
    DataType hz[NX * NY];
    DataType fict[TMAX];

    for (int i = 0; i < TMAX; i++)
        fict[i] = (DataType)i;
    for (int i = 0; i < NX; i++)
        for (int j = 0; j < NY; j++) {
            ex[i*NY+j] = ((DataType)i * (j + 1)) / NX;
            ey[i*NY+j] = ((DataType)i * (j + 2)) / NY;
            hz[i*NY+j] = ((DataType)i * (j + 3)) / NX;
        }

    core(ex, ey, hz, fict, NX, NY, TMAX);

    for (int i = 0; i < NX*NY; i++) {
        printf(DataFormat("", "\n"), ex[i]);
    }

    return 0;
}
