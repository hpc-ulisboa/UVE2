#include "Functions.h"

#ifdef RUN_UVE

void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType float_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "ss.st.d u1, %[mean], %[M], %[one] \t\n"

        "ss.sta.ld.d    u2, %[data], %[M], %[one] \t\n"
        "ss.end         u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.st.d u3, %[mean], %[M], %[one] \t\n"
        :
        : [M] "r"(sizeM), [N] "r"(sizeN), [float_n] "r"(float_n), [data] "r"(data), [mean] "r"(mean), [one] "r"(1)
        :);

    asm volatile(
        "so.v.mvsv.d u2, %[N] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.d    u1, zero \t\n"
            "so.v.dp.d      u5, zero, p0 \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.add.fp u5, u2, u5, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_1_0%= \t\n"

            "so.a.adde.fp   u2, u5, p0 \t\n"
            "so.a.div.fp    u3, u2, u4, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n" 
        :
        : [N] "r"(sizeN)
        :);

    // kernel 2
    asm volatile(
        "ss.sta.st.d    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.d    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.d    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        :
        : [M] "r"(sizeM), [N] "r"(sizeN), [float_n] "r"(float_n), [data] "r"(data), [mean] "r"(mean), [one] "r"(1)
        :);

    asm volatile(
        ".SLOOP_2%=: \t\n"
            "so.a.sub.fp  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n" :::);

    // kernel 3
    asm volatile(
        "ss.sta.st.d        u1, %[float_n], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[M], %[one] \t\n"

        "ss.sta.ld.d        u2, %[data], %[M], zero \t\n"
        "ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], %[one] \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.ld.d        u3, %[data], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.app             u3, zero, %[M], zero \t\n"
        "ss.end             u3, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u3 \t\n"

        "ss.sta.st.d        u4, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[one] \t\n"

        "ss.sta.st.d        u5, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u5, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u5, %[M], %[one] \t\n"
        "ss.end             u5, zero, %[M], %[M] \t\n"
        :
        : [M] "r"(sizeM), [N] "r"(sizeN), [float_n] "r"(float_n), [data] "r"(data), [cov] "r" (cov), [mean] "r"(mean), [one] "r"(1)
        :);

        asm volatile(
            "so.v.mvsv.d u6, %[float_nn] \t\n"

            ".SLOOP_3%=: \t\n"
                "so.v.mvsv.d    u1, zero \t\n"
                "so.v.dp.d      u7, zero, p0 \t\n"

                ".SLOOP_3_0_0%=: \t\n"
                    "so.a.mul.fp  u8, u3, u2, p0 \t\n"
                    "so.a.add.fp  u7, u7, u8, p0 \t\n"
                "so.b.ndc.1 u3, .SLOOP_3_0_0%= \t\n"

                "so.a.adde.fp   u8, u7, p0 \t\n"
                "so.a.div.fp    u9, u8, u6, p0 \t\n"
                "so.v.mv        u4, u9, p0 \t\n"
                "so.v.mv        u5, u9, p0 \t\n"
            "so.b.nc u1, .SLOOP_3%= \t\n"
            :
            : [float_nn] "r"((double)(float_n * -1-0))
            :);
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core(int sizeM, int sizeN, DataType float_n, DataType *data, DataType *cov, DataType *mean) {
    int i, j, k;

    // kernel 1
    for (j = 0; j < sizeM; j++) {
        mean[j] = 0.0;
        for (i = 0; i < sizeN; i++)
            mean[j] += data[i * sizeM + j];
        mean[j] /= float_n;
    }

    // kernel 2
    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeM; j++)
            data[i * sizeM + j] -= mean[j];

    // kernel 3
    for (i = 0; i < sizeM; i++)
        for (j = i; j < sizeM; j++) {
            cov[i * sizeM + j] = 0.0;
            for (k = 0; k < sizeN; k++)
                cov[i * sizeM + j] += data[k * sizeM + i] * data[k * sizeM + j];
            cov[i * sizeM + j] /= (float_n - 1.0);
            cov[j * sizeM + i] = cov[i * sizeM + j];
        }
}

#endif // RUN_SIMPLE