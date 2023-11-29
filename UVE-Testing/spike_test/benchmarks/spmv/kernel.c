#include "Functions.h"

#ifdef RUN_UVE
void core(void *val, void *cols, void *rowDelimiters, void *vec, void *out, uint64_t N) {
    asm volatile(
        // rowDelimiters stream (!!! this one should not be vectorial)
        "ss.ld.w                  u3, %[rowDelimiters], %[sn], %[one] \t\n" // D1: linear access
        "ss.ld.w                  u7, %[rowDelimiters], %[sn], %[one] \t\n" // D1: linear access

        // cols stream
        "ss.sta.ld.w              u2, %[cols], %[sn], %[sn] \t\n"    // D1: linear access size 'unknown'
        "ss.app.indl.siz.set      u2, u3 \t\n"                       // Indirection from stream u1 -> modifout size
        "ss.end                   u2, %[zero], %[zero], %[one] \t\n" // D2: new line stride N
        "ss.cfg.vec               u2 \t\n"

        // val stream
        "ss.sta.ld.w              u1, %[val], %[sn], %[sn] \t\n"     // D1: linear access size 'unknown'
        "ss.app.indl.siz.set      u1, u7 \t\n"                       // Indirection from stream u1 -> modifout size
        "ss.end                   u1, %[zero], %[zero], %[one] \t\n" // D2: new line stride N
        "ss.cfg.vec               u1 \t\n"

        // vec stream
        "ss.sta.ld.w              u4, %[vec], %[one], %[zero] \t\n"   // Dummy D2
        //"ss.app.ind.sca.ofs.add   u4, u2 \t\n"                      // Indirection from stream u2 -> add to base address
        "ss.app.ind.ofs.add.1     u4, u2 \t\n"                        // to be done with new insn above
        "ss.end                   u4, %[zero], %[zero], %[zero] \t\n" // D1: new line stride N
        "ss.cfg.vec               u4 \t\n"

        // out stream load
        "ss.ld.w              u5, %[out], %[sn], %[one] \t\n" // D1: linear access size N

        // out stream store
        "ss.st.w              u6, %[out], %[sn], %[one] \t\n" // D1: linear access size N

        :
        : [val] "r"(val), [cols] "r"(cols), [rowDelimiters] "r"(rowDelimiters), [vec] "r"(vec), [out] "r"(out),
          [sn] "r"(N), [zero] "r"(0), [one] "r"(1));

    asm volatile(
        ".iLoop1%=: \t\n"
            "so.v.dp.w u8, zero, p0 \t\n"

            ".jloop%=: \t\n"
                "so.a.mul.fp u9, u1, u4, p0\n\t"
                "so.a.add.fp u8, u8, u9, p0\n\t"
            "so.b.ndc.1 u2, .jloop%= \n\t"

            "so.a.adde.fp u10, u8, p0 \n\t"
            "so.a.add.fp  u6, u5, u10, p0\n\t"
        "so.b.nc	u2, .iLoop1%= \n\t"::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *val, uint32_t *cols, uint32_t *rowDelimiters, DataType *vec, DataType *out, uint64_t N) {
    DataType t;
    int cur_nnz;

    for (int i = 0; i < N; i++) {
        t = 0.0;
        cur_nnz = rowDelimiters[i];

        for (int j = 0; j < cur_nnz; j++) {
            t += val[j] * vec[cols[j]];
        }
        out[i] += t;
    }
}
#endif // RUN_SIMPLE