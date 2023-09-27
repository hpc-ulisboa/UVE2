#include "Functions.h"

#ifdef RUN_UVE
void uve_config_1(void *src1 /*data*/, void *src3 /*mean*/) {
    int v_len = 16;
    asm volatile(/*offset, size, stride*/
        // data stream load
        "ss.sta.ld.d           u1, %[src1], %[vl], %[one] \t\n" // D1: vector - linear access size V_len
        "ss.app                u1, zero, %[sn], %[sm] \t\n"  // D2: slide verticaly stride M access size N
        "ss.end                u1, zero, %[mv], %[vl] \t\n"  // D3: slide horizontaly by V_len, access size M/V_len

        // mean stream store
        "ss.st.d               u2, %[src3], %[sm], %[one] \t\n" // D1: linear access size M (should build vector automatically)

        "so.v.dp.d  u3, %[floatN], p0\t\n"
        :
        : [src1] "r"(src1), [src3] "r"(src3),
        [sn] "r"(SIZE), [sm] "r"(SIZE), [zero] "r"(0), [one] "r"(1),
        [vl] "r"(v_len), [mv] "r"(SIZE / v_len),
        [floatN] "r"((float)SIZE));

    return;
}

void uve_kernel_1() {
    asm volatile(
        "aLoop1: \t\n"
            "so.v.dp.d  u4, zero, p0\n\t"

            "add x12, x0, %[sn]\t\n"
            "bloop1: \t\n"
                "so.a.add.fp u4, u4, u1, p0\n\t"
                "sub x12, x12, %[one] \n\t"
            "bne x12, zero, bloop1 \n\t"

            "so.a.div.fp u2, u4, u3, p0\n\t"
        "so.b.nc	u1, aLoop1 \n\t"
        :
        : [sn] "r"(SIZE), [zero] "r"(0), [one] "r"(1));
}

void uve_config_2(void *src1 /*data*/, void *src3 /*mean*/) {
    int v_len = 16;
    // NOTE: Should vectorize automatically
    asm volatile(/*offset, size, stride*/
        // mean stream load
        "ss.sta.ld.d           u1, %[src3], %[sm], %[one] \t\n"  // D1: linear access size M
        "ss.end                u1, zero, %[sn], zero \t\n" // repeat N times

        // data stream load
        "ss.sta.ld.d           u2, %[src1], %[sm], %[one] \t\n" // D1: linear access size M
        "ss.end                u2, zero, %[sm], %[sm] \t\n"  // D2: strided M access size N

        // data stream store
        "ss.sta.st.d           u3, %[src1], %[sm], %[one] \t\n" // D1: linear access size M
        "ss.end                u3, zero, %[sm], %[sm] \t\n"  // D2: strided M access size N

        :
        : [src1] "r"(src1), [src3] "r"(src3),
        [sn] "r"(SIZE), [sm] "r"(SIZE), [zero] "r"(0), [one] "r"(1),
        [vl] "r"(v_len));

    return;
}

void uve_kernel_2() {
    asm volatile(
        "cLoop1: \t\n"
            "so.a.sub.fp u3, u2, u1, p0\n\t"
        "so.b.nc	u3, cLoop1 \n\t");
}

void uve_config_3(void *src1 /*data*/) {
    int v_len = 16;
    asm volatile( // offset, size, stride
        // data1 stream load - left column repeat
        "ss.sta.ld.d           u1, %[src1], %[vl], %[sm] \t\n"   // d1: strided m access size v_len
        "ss.app                u1, zero, %[mv], %[vm] \t\n"   // d2: slide vertically m/v_len times by m*v_len
        "ss.app                u1, zero, %[sm], zero  \t\n" // repeat j = m...0 times
        "ss.app                u1, zero, %[sm], %[one] \t\n"  // d3: new column stride 1
        "ss.end.mod.siz.dec    u1, %[sm], %[one] \t\n"           // decrement 'j'

        // data2 stream load - swipe all right columns
        "ss.sta.ld.d           u2, %[src1], %[vl], %[sm] \t\n"  // d1: strided m access size v_len
        "ss.app                u2, zero, %[mv], %[vm] \t\n"  // d2: slide vertically m/v_len times by m*v_len
        "ss.app                u2, zero, %[sm], %[one] \t\n" // d3: linear access size j = m...0
        "ss.app                u2, zero, %[sm], %[one] \t\n" // new column start stride 1
        "ss.end.mod.siz.dec    u2, %[sm], %[one] \t\n"          // decrement 'j'

        :
        : [src1] "r"(src1), [sn] "r"(SIZE), [sm] "r"(SIZE),
          [vl] "r"(v_len), [mv] "r"(SIZE / v_len), [vm] "r"(SIZE * v_len), [one] "r"(1));

    return;
}

