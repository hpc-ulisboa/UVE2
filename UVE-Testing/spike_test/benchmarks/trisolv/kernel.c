#include "Functions.h"
#ifdef RUN_UVE
#define v_len 8
void core(void *src1, void *src2, void *src3) {
    asm volatile(/*offset, size, stride*/ /*mod-> size, disp*/

                 // L(i,j) stream load
                 "ss.sta.ld.d           u1, %[src1], zero, %[one] \t\n" // D1: linear access (initial size: 0)
                 "ss.cfg.vec            u1 \t\n"                        // D1: configure as vector binded
                 "ss.app                u1, zero, %[sn], %[sn] \t\n"  // D2: slide verticaly stride N access size N-1
                 "ss.end.mod.siz.inc    u1, %[snm1], %[one] \t\n"       // Modifier->D1: increment D1 size N-1

                 // x(j) stream load
                 "ss.sta.ld.d           u2, %[src3], zero, %[one] \t\n" // D1: vector - linear access (initial size: 0)
                 "ss.cfg.vec            u2 \t\n"                        // D1: configure as vector binded
                 "ss.app                u2, zero, %[sn], zero \t\n"   // D2: Repeat N-1 times [dummy dimension]
                 "ss.end.mod.siz.inc    u2, %[snm1], %[one] \t\n"       // Modifier->D1: increment D1 size N-1

                 // b stream scalar load (?)
                 "ss.sta.ld.d           u3, %[src2], %[vl], zero \t\n"  // D1: scalar access
                 "ss.end                u3, zero, %[sn], %[one] \t\n" // D2: vector - linear access

                 // L(i,i) stream scalar load (?)
                 "ss.sta.ld.d               u4, %[src1], %[vl], zero \t\n"   // D1: scalar access
                 "ss.end                    u4, zero, %[sn], %[snp1] \t\n" // D2: diagonal access size N  

                 // x stream scalar store (?)
                 "ss.st.d               u5, %[src3], %[sn], %[one] \t\n" // D1: vector - linear access

                 :
                 : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
                   [sn] "r"(SIZE), [snm1] "r"(SIZE - 1), [snp1] "r"(SIZE + 1), [one] "r"(1),
                   [vl] "r"(v_len)
    );

    asm volatile(
        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

            "so.v.dp.d  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.fp u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.fp u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.1 u1, .jloop1%= \n\t"

            "so.a.adde.fp   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.fp    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.fp    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 
        :::
    );

}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *L, DataType *b, DataType *x) {
    for (int i = 0; i < SIZE; ++i) {
        x[i] = b[i];
        for (int j = 0; j < i; ++j) {
            //printf("read x[%d] = %f\n", i, x[i]);
            //printf("x[%d] -= [(L[%d][%d]=%f) * (x[%d]=%f)]=%f\n", i, i, j, L[i*SIZE+j], j, x[j], L[i*SIZE+j] * x[j]);
            x[i] -= L[i * SIZE + j] * x[j];
        }
        //printf("x[%d] = [(x[%d]=%f) / (L[%d][%d]=%f)] = %f\n", i, i, x[i], i, i, L[i*SIZE+i], x[i]/L[i*SIZE+i]);
        x[i] = x[i] / L[i * SIZE + i];
        //printf("store x[%d] = %f\n", i, x[i]);
    }
}
#endif // RUN_SIMPLE
