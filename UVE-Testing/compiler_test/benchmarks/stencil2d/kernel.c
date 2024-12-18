// https://github.com/breagen/MachSuite/tree/master/stencil/stencil2d
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *sol, DataType *orig, DataType *filter, int row_size, int col_size) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));
    int r, c, k1, k2;
#ifdef RVV
    DataType temp, mul;

    for (r = 0; r < row_size - 2; r++) {
        for (c = 0; c < col_size - 2; c++) {
            temp = (DataType)0;
            for (k1 = 0; k1 < 3; k1++) {
                for (k2 = 0; k2 < 3; k2++) {
                    mul = filter[k1 * 3 + k2] * orig[(r + k1) * col_size + c + k2];
                    temp += mul;
                }
            }
            sol[(r * col_size) + c] = temp;
        }
    }
#else
    for (r = 0; r < row_size - 2; r++) {
        for (c = 0; c < col_size - 2; c++) {
            for (k1 = 0; k1 < 3; k1++) {
                for (k2 = 0; k2 < 3; k2++) {
                    sol[(r * col_size) + c] += filter[k1 * 3 + k2] * orig[(r + k1) * col_size + c + k2];
                }
            }
        }
    }
#endif
    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}