void uve_kernel_3(void *src1, void *src2 /*cov*/) {
    int v_len = 16;
    // DataType *data = (DataType *)src1; /* NxM*/
    // DataType *cov = (DataType *)src2; /* MxM */
    // DataType float_n = (DataType) SIZE;
    // for (int i = 0; i < SIZE; i++)
    //   for (int j = i; j < SIZE; j++) {

    //     cov[i*SIZE+j] = 0.0;
    //     for (int k = 0; k < SIZE; k++)
    //       cov[i*SIZE+j] += data[k*SIZE+i] * data[k*SIZE+j];

    //     cov[i*SIZE+j] /= (float_n - 1.0);
    //     cov[j*SIZE+i] = cov[i*SIZE+j];
    //   }
    asm volatile(
        "add x10, zero, %[src2]\t\n"
        "add x11, zero, %[sm]\t\n"
        "add x12, zero, zero\t\n" // i
        "add x13, zero, zero\t\n" // j
        "fLoop1: \t\n"

            "so.v.dp.d  u7, zero, p0\n\t"
            "add x9, zero, %[mv]\t\n"
            "kloop1: \t\n"
                "so.a.mac.fp u7, u1, u2, p0\n\t"
                "sub x9, x9, %[one] \n\t"
            "bne x9, zero, kloop1 \n\t"

            "so.a.adds.fp f10, u7, p0 \n\t"

            "fdiv.s f10, f10, %[fn] \n\t"

            "mul x14, x12, x11 \n\t" // i*M
            "add x14, x14, x13 \n\t" // i*M + j
            "add x14, x10, x14 \n\t" // *cov + i*M + j
            "fsw f10, 0(x14) \n\t"   // store cov[i][j]

            "mul x14, x13, x11 \n\t" // j*M
            "add x14, x14, x12 \n\t" // j*M + i
            "add x14, x10, x14 \n\t" // *cov + j*M + i
            "fsw f10, 0(x14) \n\t"   // store cov[j][i]

            "add x13, x13, %[one] \n\t"  // inc j
            "bne x13, x11, skip1 \n\t"   // detect counter end
            "add x12, x12, %[one] \n\t"  // inc i
            "add x13, x12, zero \n\t" // reset j=i
            "sub x11, x11, %[one] \n\t"  // dec row size
            "skip1:"

        "so.b.nc	u1, fLoop1 \n\t"
        :
        : [src2] "r"(src2), [sn] "r"(SIZE), [sm] "r"(SIZE),
          [vl] "r"(v_len), [mv] "r"(SIZE / v_len), [sm1] "r"(SIZE + 1), [one] "r"(1), [fn] "f"((float)SIZE - 1.0)
        : "x10", "x11", "x12", "x13", "x9", "x14");
}

void core(void *src1, void *src2, void *src3){
    // mean
    //uve_config_1(src1, src3);
    //uve_kernel_1();
    // data
    //uve_config_2(src1, src3);
    //uve_kernel_2();
    // cov
    uve_config_3(src1);
    uve_kernel_3(src1, src2);
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core_kernel_1(void *src1, void *src2, void *src3) {
    DataType *data = (DataType *)src1; /* NxM*/
    DataType *cov = (DataType *)src2;  /* MxM */
    DataType *mean = (DataType *)src3; /* M */

    DataType float_n = (DataType)SIZE;

    for (int j = 0; j < SIZE; j++) {
        mean[j] = 0.0;
        for (int i = 0; i < SIZE; i++)
            mean[j] += data[i * SIZE + j];
        mean[j] /= float_n;
    }
}
void core_kernel_2(void *src1, void *src2, void *src3) {
    DataType *data = (DataType *)src1; /* NxM*/
    DataType *cov = (DataType *)src2;  /* MxM */
    DataType *mean = (DataType *)src3; /* M */

    DataType float_n = (DataType)SIZE;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            data[i * SIZE + j] -= mean[j];
}

void core_kernel_3(void *src1, void *src2, void *src3) {
    DataType *data = (DataType *)src1; /* NxM*/
    DataType *cov = (DataType *)src2;  /* MxM */
    DataType *mean = (DataType *)src3; /* M */

    DataType float_n = (DataType)SIZE;

    for (int i = 0; i < SIZE; i++)
        for (int j = i; j < SIZE; j++) {

            cov[i * SIZE + j] = 0.0;
            for (int k = 0; k < SIZE; k++)
                cov[i * SIZE + j] += data[k * SIZE + i] * data[k * SIZE + j];

            cov[i * SIZE + j] /= (float_n - 1.0);
            cov[j * SIZE + i] = cov[i * SIZE + j];
        }
}

void core(void *src1, void *src2, void *src3){
    // mean
    //core_kernel_1(src1, src3);
    // data
    //core_kernel_2(src1, src3);
    // cov
    core_kernel_3(src1, src2, src3);
}

#endif // RUN_SIMPLE