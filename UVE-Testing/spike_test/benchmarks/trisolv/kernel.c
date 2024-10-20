#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(void *src1, void *src2, void *src3) {
    asm volatile(
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // L(i,j) stream load
        "ss.sta.ld.d.v         u1, %[src1] \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"    // D2: slide verticaly stride N access size N-1
        "ss.app.mod.siz.inc.2  u1, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u1, zero, zero, %[one] \t\n"    // D1: linear access (initial size: 0)

        // x(j) stream load
        "ss.sta.ld.d.v         u2, %[src3] \t\n"
        "ss.app                u2, zero, %[sn], zero \t\n"     // D2: Repeat N-1 times [dummy dimension]
        "ss.app.mod.siz.inc.2  u2, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u2, zero, zero, %[one] \t\n"    // D1: vector - linear access (initial size: 0)

        // b stream scalar load (?)
        "ss.sta.ld.d           u3, %[src2] \t\n"
        "ss.end                u3, zero, %[sn], %[one] \t\n"  // D1: scalar access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.d           u4, %[src1] \t\n"
        "ss.end                u4, zero, %[sn], %[snp1] \t\n"   // D1: scalar access

        // x stream scalar store (?)
        "ss.sta.st.d           u5, %[src3] \t\n"
        "ss.end                u5, zero, %[sn], %[one] \t\n" // D1: vector - linear access

        ".fLoop1%=: \t\n"

            "so.v.dp.d  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.fp u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.fp u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.2 u1, .jloop1%= \n\t"

            "so.a.adde.fp   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.fp    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.fp    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snp1] "r"(SIZE + 1), [one] "r"(1)
    );

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(void *src1, void *src2, void *src3) {
    asm volatile(
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // L(i,j) stream load
        "ss.sta.ld.w.v         u1, %[src1] \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"    // D2: slide verticaly stride N access size N-1
        "ss.app.mod.siz.inc.2  u1, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u1, zero, zero, %[one] \t\n"    // D1: linear access (initial size: 0)

        // x(j) stream load
        "ss.sta.ld.w.v         u2, %[src3] \t\n"
        "ss.app                u2, zero, %[sn], zero \t\n"     // D2: Repeat N-1 times [dummy dimension]
        "ss.app.mod.siz.inc.2  u2, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u2, zero, zero, %[one] \t\n"    // D1: vector - linear access (initial size: 0)

        // b stream scalar load (?)
        "ss.sta.ld.w           u3, %[src2] \t\n"
        "ss.end                u3, zero, %[sn], %[one] \t\n"    // D1: scalar access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.w           u4, %[src1] \t\n"
        "ss.end                u4, zero, %[sn], %[snp1] \t\n"   // D1: scalar access

        // x stream scalar store (?)
        "ss.sta.st.w           u5, %[src3] \t\n"
        "ss.end                u5, zero, %[sn], %[one] \t\n" // D1: vector - linear access


        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

            "so.v.dp.w  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.fp u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.fp u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.2 u1, .jloop1%= \n\t"

            "so.a.adde.fp   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.fp    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.fp    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snp1] "r"(SIZE + 1), [one] "r"(1)
    );

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(void *src1, void *src2, void *src3) {
    asm volatile(
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // L(i,j) stream load
        "ss.sta.ld.w.v         u1, %[src1] \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"    // D2: slide verticaly stride N access size N-1
        "ss.app.mod.siz.inc.2  u1, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u1, zero, zero, %[one] \t\n"    // D1: linear access (initial size: 0)

        // x(j) stream load
        "ss.sta.ld.w.v         u2, %[src3] \t\n"
        "ss.app                u2, zero, %[sn], zero \t\n"     // D2: Repeat N-1 times [dummy dimension]
        "ss.app.mod.siz.inc.2  u2, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u2, zero, zero, %[one] \t\n"    // D1: vector - linear access (initial size: 0)

        // b stream scalar load (?)
        "ss.sta.ld.w           u3, %[src2] \t\n"
        "ss.end                u3, zero, %[sn], %[one] \t\n"   // D1: scalar access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.w           u4, %[src1] \t\n"
        "ss.end                u4, zero, %[sn], %[snp1] \t\n"   // D1: scalar access

        // x stream scalar store (?)
        "ss.sta.st.w           u5, %[src3] \t\n"
        "ss.end                u5, zero, %[sn], %[one] \t\n" // D1: vector - linear access


        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

            "so.v.dp.w  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.sg u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.sg u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.2 u1, .jloop1%= \n\t"

            "so.a.adde.sg   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.sg    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.sg    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snp1] "r"(SIZE + 1), [one] "r"(1)
    );

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(void *src1, void *src2, void *src3) {
    asm volatile(
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // L(i,j) stream load
        "ss.sta.ld.h.v         u1, %[src1] \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"    // D2: slide verticaly stride N access size N-1
        "ss.app.mod.siz.inc.2  u1, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u1, zero, zero, %[one] \t\n"    // D1: linear access (initial size: 0)

        // x(j) stream load
        "ss.sta.ld.h.v         u2, %[src3] \t\n"
        "ss.app                u2, zero, %[sn], zero \t\n"     // D2: Repeat N-1 times [dummy dimension]
        "ss.app.mod.siz.inc.2  u2, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u2, zero, zero, %[one] \t\n"    // D1: vector - linear access (initial size: 0)

        // b stream scalar load (?)
        "ss.sta.ld.h           u3, %[src2] \t\n"
        "ss.end                u3, zero, %[sn], %[one] \t\n"   // D1: scalar access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.h           u4, %[src1] \t\n"
        "ss.end                u4, zero, %[sn], %[snp1] \t\n"   // D1: scalar access

        // x stream scalar store (?)
        "ss.sta.st.h           u5, %[src3] \t\n"
        "ss.end                u5, zero, %[sn], %[one] \t\n" // D1: vector - linear access


        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

            "so.v.dp.h  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.sg u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.sg u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.2 u1, .jloop1%= \n\t"

            "so.a.adde.sg   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.sg    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.sg    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snp1] "r"(SIZE + 1), [one] "r"(1)
    );

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(void *src1, void *src2, void *src3) {
    asm volatile(
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // L(i,j) stream load
        "ss.sta.ld.b.v         u1, %[src1] \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"    // D2: slide verticaly stride N access size N-1
        "ss.app.mod.siz.inc.2  u1, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u1, zero, zero, %[one] \t\n"    // D1: linear access (initial size: 0)

        // x(j) stream load
        "ss.sta.ld.b.v         u2, %[src3] \t\n"
        "ss.app                u2, zero, %[sn], zero \t\n"     // D2: Repeat N-1 times [dummy dimension]
        "ss.app.mod.siz.inc.2  u2, %[one] \t\n"                // modifier_t->D1: increment D1 size N-1
        "ss.end                u2, zero, zero, %[one] \t\n"    // D1: vector - linear access (initial size: 0)

        // b stream scalar load (?)
        "ss.sta.ld.b           u3, %[src2] \t\n"
        "ss.end                u3, zero, %[sn], %[one] \t\n"   // D1: scalar access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.b           u4, %[src1] \t\n"
        "ss.end                u4, zero, %[sn], %[snp1] \t\n"   // D1: scalar access

        // x stream scalar store (?)
        "ss.sta.st.b           u5, %[src3] \t\n"
        "ss.end                u5, zero, %[sn], %[one] \t\n" // D1: vector - linear access


        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

            "so.v.dp.b  u6, zero, p0\n\t" // u6 = 0

            ".jloop1%= : \t\n"
                "so.a.mul.sg u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
                "so.a.sub.sg u6, u6, u7, p0\n\t" // u6 = u6 - u7
            "so.b.ndc.2 u1, .jloop1%= \n\t"

            "so.a.adde.sg   u7, u6, p0      \n\t" // reduce vector
            "so.a.add.sg    u7, u3, u7, p0  \n\t" //  t = b + red
            "so.a.div.sg    u5, u7, u4, p0  \n\t" //  x = t / L

        "so.b.nc	u1, .fLoop1%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snp1] "r"(SIZE + 1), [one] "r"(1)
    );

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *L, DataType *b, DataType *x) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
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
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE