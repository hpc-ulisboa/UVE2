#include "Functions.h"
#define PB_D 10
#define PB_N 442
#define BATCH_SIZE 1 // MUST BE 1
#define NUM_BATCHES PB_N / BATCH_SIZE
#define EPOCHS 100

#ifdef RUN_UVE

void core(void **x, void *y, void *y_err, void *sgd_model){
    asm volatile(/*offset, size, stride*/ /*mod-> size, disp*/

        // KERNEL 1 Streams
        // sgd_model(j) stream load
        "ss.sta.ld.d           u7, %[sgd_model], %[epochs], zero \t\n"
        "ss.app                u7, zero, %[pb_n], zero \t\n"
        "ss.end                u7, zero, %[pb_d], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // x(b,j) stream load
        "ss.sta.ld.d           u6, %[x], %[epochs], zero \t\n"
        "ss.app                u6, zero, %[pb_n], %[pb_d] \t\n"
        "ss.end                u6, zero, %[pb_d], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // y_err(i) stream store => y(0) since BATCH_SIZE = 1
        "ss.sta.st.d           u8, %[y_err], %[epochs], zero \t\n"
        "ss.end                u8, zero, %[pb_n], zero \t\n"

        // y(b) stream load
        "ss.sta.ld.d           u10, %[y], %[epochs], zero \t\n"
        "ss.end                u10, zero, %[pb_n], %[one] \t\n"

        // KERNEL 3 Streams
        // sgd_model(j) stream load kernel3
        "ss.sta.ld.d           u12, %[sgd_model], %[epochs], zero \t\n"
        "ss.app                u12, zero, %[pb_n], zero \t\n"
        "ss.end                u12, zero, %[pb_d], %[one] \t\n"
        "ss.cfg.vec            u12 \t\n"

        // x(b,j) stream load kernel3
        "ss.sta.ld.d           u16, %[x], %[epochs], zero \t\n"
        "ss.app                u16, zero, %[pb_n], %[pb_d] \t\n"
        "ss.end                u16, zero, %[pb_d], %[one] \t\n"
        "ss.cfg.vec            u16 \t\n"

        // sgd_model(j) stream store kernel3
        "ss.sta.st.d           u13, %[sgd_model], %[epochs], zero \t\n"
        "ss.app                u13, zero, %[pb_n], zero \t\n"
        "ss.end                u13, zero, %[pb_d], %[one] \t\n"
        "ss.cfg.vec            u13 \t\n"

        :
        : [sgd_model] "r"(sgd_model), [x] "r"(x), [y_err] "r"(y_err), [y] "r"(y),
        [epochs] "r"(EPOCHS), [pb_n] "r"(PB_N), [pb_d] "r"(PB_D), [one] "r"(1) );

    asm volatile(

        "so.v.mvsv.d u1, zero  \t\n"  // intercept
        "so.v.mvsv.d u2, %[lr] \t\n" // adjusted learning rate

        ".SLOOP_1%=: \t\n"

            // KERNEL1
            "so.v.dp.d u4, zero, p0 \t\n"

            ".SLOOP_1_0_0%=:  \t\n"
                "so.a.mul.fp  u5, u6, u7, p0 \t\n"
                "so.a.add.fp  u4, u4, u5, p0 \t\n"
            "so.b.ndc.1 u6, .SLOOP_1_0_0%= \t\n"

            "so.a.adde.fp  u5, u4, p0 \t\n"
            "so.a.add.fp  u11, u5, u1, p0 \t\n"
            "so.a.sub.fp  u8, u10, u11, p0 \t\n" // store y_err
            //"so.v.mvsv.d u9, u8  \t\n" //make a copy of y_err

            // KERNEL2
            "so.a.mul.fp  u3, u8, u2, p0 \t\n" //(-learning_rate*-2)*yerr
            "so.a.add.fp  u1, u1, u3, p0 \t\n" // update intercept

            // KERNEL3
            ".SLOOP_1_0_1%=:  \t\n"
                "so.a.mul.fp  u15, u16, u2, p0 \t\n"
                "so.a.mul.fp  u14, u15, u8, p0 \t\n"
                "so.a.add.fp  u13, u12, u14, p0 \t\n"
            "so.b.ndc.1 u16, .SLOOP_1_0_1%= \t\n"

        "so.b.nc u8, .SLOOP_1%= \t\n"
        :
        : [lr] "r"((double)(0.02)));
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType **x, DataType *y, DataType *y_err, DataType *sgd_model){
    int e, b, i, j;
    DataType yhat, raw_update, intercept_der, intercept = 0;
    DataType learning_rate = 0.01;

    for (e = 0; e < EPOCHS; e++){
        for (b = 0; b < PB_N; b += BATCH_SIZE){
            for (i = 0; i < BATCH_SIZE /*&& b+i < PB_N*/; i++){
                yhat = 0.0;
                for (j = 0; j < PB_D; j++)
                    yhat += x[b + i][j] * sgd_model[j];
                y_err[i] = y[b + i] - (yhat + intercept);
            }

            intercept_der = 0.0;
            for (i = 0; i < BATCH_SIZE; i++)
                intercept_der += y_err[i];
            intercept_der = -2.0 * (intercept_der / (DataType)BATCH_SIZE);
            intercept = intercept - (learning_rate * intercept_der);

            for (i = 0; i < PB_D; i++){
                raw_update = 0.0;
                for (j = 0; j < BATCH_SIZE && b + j < PB_N; j++)
                    raw_update += x[b + j][i] * y_err[j];
                sgd_model[i] = sgd_model[i] - (learning_rate * -2.0 * raw_update);
            }
        }
    }

    //printf("\n%.4lf", intercept);
}
#endif // RUN_SIMPLE
