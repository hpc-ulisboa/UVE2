#include "Functions.h"
#define PB_D 10
#define PB_N 442
#define BATCH_SIZE PB_N
#define NUM_BATCHES PB_N / BATCH_SIZE
#define EPOCHS 100

long int start = 0, end = 0;

#ifdef RUN_UVE

DataType core_kernel(void *x_array, void *y, void *y_err, void *sgd_model, long int *insns) {
    DataType intercept;

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
        // KERNEL 1 Streams
        // sgd_model(j) stream load
        "ss.sta.ld.d.v.m       u1, %[sgd_model] \n"
        "ss.app                u1, zero, %[epochs], zero \n"
        "ss.app                u1, zero, %[pb_n], zero \n"
        "ss.end                u1, zero, %[pb_d], %[one] \n"

        // x(i,j) stream load
        "ss.sta.ld.d.v.m       u2, %[x] \n"
        "ss.app                u2, zero, %[epochs], zero \n"
        "ss.app                u2, zero, %[pb_n], %[pb_d] \n"
        "ss.end                u2, zero, %[pb_d], %[one] \n"

        // y_err(i)
        "ss.sta.st.d           u3, %[y_err] \n"
        "ss.app                u3, zero, %[epochs], zero \n"
        "ss.end                u3, zero, %[pb_n], %[one] \n"

        // y(i) stream load
        "ss.sta.ld.d           u4, %[y] \n"
        "ss.app                u4, zero, %[epochs], zero \n"
        "ss.end                u4, zero, %[pb_n], %[one] \n"

        // KERNEL 2 Streams
        // y_err(i) stream load
        "ss.sta.ld.d.v         u5, %[y_err] \n"
        "ss.app                u5, zero, %[epochs], zero \n"
        "ss.end                u5, zero, %[pb_n], %[one] \n"

        // KERNEL 3 Streams
        // x(j,i) stream load kernel3
        "ss.sta.ld.d.v         u6, %[x] \n"
        "ss.app                u6, zero, %[epochs], zero \n"
        "ss.app                u6, zero, %[pb_d], %[one] \n"
        "ss.end                u6, zero, %[pb_n], %[pb_d] \n"

        // y_err(j)
        "ss.sta.ld.d.v         u7, %[y_err] \n"
        "ss.app                u7, zero, %[epochs], zero \n"
        "ss.app                u7, zero, %[pb_d], zero \n"
        "ss.end                u7, zero, %[pb_n], %[one] \n"

        // sgd_model(j) stream load kernel3
        "ss.sta.ld.d           u8, %[sgd_model] \n"
        "ss.app                u8, zero, %[epochs], zero \n"
        "ss.end                u8, zero, %[pb_d], %[one] \n"

        // sgd_model(j) stream store kernel3
        "ss.sta.st.d           u9, %[sgd_model] \n"
        "ss.app                u9, zero, %[epochs], zero \n"
        "ss.end                u9, zero, %[pb_d], %[one] \n"

        "so.v.mvsv.d u10, zero \n"   // intercept
        "so.v.mvsv.d u11, %[lr] \n"   // adjusted learning rate
        "so.v.mvsv.d u12, %[pb_n_d] \n" // pb_n (double)

        ".SLOOP_1%=: \n"

            // KERNEL 1
            ".SLOOP_1_0%=: \n"

                "so.v.dp.d u13, zero, p0 \n"

                ".SLOOP_1_0_0%=: \n"
                    /*"so.a.mul.fp  u14, u2, u1, p0 \n"   // x(i,j) *sgd_model(j)
                    "so.a.add.fp  u13, u13, u14, p0 \n" // yhat += x(i,j) *sgd_model(j)*/
                    "so.a.mac.fp  u13, u2, u1, p0 \n" // yhat += x(i,j) *sgd_model(j)
                "so.b.ndc.3 u1, .SLOOP_1_0_0%= \n"

                "so.a.adde.fp  u15, u13, p0 \n" // reduce yhat
                "so.a.add.fp  u15, u15, u10, p0 \n" // yhat += intercept
                "so.a.sub.fp  u3, u4, u15, p0 \n" // y_err = y - yhat
                //"so.v.mvsv.d u9, u8 \n" // make a copy of y_err

            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            // KERNEL 2
            "so.v.mvsv.d u16, zero \n" // intercept_der

            ".SLOOP_1_1%=: \n"
                "so.a.adde.acc.fp  u16, u5, p0 \n" // intercept_der += y_err(i)
            "so.b.ndc.2 u5, .SLOOP_1_1%= \n"

            //"so.a.adde.fp  u17, u16, p0 \n"     // reduce intercept_der
            "so.a.div.fp  u16, u16, u12, p0 \n" // intercept_der /= PB_N
            /*"so.a.mul.fp  u16, u16, u11, p0 \n" // intercept_der *= lr
            "so.a.add.fp  u10, u10, u16, p0 \n" // intercept += intercept_der*/
            "so.a.mac.fp  u10, u16, u11, p0 \n" // intercept += intercept_der

            // KERNEL 3
            ".SLOOP_1_2%=: \n"

                "so.v.dp.d u18, zero, p0 \n" // raw_update

                ".SLOOP_1_2_0%=: \n"
                    "so.a.mul.fp  u19, u6, u7, p0 \n"   // x(j,i) * y_err(j)
                    "so.a.adde.acc.fp  u18, u19, p0 \n" // raw_update += x(j,i) * y_err(j)
                "so.b.ndc.3 u6, .SLOOP_1_2_0%= \n"

                //"so.a.adde.fp  u19, u18, p0 \n"     // reduce raw_update
                "so.a.mul.fp  u18, u18, u11, p0 \n" // raw_update * lr
                "so.a.add.fp  u9, u8, u18, p0 \n"   // sgd_model(j) += raw_update * lr

            "so.b.ndc.2 u6, .SLOOP_1_2%= \n"

        "so.b.nc u1, .SLOOP_1%= \n"

        "so.v.mvvs %[i], u10 \n" // intercept

        : [i] "=r" (intercept)
        : [sgd_model] "r"(sgd_model), [x] "r"(x_array), [y_err] "r"(y_err), [y] "r"(y),
          [epochs] "r"(EPOCHS), [pb_n] "r"(PB_N), [pb_d] "r"(PB_D), [one] "r"(1),
          [lr] "r"((DataType)(0.02)), [pb_n_d] "r"((DataType)PB_N));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    *insns = end - start;

    return intercept;
}

