#include "Functions.h"

#ifdef RUN_UVE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    asm volatile(
        "ss.sta.ld.w     u1, %[cols], %[N], %[L] \t\n"
        "ss.end          u1, zero, %[L], %[one] \t\n"
        //"ss.cfg.ind      u1 \t\n"

        "ss.sta.ld.d     u2, %[nzval], %[N], %[L] \t\n"
        "ss.end          u2, zero, %[L], %[one] \t\n"
        "ss.cfg.vec      u2 \t\n"

        "ss.sta.ld.d            u3, %[vec], %[N], zero \t\n"
        "ss.app		            u3, zero, %[L], zero \t\n"
        "ss.app.indl.ofs.add    u3, u1 \t\n"
        "ss.cfg.vec             u3 \t\n"
        "ss.end                 u3, zero, %[one], zero \t\n"

        "ss.st.d  u4, %[out], %[N], %[one] \t\n"

        ::[cols] "r"(cols),
        [nzval] "r"(nzval),
        [vec] "r"(vec),
        [out] "r"(out),
        [N] "r"(N),
        [L] "r"(L),
        [one] "r"(1));

    asm volatile(
        ".iLoop1%=: \t\n"
            "so.v.dp.d u5, zero, p0 \t\n"

            ".kloop1%=: \t\n"
                //"so.a.mac.fp u5, u1, u2, p0\n\t"
                "so.a.mul.fp u6, u2, u3, p0\n\t"
                "so.a.add.fp u5, u5, u6, p0\n\t"
            "so.b.ndc.1 u2, .kloop1%= \n\t"

            "so.a.adde.fp  u4, u5, p0 \n\t"
        "so.b.nc	u2, .iLoop1%= \n\t" :::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    int i, j;
    DataType Si;

    for (i = 0; i < N; i++) {
        DataType sum = out[i];
        for (j = 0; j < L; j++) {
            Si = nzval[i * L + j] * vec[cols[i * L + j]];
            //printf("(nzval[%d] = %f  *  vec[%d] = %f) = (Si = %f)\n", i*L+j, nzval[i * L + j], cols[i*L+j], vec[cols[i * L + j]], Si);
            sum += Si;
        }
        out[i] = sum;
    }
}
#endif // RUN_SIMPLE

#ifdef RUN_BLANK
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
}
#endif // RUN_BLANK