#include "Functions.h"
#define PB_D 10
#define PB_N 442
#define BATCH_SIZE PB_N
#define NUM_BATCHES PB_N / BATCH_SIZE
#define EPOCHS 100

long int start = 0, end = 0;

DataType core(DataType **x, DataType *x_array, DataType *y, DataType *y_err, DataType *sgd_model, DataType *y_fitted) {
    int e, /*b,*/ i, j;
    DataType yhat, raw_update, intercept_der, intercept = 0;
    DataType learning_rate = 0.01 * 2.0;

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));    

    for (e = 0; e < EPOCHS; e++) {
        // for (b = 0; b < PB_N; b += BATCH_SIZE){
        for (i = 0; i /*< BATCH_SIZE && b+i*/ < PB_N; i++) {
            yhat = 0.0;
            for (j = 0; j < PB_D; j++)
                yhat += x[/*b + */ i][j] * sgd_model[j];
            y_err[i] = y[/*b + */ +i] - (yhat + intercept);
        }

        intercept_der = 0.0;
        for (i = 0; i < PB_N /*BATCH_SIZE*/; i++)
            intercept_der += y_err[i];
            
        //printf("\nintercept_der: %.4lf\n", intercept_der);
        intercept_der = intercept_der / (DataType)PB_N /*BATCH_SIZE*/;
        intercept = intercept + (learning_rate * intercept_der);

        //printf("intercept: %.4lf\n", intercept);

        for (i = 0; i < PB_D; i++) {
            raw_update = 0.0;
            for (j = 0; j /*< BATCH_SIZE && b + j*/ < PB_N; j++)
                raw_update += x[/*b +*/ j][i] * y_err[j];
            sgd_model[i] = sgd_model[i] + (learning_rate * raw_update);
        }

        // printf("\n%.4lf\n", intercept);
    }

    DataType r;  

    for (int i = 0; i < PB_N; i++) {
        r = 0.0;
        for (int j = 0; j < PB_D; j++)
            r += x[i][j] * sgd_model[j];

        y_fitted[i] = r + intercept;
    }

    DataType y_mean = 0.0;
    for (int i = 0; i < PB_N; i++)
        y_mean += y[i];
    y_mean = y_mean / (DataType)PB_N;

    DataType res = 0.0;
    for (int i = 0; i < PB_N; i++)
        res += (y[i] - y_fitted[i]) * (y[i] - y_fitted[i]);

    DataType tot = 0.0;
    for (int i = 0; i < PB_N; i++)
        tot += (y[i] - y_mean) * (y[i] - y_mean);

    DataType result = 1 - (res / tot);

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));    
    printf("%ld\n%ld\n", start, end);

    return result;
}