long int predict(void *y_fitted, void *x_array, void *sgd_model, DataType intercept) {

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
        // KERNEL 1 Streams
        // sgd_model(j) stream load
        "ss.sta.ld.d.v.m       u1, %[sgd_model] \n"
        "ss.app                u1, zero, %[pb_n], zero \n"
        "ss.end                u1, zero, %[pb_d], %[one] \n"

        // x(i,j) stream load
        "ss.sta.ld.d.v.m       u2, %[x] \n"
        "ss.app                u2, zero, %[pb_n], %[pb_d] \n"
        "ss.end                u2, zero, %[pb_d], %[one] \n"

        // y_fitted(i)
        "ss.sta.st.d           u3, %[y_fitted] \n"
        "ss.end                u3, zero, %[pb_n], %[one] \n"

        "so.v.mvsv.d u4, %[i] \n" // intercept

        ".SLOOP_1%=: \n"

            "so.v.dp.d u5, zero, p0 \n" // r

            ".SLOOP_1_0%=: \n"
                /*"so.a.mul.fp  u6, u2, u1, p0 \n" // aux = x(i,j) *sgd_model(j)
                "so.a.adde.acc.fp  u5, u6, p0 \n" // r += aux*/
                "so.a.mac.fp  u5, u2, u1, p0 \n" // r += x(i,j) *sgd_model(j)
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.adde.fp u7, u5, p0 \n"    // reduce r
            "so.a.add.fp  u3, u7, u4, p0 \n" // store r + intercept

        "so.b.nc  u1, .SLOOP_1%= \n"

        :: [i] "r"(intercept), [sgd_model] "r"(sgd_model), [x] "r"(x_array), [y_fitted] "r"(y_fitted),
          [pb_n] "r"(PB_N), [pb_d] "r"(PB_D), [one] "r"(1));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    return end - start;
}

