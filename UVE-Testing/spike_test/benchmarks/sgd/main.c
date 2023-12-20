/**
 * gcc -L/usr/lib -Wl,-rpath=/usr/lib -o sgd sgd.c -lpapi -O3
 * objdump -dSCze -M numeric sgd > sgd.dump
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Functions.h"

#define PB_D 10
#define PB_N 442
#define BATCH_SIZE PB_N
#define NUM_BATCHES PB_N / BATCH_SIZE
#define EPOCHS 100

extern DataType core(DataType **x, DataType *x_array, DataType *y, DataType *y_err, DataType *sgd_model, DataType *y_fitted);

/* Array initialization. */
static void init_array(DataType **x, DataType *y){
    const DataType X_data[] = sgdX;
    const DataType y_data[] = sgdY;
    int i, j;

    for (i = 0; i < PB_N; i++){
        for (j = 0; j < PB_D; j++)
            x[i][j] = X_data[i*PB_D+j]; //( (DataType) ((i+j) % PB_N)) / 100.0;
        y[i] = y_data[i];           // 1.0 + ( (DataType) ((i) % PB_N)) / 100.0;
    }
}

int main(int argc, char **argv){
    /* Retrieve problem size. */
    int i;
    DataType intercept, result;

    /* Variable declaration/allocation. */

    DataType *x_array = (DataType *)malloc(PB_N * PB_D * sizeof(DataType));
    DataType **x = (DataType **)malloc(PB_N * sizeof(DataType *));
    for (i = 0; i < PB_N; i++)
        x[i] = x_array + i * PB_D;

    DataType *y = (DataType *)malloc(PB_N * sizeof(DataType));
    DataType *y_fitted = (DataType *)malloc(PB_N * sizeof(DataType));
    DataType *y_err = (DataType *)malloc(PB_N * sizeof(DataType));
    DataType *sgd_model = (DataType *)malloc(PB_D * sizeof(DataType));

    /* Initialize array(s). */
    init_array(x, y);
    initZero(y_err, BATCH_SIZE);
    initConstant(sgd_model, PB_D, 1.0);

    /* Run kernel. */
    result= core(x, x_array, y, y_err, sgd_model, y_fitted);

    for (i = 0; i < PB_D; i++)
        printf( DataFormat("", "\n"), sgd_model[i]);
    printf( DataFormat("\n", "\n"), result);

    free(x);
    free(y);
    free(y_fitted);
    free(y_err);
    free(sgd_model);

    return 0;
}
