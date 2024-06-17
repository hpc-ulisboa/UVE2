#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.d           u1, %[src_0], %[inm2], %[jn] \t\n"
        "ss.end                u1, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        // src(y-1,x) stream load
        "ss.sta.ld.d           u2, %[src_1], %[inm2], %[jn] \t\n"
        "ss.end                u2, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u2 \t\n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.d           u3, %[src_2], %[inm2], %[jn] \t\n"
        "ss.end                u3, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u3 \t\n"

        // src(y,x-1) stream load
        "ss.sta.ld.d           u4, %[src_3], %[inm2], %[jn] \t\n"
        "ss.end                u4, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u4 \t\n"

        // src(y,x) stream load
        "ss.sta.ld.d           u5, %[src_4], %[inm2], %[jn] \t\n"
        "ss.end                u5, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u5 \t\n"

        // src(y,x+1) stream load
        "ss.sta.ld.d           u6, %[src_5], %[inm2], %[jn] \t\n"
        "ss.end                u6, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.d           u7, %[src_6], %[inm2], %[jn] \t\n"
        "ss.end                u7, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // src(y+1,x) stream load
        "ss.sta.ld.d           u8, %[src_7], %[inm2], %[jn] \t\n"
        "ss.end                u8, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u8 \t\n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.d           u9, %[src_8], %[inm2], %[jn] \t\n"
        "ss.end                u9, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u9 \t\n"

        /* filter(k,j) stream load
        "ss.sta.ld.d           u10, %[filter], %[inm2], zero \t\n"
        "ss.app                u10, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u10 \t\n"
        "ss.end                u10, zero, %[fsize], zero \t\n"*/

        // filter
        "so.v.dp.d  u10, %[filter0], p0\t\n"
        "so.v.dp.d  u11, %[filter1], p0\t\n"
        "so.v.dp.d  u12, %[filter2], p0\t\n"
        "so.v.dp.d  u13, %[filter3], p0\t\n"
        "so.v.dp.d  u14, %[filter4], p0\t\n"
        "so.v.dp.d  u15, %[filter5], p0\t\n"
        "so.v.dp.d  u16, %[filter6], p0\t\n"
        "so.v.dp.d  u17, %[filter7], p0\t\n"
        "so.v.dp.d  u18, %[filter8], p0\t\n"


        // dst(y,x) stream store
        "ss.sta.st.d           u0, %[dst], %[inm2], %[jn] \t\n"
        "ss.end                u0, %[one], %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u0 \t\n"

        ".loop_y%=: \t\n"
        
            "so.a.mul.fp u19, u10, u9, p0\n\t" // filter[0] * src[(y + 1)][(x + 1)]
            "so.a.mul.fp u20, u11, u8, p0\n\t" // filter[1] * src[(y + 1)][(x)]
            "so.a.mul.fp u21, u12, u7, p0\n\t" // filter[2] * src[(y + 1)][(x - 1)]
            "so.a.mul.fp u22, u13, u6, p0\n\t" // filter[3] * src[(y)][(x + 1)]
            "so.a.mul.fp u23, u14, u5, p0\n\t" // filter[4] * src[(y)][(x)]
            "so.a.mul.fp u24, u15, u4, p0\n\t" // filter[5] * src[(y)][(x - 1)]
            "so.a.mul.fp u25, u16, u3, p0\n\t" // filter[6] * src[(y - 1)][(x + 1)]
            "so.a.mul.fp u26, u17, u2, p0\n\t" // filter[7] * src[(y - 1)][(x)]
            "so.a.mul.fp u27, u18, u1, p0\n\t" // filter[8] * src[(y - 1)][(x - 1)]

            "so.a.add.fp u28, u19, u20, p0\n\t" 
            "so.a.add.fp u29, u21, u22, p0\n\t"
            "so.a.add.fp u30, u23, u24, p0\n\t"
            "so.a.add.fp u31, u25, u26, p0\n\t"

            "so.a.add.fp u29, u29, u28, p0\n\t"
            "so.a.add.fp u31, u31, u30, p0\n\t"
            "so.a.add.fp u31, u31, u29, p0\n\t"

            "so.a.add.fp u0, u31, u27, p0\n\t"

            "so.b.nc	u0, .loop_y%= \n\t"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src_0] "r" (src),         [src_1] "r" (src+1),           [src_2] "r" (src+2),
        [src_3] "r" (src+PB_J),      [src_4] "r" (src+PB_J+1),      [src_5] "r" (src+PB_J+2),
        [src_6] "r" (src+PB_J+PB_J), [src_7] "r" (src+PB_J+PB_J+1), [src_8] "r" (src+PB_J+PB_J+2),
        [dst] "r"(dst + PB_J), [filter] "r"(filter), 	
        [in] "r"(PB_I), [jn] "r"(PB_J), [jnm2] "r"(PB_J - 2), [inm2] "r"(PB_I - 2), [fsize] "r"(3),
        [one] "r"(1),
        [filter0] "r"(filter[0]), [filter1] "r"(filter[1]), [filter2] "r"(filter[2]),
        [filter3] "r"(filter[3]), [filter4] "r"(filter[4]), [filter5] "r"(filter[5]),
        [filter6] "r"(filter[6]), [filter7] "r"(filter[7]), [filter8] "r"(filter[8]));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.w           u1, %[src_0], %[inm2], %[jn] \t\n"
        "ss.end                u1, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        // src(y-1,x) stream load
        "ss.sta.ld.w           u2, %[src_1], %[inm2], %[jn] \t\n"
        "ss.end                u2, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u2 \t\n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.w           u3, %[src_2], %[inm2], %[jn] \t\n"
        "ss.end                u3, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u3 \t\n"

        // src(y,x-1) stream load
        "ss.sta.ld.w           u4, %[src_3], %[inm2], %[jn] \t\n"
        "ss.end                u4, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u4 \t\n"

        // src(y,x) stream load
        "ss.sta.ld.w           u5, %[src_4], %[inm2], %[jn] \t\n"
        "ss.end                u5, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u5 \t\n"

        // src(y,x+1) stream load
        "ss.sta.ld.w           u6, %[src_5], %[inm2], %[jn] \t\n"
        "ss.end                u6, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.w           u7, %[src_6], %[inm2], %[jn] \t\n"
        "ss.end                u7, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // src(y+1,x) stream load
        "ss.sta.ld.w           u8, %[src_7], %[inm2], %[jn] \t\n"
        "ss.end                u8, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u8 \t\n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.w           u9, %[src_8], %[inm2], %[jn] \t\n"
        "ss.end                u9, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u9 \t\n"

        /* filter(k,j) stream load
        "ss.sta.ld.w           u10, %[filter], %[inm2], zero \t\n"
        "ss.app                u10, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u10 \t\n"
        "ss.end                u10, zero, %[fsize], zero \t\n"*/

        // filter
        "so.v.dp.w  u10, %[filter0], p0\t\n"
        "so.v.dp.w  u11, %[filter1], p0\t\n"
        "so.v.dp.w  u12, %[filter2], p0\t\n"
        "so.v.dp.w  u13, %[filter3], p0\t\n"
        "so.v.dp.w  u14, %[filter4], p0\t\n"
        "so.v.dp.w  u15, %[filter5], p0\t\n"
        "so.v.dp.w  u16, %[filter6], p0\t\n"
        "so.v.dp.w  u17, %[filter7], p0\t\n"
        "so.v.dp.w  u18, %[filter8], p0\t\n"


        // dst(y,x) stream store
        "ss.sta.st.w           u0, %[dst], %[inm2], %[jn] \t\n"
        "ss.end                u0, %[one], %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u0 \t\n"

        ".loop_y%=: \t\n"
        
            "so.a.mul.fp u19, u10, u9, p0\n\t" // filter[0] * src[(y + 1)][(x + 1)]
            "so.a.mul.fp u20, u11, u8, p0\n\t" // filter[1] * src[(y + 1)][(x)]
            "so.a.mul.fp u21, u12, u7, p0\n\t" // filter[2] * src[(y + 1)][(x - 1)]
            "so.a.mul.fp u22, u13, u6, p0\n\t" // filter[3] * src[(y)][(x + 1)]
            "so.a.mul.fp u23, u14, u5, p0\n\t" // filter[4] * src[(y)][(x)]
            "so.a.mul.fp u24, u15, u4, p0\n\t" // filter[5] * src[(y)][(x - 1)]
            "so.a.mul.fp u25, u16, u3, p0\n\t" // filter[6] * src[(y - 1)][(x + 1)]
            "so.a.mul.fp u26, u17, u2, p0\n\t" // filter[7] * src[(y - 1)][(x)]
            "so.a.mul.fp u27, u18, u1, p0\n\t" // filter[8] * src[(y - 1)][(x - 1)]

            "so.a.add.fp u28, u19, u20, p0\n\t" 
            "so.a.add.fp u29, u21, u22, p0\n\t"
            "so.a.add.fp u30, u23, u24, p0\n\t"
            "so.a.add.fp u31, u25, u26, p0\n\t"

            "so.a.add.fp u29, u29, u28, p0\n\t"
            "so.a.add.fp u31, u31, u30, p0\n\t"
            "so.a.add.fp u31, u31, u29, p0\n\t"

            "so.a.add.fp u0, u31, u27, p0\n\t"

            "so.b.nc	u0, .loop_y%= \n\t"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src_0] "r" (src),         [src_1] "r" (src+1),           [src_2] "r" (src+2),
        [src_3] "r" (src+PB_J),      [src_4] "r" (src+PB_J+1),      [src_5] "r" (src+PB_J+2),
        [src_6] "r" (src+PB_J+PB_J), [src_7] "r" (src+PB_J+PB_J+1), [src_8] "r" (src+PB_J+PB_J+2),
        [dst] "r"(dst + PB_J), [filter] "r"(filter), 	
        [in] "r"(PB_I), [jn] "r"(PB_J), [jnm2] "r"(PB_J - 2), [inm2] "r"(PB_I - 2), [fsize] "r"(3),
        [one] "r"(1),
        [filter0] "r"(filter[0]), [filter1] "r"(filter[1]), [filter2] "r"(filter[2]),
        [filter3] "r"(filter[3]), [filter4] "r"(filter[4]), [filter5] "r"(filter[5]),
        [filter6] "r"(filter[6]), [filter7] "r"(filter[7]), [filter8] "r"(filter[8]));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.w           u1, %[src_0], %[inm2], %[jn] \t\n"
        "ss.end                u1, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        // src(y-1,x) stream load
        "ss.sta.ld.w           u2, %[src_1], %[inm2], %[jn] \t\n"
        "ss.end                u2, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u2 \t\n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.w           u3, %[src_2], %[inm2], %[jn] \t\n"
        "ss.end                u3, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u3 \t\n"

        // src(y,x-1) stream load
        "ss.sta.ld.w           u4, %[src_3], %[inm2], %[jn] \t\n"
        "ss.end                u4, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u4 \t\n"

        // src(y,x) stream load
        "ss.sta.ld.w           u5, %[src_4], %[inm2], %[jn] \t\n"
        "ss.end                u5, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u5 \t\n"

        // src(y,x+1) stream load
        "ss.sta.ld.w           u6, %[src_5], %[inm2], %[jn] \t\n"
        "ss.end                u6, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.w           u7, %[src_6], %[inm2], %[jn] \t\n"
        "ss.end                u7, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // src(y+1,x) stream load
        "ss.sta.ld.w           u8, %[src_7], %[inm2], %[jn] \t\n"
        "ss.end                u8, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u8 \t\n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.w           u9, %[src_8], %[inm2], %[jn] \t\n"
        "ss.end                u9, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u9 \t\n"

        /* filter(k,j) stream load
        "ss.sta.ld.w           u10, %[filter], %[inm2], zero \t\n"
        "ss.app                u10, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u10 \t\n"
        "ss.end                u10, zero, %[fsize], zero \t\n"*/

        // filter
        "so.v.dp.w  u10, %[filter0], p0\t\n"
        "so.v.dp.w  u11, %[filter1], p0\t\n"
        "so.v.dp.w  u12, %[filter2], p0\t\n"
        "so.v.dp.w  u13, %[filter3], p0\t\n"
        "so.v.dp.w  u14, %[filter4], p0\t\n"
        "so.v.dp.w  u15, %[filter5], p0\t\n"
        "so.v.dp.w  u16, %[filter6], p0\t\n"
        "so.v.dp.w  u17, %[filter7], p0\t\n"
        "so.v.dp.w  u18, %[filter8], p0\t\n"


        // dst(y,x) stream store
        "ss.sta.st.w           u0, %[dst], %[inm2], %[jn] \t\n"
        "ss.end                u0, %[one], %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u0 \t\n"

        ".loop_y%=: \t\n"
        
            "so.a.mul.sg u19, u10, u9, p0\n\t" // filter[0] * src[(y + 1)][(x + 1)]
            "so.a.mul.sg u20, u11, u8, p0\n\t" // filter[1] * src[(y + 1)][(x)]
            "so.a.mul.sg u21, u12, u7, p0\n\t" // filter[2] * src[(y + 1)][(x - 1)]
            "so.a.mul.sg u22, u13, u6, p0\n\t" // filter[3] * src[(y)][(x + 1)]
            "so.a.mul.sg u23, u14, u5, p0\n\t" // filter[4] * src[(y)][(x)]
            "so.a.mul.sg u24, u15, u4, p0\n\t" // filter[5] * src[(y)][(x - 1)]
            "so.a.mul.sg u25, u16, u3, p0\n\t" // filter[6] * src[(y - 1)][(x + 1)]
            "so.a.mul.sg u26, u17, u2, p0\n\t" // filter[7] * src[(y - 1)][(x)]
            "so.a.mul.sg u27, u18, u1, p0\n\t" // filter[8] * src[(y - 1)][(x - 1)]

            "so.a.add.sg u28, u19, u20, p0\n\t" 
            "so.a.add.sg u29, u21, u22, p0\n\t"
            "so.a.add.sg u30, u23, u24, p0\n\t"
            "so.a.add.sg u31, u25, u26, p0\n\t"

            "so.a.add.sg u29, u29, u28, p0\n\t"
            "so.a.add.sg u31, u31, u30, p0\n\t"
            "so.a.add.sg u31, u31, u29, p0\n\t"

            "so.a.add.sg u0, u31, u27, p0\n\t"

            "so.b.nc	u0, .loop_y%= \n\t"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src_0] "r" (src),         [src_1] "r" (src+1),           [src_2] "r" (src+2),
        [src_3] "r" (src+PB_J),      [src_4] "r" (src+PB_J+1),      [src_5] "r" (src+PB_J+2),
        [src_6] "r" (src+PB_J+PB_J), [src_7] "r" (src+PB_J+PB_J+1), [src_8] "r" (src+PB_J+PB_J+2),
        [dst] "r"(dst + PB_J), [filter] "r"(filter), 	
        [in] "r"(PB_I), [jn] "r"(PB_J), [jnm2] "r"(PB_J - 2), [inm2] "r"(PB_I - 2), [fsize] "r"(3),
        [one] "r"(1),
        [filter0] "r"(filter[0]), [filter1] "r"(filter[1]), [filter2] "r"(filter[2]),
        [filter3] "r"(filter[3]), [filter4] "r"(filter[4]), [filter5] "r"(filter[5]),
        [filter6] "r"(filter[6]), [filter7] "r"(filter[7]), [filter8] "r"(filter[8]));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.h           u1, %[src_0], %[inm2], %[jn] \t\n"
        "ss.end                u1, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        // src(y-1,x) stream load
        "ss.sta.ld.h           u2, %[src_1], %[inm2], %[jn] \t\n"
        "ss.end                u2, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u2 \t\n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.h           u3, %[src_2], %[inm2], %[jn] \t\n"
        "ss.end                u3, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u3 \t\n"

        // src(y,x-1) stream load
        "ss.sta.ld.h           u4, %[src_3], %[inm2], %[jn] \t\n"
        "ss.end                u4, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u4 \t\n"

        // src(y,x) stream load
        "ss.sta.ld.h           u5, %[src_4], %[inm2], %[jn] \t\n"
        "ss.end                u5, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u5 \t\n"

        // src(y,x+1) stream load
        "ss.sta.ld.h           u6, %[src_5], %[inm2], %[jn] \t\n"
        "ss.end                u6, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.h           u7, %[src_6], %[inm2], %[jn] \t\n"
        "ss.end                u7, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // src(y+1,x) stream load
        "ss.sta.ld.h           u8, %[src_7], %[inm2], %[jn] \t\n"
        "ss.end                u8, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u8 \t\n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.h           u9, %[src_8], %[inm2], %[jn] \t\n"
        "ss.end                u9, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u9 \t\n"

        /* filter(k,j) stream load
        "ss.sta.ld.h           u10, %[filter], %[inm2], zero \t\n"
        "ss.app                u10, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u10 \t\n"
        "ss.end                u10, zero, %[fsize], zero \t\n"*/

        // filter
        "so.v.dp.h  u10, %[filter0], p0\t\n"
        "so.v.dp.h  u11, %[filter1], p0\t\n"
        "so.v.dp.h  u12, %[filter2], p0\t\n"
        "so.v.dp.h  u13, %[filter3], p0\t\n"
        "so.v.dp.h  u14, %[filter4], p0\t\n"
        "so.v.dp.h  u15, %[filter5], p0\t\n"
        "so.v.dp.h  u16, %[filter6], p0\t\n"
        "so.v.dp.h  u17, %[filter7], p0\t\n"
        "so.v.dp.h  u18, %[filter8], p0\t\n"


        // dst(y,x) stream store
        "ss.sta.st.h           u0, %[dst], %[inm2], %[jn] \t\n"
        "ss.end                u0, %[one], %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u0 \t\n"

        ".loop_y%=: \t\n"
        
            "so.a.mul.sg u19, u10, u9, p0\n\t" // filter[0] * src[(y + 1)][(x + 1)]
            "so.a.mul.sg u20, u11, u8, p0\n\t" // filter[1] * src[(y + 1)][(x)]
            "so.a.mul.sg u21, u12, u7, p0\n\t" // filter[2] * src[(y + 1)][(x - 1)]
            "so.a.mul.sg u22, u13, u6, p0\n\t" // filter[3] * src[(y)][(x + 1)]
            "so.a.mul.sg u23, u14, u5, p0\n\t" // filter[4] * src[(y)][(x)]
            "so.a.mul.sg u24, u15, u4, p0\n\t" // filter[5] * src[(y)][(x - 1)]
            "so.a.mul.sg u25, u16, u3, p0\n\t" // filter[6] * src[(y - 1)][(x + 1)]
            "so.a.mul.sg u26, u17, u2, p0\n\t" // filter[7] * src[(y - 1)][(x)]
            "so.a.mul.sg u27, u18, u1, p0\n\t" // filter[8] * src[(y - 1)][(x - 1)]

            "so.a.add.sg u28, u19, u20, p0\n\t" 
            "so.a.add.sg u29, u21, u22, p0\n\t"
            "so.a.add.sg u30, u23, u24, p0\n\t"
            "so.a.add.sg u31, u25, u26, p0\n\t"

            "so.a.add.sg u29, u29, u28, p0\n\t"
            "so.a.add.sg u31, u31, u30, p0\n\t"
            "so.a.add.sg u31, u31, u29, p0\n\t"

            "so.a.add.sg u0, u31, u27, p0\n\t"

            "so.b.nc	u0, .loop_y%= \n\t"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src_0] "r" (src),         [src_1] "r" (src+1),           [src_2] "r" (src+2),
        [src_3] "r" (src+PB_J),      [src_4] "r" (src+PB_J+1),      [src_5] "r" (src+PB_J+2),
        [src_6] "r" (src+PB_J+PB_J), [src_7] "r" (src+PB_J+PB_J+1), [src_8] "r" (src+PB_J+PB_J+2),
        [dst] "r"(dst + PB_J), [filter] "r"(filter), 	
        [in] "r"(PB_I), [jn] "r"(PB_J), [jnm2] "r"(PB_J - 2), [inm2] "r"(PB_I - 2), [fsize] "r"(3),
        [one] "r"(1),
        [filter0] "r"(filter[0]), [filter1] "r"(filter[1]), [filter2] "r"(filter[2]),
        [filter3] "r"(filter[3]), [filter4] "r"(filter[4]), [filter5] "r"(filter[5]),
        [filter6] "r"(filter[6]), [filter7] "r"(filter[7]), [filter8] "r"(filter[8]));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \t\n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.b           u1, %[src_0], %[inm2], %[jn] \t\n"
        "ss.end                u1, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        // src(y-1,x) stream load
        "ss.sta.ld.b           u2, %[src_1], %[inm2], %[jn] \t\n"
        "ss.end                u2, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u2 \t\n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.b           u3, %[src_2], %[inm2], %[jn] \t\n"
        "ss.end                u3, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u3 \t\n"

        // src(y,x-1) stream load
        "ss.sta.ld.b           u4, %[src_3], %[inm2], %[jn] \t\n"
        "ss.end                u4, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u4 \t\n"

        // src(y,x) stream load
        "ss.sta.ld.b           u5, %[src_4], %[inm2], %[jn] \t\n"
        "ss.end                u5, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u5 \t\n"

        // src(y,x+1) stream load
        "ss.sta.ld.b           u6, %[src_5], %[inm2], %[jn] \t\n"
        "ss.end                u6, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u6 \t\n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.b           u7, %[src_6], %[inm2], %[jn] \t\n"
        "ss.end                u7, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u7 \t\n"

        // src(y+1,x) stream load
        "ss.sta.ld.b           u8, %[src_7], %[inm2], %[jn] \t\n"
        "ss.end                u8, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u8 \t\n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.b           u9, %[src_8], %[inm2], %[jn] \t\n"
        "ss.end                u9, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u9 \t\n"

        /* filter(k,j) stream load
        "ss.sta.ld.b           u10, %[filter], %[inm2], zero \t\n"
        "ss.app                u10, zero, %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u10 \t\n"
        "ss.end                u10, zero, %[fsize], zero \t\n"*/

        // filter
        "so.v.dp.b  u10, %[filter0], p0\t\n"
        "so.v.dp.b  u11, %[filter1], p0\t\n"
        "so.v.dp.b  u12, %[filter2], p0\t\n"
        "so.v.dp.b  u13, %[filter3], p0\t\n"
        "so.v.dp.b  u14, %[filter4], p0\t\n"
        "so.v.dp.b  u15, %[filter5], p0\t\n"
        "so.v.dp.b  u16, %[filter6], p0\t\n"
        "so.v.dp.b  u17, %[filter7], p0\t\n"
        "so.v.dp.b  u18, %[filter8], p0\t\n"


        // dst(y,x) stream store
        "ss.sta.st.b           u0, %[dst], %[inm2], %[jn] \t\n"
        "ss.end                u0, %[one], %[jnm2], %[one] \t\n"
        "ss.cfg.vec            u0 \t\n"

        ".loop_y%=: \t\n"
        
            "so.a.mul.sg u19, u10, u9, p0\n\t" // filter[0] * src[(y + 1)][(x + 1)]
            "so.a.mul.sg u20, u11, u8, p0\n\t" // filter[1] * src[(y + 1)][(x)]
            "so.a.mul.sg u21, u12, u7, p0\n\t" // filter[2] * src[(y + 1)][(x - 1)]
            "so.a.mul.sg u22, u13, u6, p0\n\t" // filter[3] * src[(y)][(x + 1)]
            "so.a.mul.sg u23, u14, u5, p0\n\t" // filter[4] * src[(y)][(x)]
            "so.a.mul.sg u24, u15, u4, p0\n\t" // filter[5] * src[(y)][(x - 1)]
            "so.a.mul.sg u25, u16, u3, p0\n\t" // filter[6] * src[(y - 1)][(x + 1)]
            "so.a.mul.sg u26, u17, u2, p0\n\t" // filter[7] * src[(y - 1)][(x)]
            "so.a.mul.sg u27, u18, u1, p0\n\t" // filter[8] * src[(y - 1)][(x - 1)]

            "so.a.add.sg u28, u19, u20, p0\n\t" 
            "so.a.add.sg u29, u21, u22, p0\n\t"
            "so.a.add.sg u30, u23, u24, p0\n\t"
            "so.a.add.sg u31, u25, u26, p0\n\t"

            "so.a.add.sg u29, u29, u28, p0\n\t"
            "so.a.add.sg u31, u31, u30, p0\n\t"
            "so.a.add.sg u31, u31, u29, p0\n\t"

            "so.a.add.sg u0, u31, u27, p0\n\t"

            "so.b.nc	u0, .loop_y%= \n\t"

        "rdinstret %[e] \t\n"

        : [s] "=&r" (start), [e] "=&r" (end)
        : [src_0] "r" (src),         [src_1] "r" (src+1),           [src_2] "r" (src+2),
        [src_3] "r" (src+PB_J),      [src_4] "r" (src+PB_J+1),      [src_5] "r" (src+PB_J+2),
        [src_6] "r" (src+PB_J+PB_J), [src_7] "r" (src+PB_J+PB_J+1), [src_8] "r" (src+PB_J+PB_J+2),
        [dst] "r"(dst + PB_J), [filter] "r"(filter), 	
        [in] "r"(PB_I), [jn] "r"(PB_J), [jnm2] "r"(PB_J - 2), [inm2] "r"(PB_I - 2), [fsize] "r"(3),
        [one] "r"(1),
        [filter0] "r"(filter[0]), [filter1] "r"(filter[1]), [filter2] "r"(filter[2]),
        [filter3] "r"(filter[3]), [filter4] "r"(filter[4]), [filter5] "r"(filter[5]),
        [filter6] "r"(filter[6]), [filter7] "r"(filter[7]), [filter8] "r"(filter[8]));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

    DataType sum;
    int y, x, k, j;
    for (y = 1; y < PB_I - 1; y++){
        for (x = 1; x < PB_J - 1; x++){
            sum = 0.0;
            for (k = -1; k <= 1; k++){
                for (j = -1; j <= 1; j++)
                    sum = sum + filter[(j + 1) * 3 + k + 1] * src[(y - j) * PB_J + (x - k)];
            }
            dst[y * PB_J + x] = sum;
        }
    }
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

/*
for (y = 1; y < PB_I - 1; y++){
    for (x = 1; x < PB_J - 1; x++){
                dst[y * PB_I + x] = filter[0] * src[(y + 1)][(x + 1)] +
                            filter[1] * src[(y + 1)][(x)] +
                            filter[2] * src[(y + 1)][(x - 1)] +
                            filter[3] * src[(y)][(x + 1)] +
                            filter[4] * src[(y)][(x)] +
                            filter[5] * src[(y)][(x - 1)] +
                            filter[6] * src[(y - 1)][(x + 1)] +
                            filter[7] * src[(y - 1)][(x)] +
                            filter[8] * src[(y - 1)][(x - 1)];

        dst[y * PB_I + x] = filter[0] * src[(y - 1)][(x - 1)] +
                            filter[1] * src[(y - 1)][(x)] +
                            filter[2] * src[(y - 1)][(x + 1)] +
                            filter[3] * src[(y)][(x - 1)] +
                            filter[4] * src[(y)][(x)] +
                            filter[5] * src[(y)][(x + 1)] +
                            filter[6] * src[(y + 1)][(x - 1)] +
                            filter[7] * src[(y + 1)][(x)] +
                            filter[8] * src[(y + 1)][(x + 1)];
    }
}
*/