DataType r2_score(void *y_fitted, void *y, long int *insns) {
    DataType result = 0.0;

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile (
        // y(i) streams load
        "ss.sta.ld.d           u1, %[y] \n"
        "ss.end                u1, zero, %[pb_n], %[one] \n"
        "ss.sta.ld.d           u2, %[y] \n"
        "ss.end                u2, zero, %[pb_n], %[one] \n"
        "ss.sta.ld.d           u3, %[y] \n"
        "ss.end                u3, zero, %[pb_n], %[one] \n"

        // y_fitted(i) stream load
        "ss.sta.ld.d           u4, %[y_fitted] \n"
        "ss.end                u4, zero, %[pb_n], %[one] \n"

        "so.v.mvsv.d u5, %[pb_n_d] \n" // PB_N (double)

        "so.v.dp.d u6, zero, p0 \n" // y_mean

        ".SLOOP_1%=: \n"
            "so.a.add.fp  u6, u6, u1, p0 \n" // y_mean += y[i]
        "so.b.nc u1, .SLOOP_1%= \n"

        "so.a.adde.fp  u7, u6, p0 \n"    // reduce y_mean
        "so.a.div.fp  u7, u7, u5, p0 \n" // y_mean = y_mean / (DataType)PB_N

        "so.v.dp.d u8, zero, p0 \n" // res

        ".SLOOP_2%=: \n"
            "so.a.sub.fp  u9, u2, u4, p0 \n" // y[i] - y_fitted[i]
            "so.a.mul.fp  u9, u9, u9, p0 \n" // (y[i] - y_fitted[i]) * (y[i] - y_fitted[i])
            //"so.a.add.fp  u8, u8, u9, p0 \n" // res += (y[i] - y_fitted[i]) * (y[i] - y_fitted[i])
            "so.a.adde.acc.fp  u8, u9, p0 \n" // res += (y[i] - y_fitted[i]) * (y[i] - y_fitted[i])
        "so.b.nc u2, .SLOOP_2%= \n"

        //"so.a.adde.fp  u10, u8, p0 \n"    // reduce res

        "so.v.dp.d u11, zero, p0 \n" // tot

        ".SLOOP_3%=: \n"
            "so.a.sub.fp  u12, u3, u7, p0 \n" // y[i] - y_mean
            "so.a.mul.fp  u12, u12, u12, p0 \n" // (y[i] - y_mean) * (y[i] - y_mean)
            //"so.a.add.fp  u11, u11, u12, p0 \n" // tot += (y[i] - y_mean) * (y[i] - y_mean)
            "so.a.adde.acc.fp  u11, u12, p0 \n" // tot += (y[i] - y_mean) * (y[i] - y_mean)
        "so.b.nc u3, .SLOOP_3%= \n"

         //"so.a.adde.fp  u15, u11, p0 \n" // reduce tot

        "so.v.mvsv.d u14, %[one_d] \n" // 1.0

        "so.a.div.fp  u11, u8, u11, p0 \n" // res / tot
        "so.a.sub.fp  u14, u14, u11, p0 \n"  // 1.0 - (res / tot)

        "so.v.mvvs %[result], u14 \n" // result = 1.0 - (res / tot)
        //"so.a.adds.acc.fp %[result], u14, p0 \n" // result = 1.0 - (res / tot) TO TEST ADDS.ACC INSNS

        //: [result] "+f"(result) // TO TEST ADDS.ACC INSNS

        : [result] "=&r"(result)
        : [pb_n_d] "r"((DataType)(PB_N)), [one_d] "r"((DataType)(1)), [y] "r"(y),
          [y_fitted] "r"(y_fitted), [pb_n] "r"(PB_N), [one] "r"(1));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    *insns = end - start;

    return result;
}

DataType core(DataType **x, DataType *x_array, DataType *y, DataType *y_err, DataType *sgd_model, DataType *y_fitted) {
    long int insns1, insns2, insns3;

    DataType intercept = core_kernel(x_array, y, y_err, sgd_model, &insns1);
    insns2 = predict(y_fitted, x_array, sgd_model, intercept);
    DataType r = r2_score(y_fitted, y, &insns3);

    insns3 += insns1 + insns2;

    printf("%d\n%ld\n", 0, insns3);

    return r;
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE
DataType core_kernel(DataType **x, DataType *y, DataType *y_err, DataType *sgd_model, long int *insns) {
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

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    *insns = end - start;

    return intercept;
}

long int predict(DataType *y_fitted, DataType **x, DataType *sgd_model, DataType intercept) {
    DataType r;

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));    

    for (int i = 0; i < PB_N; i++) {
        r = 0.0;
        for (int j = 0; j < PB_D; j++)
            r += x[i][j] * sgd_model[j];

        y_fitted[i] = r + intercept;
    }

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    return end - start;
}

DataType r2_score(DataType *y_fitted, DataType *y, long int *insns) {

    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));    

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

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end)); 

    *insns = end - start;  

    return 1.0 - (res / tot);
}

DataType core(DataType **x, DataType *x_array, DataType *y, DataType *y_err, DataType *sgd_model, DataType *y_fitted) {
    long int insns1, insns2, insns3;

    DataType intercept = core_kernel(x, y, y_err, sgd_model, &insns1);
    insns2 = predict(y_fitted, x, sgd_model, intercept);
    DataType r = r2_score(y_fitted, y, &insns3);

    insns3 += insns1 + insns2;

    printf("%d\n%ld\n", 0, insns3);

    return r;
}
#endif // RUN_SIMPLE
