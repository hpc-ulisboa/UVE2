#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    DataType sum;
    int y, x, k, j;
    for (y = 1; y < PB_I - 1; y++)
        for (x = 1; x < PB_J - 1; x++)
            for (k = -1; k <= 1; k++)
                for (j = -1; j <= 1; j++)
                    dst[y * PB_J + x] += filter[(j + 1) * 3 + k + 1] * src[(y - j) * PB_J + (x - k)];

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

/*
for (y = 1; y < PB_I - 1; y++){
    for (x = 1; x < PB_J - 1; x++){
                dst[y * PB_I + x] = filter[0] * src[(y + 1)][(x + 1)] +
                            filter[1] * src[(y + 1)][(x)] +
                            filter[2] * src[(y + 1)][(x - 1)] +
                            filter[3] * src[(y)][(x + 1)] +
                            filter[4] * src[(y)][(x)] +
                            filter[5] * src[(y)][(x - 1)] +
                            filter[6] * src[(y - 1)][(x + 1)] +
                            filter[7] * src[(y - 1)][(x)] +
                            filter[8] * src[(y - 1)][(x - 1)];

        dst[y * PB_I + x] = filter[0] * src[(y - 1)][(x - 1)] +
                            filter[1] * src[(y - 1)][(x)] +
                            filter[2] * src[(y - 1)][(x + 1)] +
                            filter[3] * src[(y)][(x - 1)] +
                            filter[4] * src[(y)][(x)] +
                            filter[5] * src[(y)][(x + 1)] +
                            filter[6] * src[(y + 1)][(x - 1)] +
                            filter[7] * src[(y + 1)][(x)] +
                            filter[8] * src[(y + 1)][(x + 1)];
    }
}
*/