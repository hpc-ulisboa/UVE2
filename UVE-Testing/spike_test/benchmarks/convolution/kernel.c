#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
// #define v_len 8 //inner loop is has lenght, might not be worth it
// vectorization in k and j is possible?
// losing performance if cfg.vec is not filling the vectors?
#ifdef D_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y,x) stream load
        "ss.sta.ld.d           u7, %[src], %[wnm2], %[hn] \t\n"
        "ss.app                u7, %[one], %[hnm2], %[one] \t\n"
        "ss.app                u7, %[one], %[fsize], %[onen] \t\n"
        "ss.end                u7, %[hn], %[fsize], %[hnn] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // filter(k,j) stream load
        "ss.sta.ld.d           u6, %[filter], %[wnm2], zero \t\n"
        "ss.app                u6, zero, %[hnm2], zero \t\n"
        "ss.app                u6, zero, %[fsize], %[one] \t\n"
        "ss.end                u6, zero, %[fsize], %[fsize] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // dst(y,x) stream scalar store
        "ss.sta.st.d           u3, %[dst], %[wnm2], %[hn] \t\n"
        "ss.end                u3, %[one], %[hnm2], %[one] \t\n"

        ".SLOOP_1%=: \t\n"

            "so.v.dp.d u4, zero, p0\t\n"

            ".SLOOP_1_0_0%=: \t\n"
                "so.a.mul.fp  u5, u6, u7, p0\t\n"
                "so.a.add.fp  u4, u4, u5, p0\t\n"
            "so.b.ndc.2 u6, .SLOOP_1_0_0%=\t\n"

            "so.a.adde.fp  u3, u4, p0\t\n"

        "so.b.nc u3, .SLOOP_1%= \t\n"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src] "r"(src + PB_H), [dst] "r"(dst + PB_H), [filter] "r"(filter),
        [hn] "r"(PB_H), [hnm2] "r"(PB_H - 2), [wnm2] "r"(PB_W - 2), [fsize] "r"(3),
        [one] "r"(1), [fsizen] "r"(-3), [hnn] "r"(-PB_H), [onen] "r"(-1));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y,x) stream load
        "ss.sta.ld.w           u7, %[src], %[wnm2], %[hn] \t\n"
        "ss.app                u7, %[one], %[hnm2], %[one] \t\n"
        "ss.app                u7, %[one], %[fsize], %[onen] \t\n"
        "ss.end                u7, %[hn], %[fsize], %[hnn] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // filter(k,j) stream load
        "ss.sta.ld.w           u6, %[filter], %[wnm2], zero \t\n"
        "ss.app                u6, zero, %[hnm2], zero \t\n"
        "ss.app                u6, zero, %[fsize], %[one] \t\n"
        "ss.end                u6, zero, %[fsize], %[fsize] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // dst(y,x) stream scalar store
        "ss.sta.st.w           u3, %[dst], %[wnm2], %[hn] \t\n"
        "ss.end                u3, %[one], %[hnm2], %[one] \t\n"

        ".SLOOP_1%=: \t\n"

            "so.v.dp.w u4, zero, p0\t\n"

            ".SLOOP_1_0_0%=: \t\n"
                "so.a.mul.fp  u5, u6, u7, p0\t\n"
                "so.a.add.fp  u4, u4, u5, p0\t\n"
            "so.b.ndc.2 u6, .SLOOP_1_0_0%=\t\n"

            "so.a.adde.fp  u3, u4, p0\t\n"

        "so.b.nc u3, .SLOOP_1%= \t\n"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src] "r"(src + PB_H), [dst] "r"(dst + PB_H), [filter] "r"(filter),
        [hn] "r"(PB_H), [hnm2] "r"(PB_H - 2), [wnm2] "r"(PB_W - 2), [fsize] "r"(3),
        [one] "r"(1), [fsizen] "r"(-3), [hnn] "r"(-PB_H), [onen] "r"(-1));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y,x) stream load
        "ss.sta.ld.w           u7, %[src], %[wnm2], %[hn] \t\n"
        "ss.app                u7, %[one], %[hnm2], %[one] \t\n"
        "ss.app                u7, %[one], %[fsize], %[onen] \t\n"
        "ss.end                u7, %[hn], %[fsize], %[hnn] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // filter(k,j) stream load
        "ss.sta.ld.w           u6, %[filter], %[wnm2], zero \t\n"
        "ss.app                u6, zero, %[hnm2], zero \t\n"
        "ss.app                u6, zero, %[fsize], %[one] \t\n"
        "ss.end                u6, zero, %[fsize], %[fsize] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // dst(y,x) stream scalar store
        "ss.sta.st.w           u3, %[dst], %[wnm2], %[hn] \t\n"
        "ss.end                u3, %[one], %[hnm2], %[one] \t\n"

        ".SLOOP_1%=: \t\n"

            "so.v.dp.w u4, zero, p0\t\n"

            ".SLOOP_1_0_0%=: \t\n"
                "so.a.mul.sg  u5, u6, u7, p0\t\n"
                "so.a.add.sg  u4, u4, u5, p0\t\n"
            "so.b.ndc.2 u6, .SLOOP_1_0_0%=\t\n"

            "so.a.adde.sg  u3, u4, p0\t\n"

        "so.b.nc u3, .SLOOP_1%= \t\n"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src] "r"(src + PB_H), [dst] "r"(dst + PB_H), [filter] "r"(filter),
        [hn] "r"(PB_H), [hnm2] "r"(PB_H - 2), [wnm2] "r"(PB_W - 2), [fsize] "r"(3),
        [one] "r"(1), [fsizen] "r"(-3), [hnn] "r"(-PB_H), [onen] "r"(-1));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y,x) stream load
        "ss.sta.ld.h           u7, %[src], %[wnm2], %[hn] \t\n"
        "ss.app                u7, %[one], %[hnm2], %[one] \t\n"
        "ss.app                u7, %[one], %[fsize], %[onen] \t\n"
        "ss.end                u7, %[hn], %[fsize], %[hnn] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // filter(k,j) stream load
        "ss.sta.ld.h           u6, %[filter], %[wnm2], zero \t\n"
        "ss.app                u6, zero, %[hnm2], zero \t\n"
        "ss.app                u6, zero, %[fsize], %[one] \t\n"
        "ss.end                u6, zero, %[fsize], %[fsize] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // dst(y,x) stream scalar store
        "ss.sta.st.h           u3, %[dst], %[wnm2], %[hn] \t\n"
        "ss.end                u3, %[one], %[hnm2], %[one] \t\n"

        ".SLOOP_1%=: \t\n"

            "so.v.dp.h u4, zero, p0\t\n"

            ".SLOOP_1_0_0%=: \t\n"
                "so.a.mul.sg  u5, u6, u7, p0\t\n"
                "so.a.add.sg  u4, u4, u5, p0\t\n"
            "so.b.ndc.2 u6, .SLOOP_1_0_0%=\t\n"

            "so.a.adde.sg  u3, u4, p0\t\n"

        "so.b.nc u3, .SLOOP_1%= \t\n"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src] "r"(src + PB_H), [dst] "r"(dst + PB_H), [filter] "r"(filter),
        [hn] "r"(PB_H), [hnm2] "r"(PB_H - 2), [wnm2] "r"(PB_W - 2), [fsize] "r"(3),
        [one] "r"(1), [fsizen] "r"(-3), [hnn] "r"(-PB_H), [onen] "r"(-1));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y,x) stream load
        "ss.sta.ld.b           u7, %[src], %[wnm2], %[hn] \t\n"
        "ss.app                u7, %[one], %[hnm2], %[one] \t\n"
        "ss.app                u7, %[one], %[fsize], %[onen] \t\n"
        "ss.end                u7, %[hn], %[fsize], %[hnn] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // filter(k,j) stream load
        "ss.sta.ld.b           u6, %[filter], %[wnm2], zero \t\n"
        "ss.app                u6, zero, %[hnm2], zero \t\n"
        "ss.app                u6, zero, %[fsize], %[one] \t\n"
        "ss.end                u6, zero, %[fsize], %[fsize] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // dst(y,x) stream scalar store
        "ss.sta.st.b           u3, %[dst], %[wnm2], %[hn] \t\n"
        "ss.end                u3, %[one], %[hnm2], %[one] \t\n"

        ".SLOOP_1%=: \t\n"

            "so.v.dp.b u4, zero, p0\t\n"

            ".SLOOP_1_0_0%=: \t\n"
                "so.a.mul.sg  u5, u6, u7, p0\t\n"
                "so.a.add.sg  u4, u4, u5, p0\t\n"
            "so.b.ndc.2 u6, .SLOOP_1_0_0%=\t\n"

            "so.a.adde.sg  u3, u4, p0\t\n"

        "so.b.nc u3, .SLOOP_1%= \t\n"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src] "r"(src + PB_H), [dst] "r"(dst + PB_H), [filter] "r"(filter),
        [hn] "r"(PB_H), [hnm2] "r"(PB_H - 2), [wnm2] "r"(PB_W - 2), [fsize] "r"(3),
        [one] "r"(1), [fsizen] "r"(-3), [hnn] "r"(-PB_H), [onen] "r"(-1));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *src, DataType *dst, DataType *filter, int PB_H, int PB_W){
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

    DataType sum;
    int y, x, k, j;
    for (y = 1; y < PB_W - 1; y++){
        for (x = 1; x < PB_H - 1; x++){
            sum = 0.0;
            for (k = -1; k <= 1; k++){
                for (j = -1; j <= 1; j++)
                    sum = sum + filter[(j + 1) * 3 + k + 1] * src[(y - j) * PB_W + (x - k)];
            }
            dst[y * PB_W + x] = sum;
        }
    }

    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE
