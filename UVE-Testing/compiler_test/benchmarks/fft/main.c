#include "Functions.h"
#include <math.h>
#include <stdio.h>

#define twoPI 6.283185307179586476925286766559

extern void core(DataType *real, DataType *img, DataType *real_twid, DataType *img_twid, DataType *temp, int FFT_SIZE);

int main() {
    DataType real[SIZE];
    DataType img[SIZE];
    DataType real_twid[SIZE];
    DataType img_twid[SIZE];
    DataType temp[SIZE];

    DataType typed;

    initArray(real, SIZE);

    for (int i = 0; i < SIZE; i++) {
        img[i] = (DataType)(i % SIZE) / SIZE;
    }

    for (int n = 0; n < (SIZE >> 1); n++) {
        typed = (DataType)(twoPI * n / SIZE);
        real_twid[n] = cos(typed);
        img_twid[n] = (-1.0) * sin(typed);
    }

    core(real, img, real_twid, img_twid, temp, SIZE);

    for (int i = 0; i < SIZE; i++) {
        printf(DataFormat("", "\n"), real[i]);
        printf(DataFormat("", "\n"), img[i]);
    }

    return 0;
}
