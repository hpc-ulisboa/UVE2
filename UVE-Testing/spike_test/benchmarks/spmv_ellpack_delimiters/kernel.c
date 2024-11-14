#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
void core(void *val, void *cols, void *rowDelimiters, void *vec, void *out, uint64_t N, uint64_t K) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(

        // rowDelimiters stream (!!! this one should not be vectorial)
        "ss.sta.ld.w.inds  u3, %[rowDelimiters] \n"
        "ss.end            u3, zero, %[sn], %[one] \n" // D1: linear access

        "ss.sta.ld.w.inds  u7, %[rowDelimiters] \n"
        "ss.end            u7, zero, %[sn], %[one] \n" // D1: linear access

        "ss.sta.ld.w.inds  u10, %[rowDelimiters] \n"
        "ss.end            u10, zero, %[sn], %[one] \n" // D1: linear access

        // val stream
        "ss.sta.ld.d.v.m       u1, %[val] \n"
        "ss.app                u1, zero, %[sn], %[sk] \n" // D1
        "ss.app.ind.siz.set.2  u1, u7 \n"                 // Indirection from stream u7 -> modify size
        "ss.end                u1, zero, zero, %[one] \n" // D2: new line stride 1

        // cols stream
        //"ss.sta.ld.w.v.1       u2, %[cols] \n"
        "ss.sta.ld.w.inds      u2, %[cols] \n"
        "ss.app                u2, zero, %[sn], %[sk] \n" // D1
        "ss.app.ind.siz.set.2  u2, u3 \n"                 // Indirection from stream u3 -> modify size
        "ss.end                u2, zero, zero, %[one] \n" // D2: new line stride 1

        // vec stream
        "ss.sta.ld.d.v.m       u4, %[vec] \n"
        "ss.app                u4, zero, %[sn], zero \n" // D1
        "ss.app.ind.siz.set.2  u4, u10 \n"               // Indirection from stream u10(=u3) -> modify size
        "ss.app                u4, zero, zero, zero \n"  // D2: new line 
        "ss.end.sgi.ofs.add    u4, u2 \n"                // SG Indirection from stream u2 -> add to base address

        // out stream store
        "ss.sta.st.d  u5, %[out] \n"
        "ss.end       u5, zero, %[sn], %[one] \n" // D1: linear access size N

        ".iLoop1%=: \n"
            "so.v.dp.d u8, zero, p0 \n"

            ".jloop%=: \n"
                /*"so.a.mul.fp u9, u1, u4, p0 \n"
                "so.a.add.fp u8, u8, u9, p0 \n"*/
                "so.a.mac.fp u8, u1, u4, p0 \n"
            "so.b.ndc.2 u1, .jloop%= \n"

            "so.a.adde.fp u5, u8, p0 \n"
        "so.b.nc	u1, .iLoop1%= \n"

        :: [val] "r"(val), [cols] "r"(cols), [rowDelimiters] "r"(rowDelimiters), [vec] "r"(vec), [out] "r"(out),
          [sn] "r"(N), [sk] "r"(K), [zero] "r"(0), [one] "r"(1));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *val, uint32_t *cols, uint32_t *rowDelimiters, DataType *vec, DataType *out, uint64_t N, uint64_t K) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    int cur_nnz;

    for (int i = 0; i < N; i++) {
        cur_nnz = rowDelimiters[i];
        for (int j = 0; j < cur_nnz; j++)
            out[i] += val[i * K + j] * vec[cols[i * K + j]];
    }

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

