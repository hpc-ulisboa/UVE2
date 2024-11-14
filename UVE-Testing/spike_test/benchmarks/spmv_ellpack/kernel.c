#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(

        "ss.sta.ld.w.inds u1, %[cols] \n"
        "ss.app           u1, zero, %[N], %[L] \n"
        "ss.end           u1, zero, %[L], %[one] \n"

        "ss.sta.ld.d.v.m   u2, %[nzval] \n"
        "ss.app            u2, zero, %[N], %[L] \n"
        "ss.end            u2, zero, %[L], %[one] \n"

        //"ss.sta.ld.d.v.2.m    u3, %[vec] \n"
        "ss.sta.ld.d.v.m    u3, %[vec] \n"
        "ss.app               u3, zero, %[N], zero \n"
        "ss.app		          u3, zero, %[L], zero \n"
        //"ss.app.ind.ofs.add.3 u3, u1 \n"
        //"ss.end               u3, zero, %[one], zero \n"
        "ss.end.sgi.ofs.add   u3, u1 \n"

        "ss.sta.st.d  u4, %[out] \n"
        "ss.end       u4, zero, %[N], %[one] \n"

        ".iLoop1%=: \n"
            "so.v.dp.d u5, zero, p0 \n"

            ".kloop1%=: \n"
                "so.a.mac.fp u5, u2, u3, p0 \n"
                //"so.a.mul.fp u6, u2, u3, p0 \n"
                //"so.a.add.fp u5, u5, u6, p0 \n"
            "so.b.ndc.2 u2, .kloop1%= \n"

            "so.a.adde.fp  u4, u5, p0 \n"
        "so.b.nc	u2, .iLoop1%= \n"

        :: [cols] "r"(cols),
          [nzval] "r"(nzval),
          [vec] "r"(vec),
          [out] "r"(out),
          [N] "r"(N),
          [L] "r"(L),
          [one] "r"(1));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out, int32_t N, int32_t L) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    for (int i = 0; i < N; i++) 
        for (int j = 0; j < L; j++) 
            out[i] += nzval[i * L + j] * vec[cols[i * L + j]];

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE
