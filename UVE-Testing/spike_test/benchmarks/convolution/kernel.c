#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *src, DataType *dst, DataType *filter, int PB_J, int PB_I){
    asm volatile(
        "rdinstret %[s] \n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.d.v         u1, %[src_0] \n"
        "ss.app                u1, zero, %[inm2], %[jn] \n"
        "ss.end                u1, zero, %[jnm2], %[one] \n"

        // src(y-1,x) stream load
        "ss.sta.ld.d.v         u2, %[src_1] \n"
        "ss.app                u2, zero, %[inm2], %[jn] \n"
        "ss.end                u2, zero, %[jnm2], %[one] \n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.d.v         u3, %[src_2] \n"
        "ss.app                u3, zero, %[inm2], %[jn] \n"
        "ss.end                u3, zero, %[jnm2], %[one] \n"

        // src(y,x-1) stream load
        "ss.sta.ld.d.v         u4, %[src_3] \n"
        "ss.app                u4, zero, %[inm2], %[jn] \n"
        "ss.end                u4, zero, %[jnm2], %[one] \n"

        // src(y,x) stream load
        "ss.sta.ld.d.v         u5, %[src_4] \n"
        "ss.app                u5, zero, %[inm2], %[jn] \n"
        "ss.end                u5, zero, %[jnm2], %[one] \n"

        // src(y,x+1) stream load
        "ss.sta.ld.d.v         u6, %[src_5] \n"
        "ss.app                u6, zero, %[inm2], %[jn] \n"
        "ss.end                u6, zero, %[jnm2], %[one] \n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.d.v         u7, %[src_6] \n"
        "ss.app                u7, zero, %[inm2], %[jn] \n"
        "ss.end                u7, zero, %[jnm2], %[one] \n"

        // src(y+1,x) stream load
        "ss.sta.ld.d.v         u8, %[src_7] \n"
        "ss.app                u8, zero, %[inm2], %[jn] \n"
        "ss.end                u8, zero, %[jnm2], %[one] \n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.d.v         u9, %[src_8] \n"
        "ss.app                u9, zero, %[inm2], %[jn] \n"
        "ss.end                u9, zero, %[jnm2], %[one] \n"

        /* filter(k,j) stream load
        "ss.sta.ld.d.v         u10, %[filter] \n"
        "ss.app                u10, zero, %[inm2], zero \n"
        "ss.app                u10, zero, %[jnm2], %[one] \n""
        "ss.end                u10, zero, %[fsize], zero \n"*/

        // filter
        "so.v.dp.d  u10, %[filter0], p0\n"
        "so.v.dp.d  u11, %[filter1], p0\n"
        "so.v.dp.d  u12, %[filter2], p0\n"
        "so.v.dp.d  u13, %[filter3], p0\n"
        "so.v.dp.d  u14, %[filter4], p0\n"
        "so.v.dp.d  u15, %[filter5], p0\n"
        "so.v.dp.d  u16, %[filter6], p0\n"
        "so.v.dp.d  u17, %[filter7], p0\n"
        "so.v.dp.d  u18, %[filter8], p0\n"


        // dst(y,x) stream store
        "ss.sta.st.d.v    u0, %[dst] \n"
        "ss.app           u0, zero, %[inm2], %[jn] \n"
        "ss.end           u0, %[one], %[jnm2], %[one] \n"

        ".loop_y%=: \n"
        
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

        "rdinstret %[e] \n"

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
        "rdinstret %[s] \n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.w.v         u1, %[src_0] \n"
        "ss.app                u1, zero, %[inm2], %[jn] \n"
        "ss.end                u1, zero, %[jnm2], %[one] \n"

        // src(y-1,x) stream load
        "ss.sta.ld.w.v         u2, %[src_1] \n"
        "ss.app                u2, zero, %[inm2], %[jn] \n"
        "ss.end                u2, zero, %[jnm2], %[one] \n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.w.v         u3, %[src_2] \n"
        "ss.app                u3, zero, %[inm2], %[jn] \n"
        "ss.end                u3, zero, %[jnm2], %[one] \n"

        // src(y,x-1) stream load
        "ss.sta.ld.w.v         u4, %[src_3] \n"
        "ss.app                u4, zero, %[inm2], %[jn] \n"
        "ss.end                u4, zero, %[jnm2], %[one] \n"

        // src(y,x) stream load
        "ss.sta.ld.w.v         u5, %[src_4] \n"
        "ss.app                u5, zero, %[inm2], %[jn] \n"
        "ss.end                u5, zero, %[jnm2], %[one] \n"

        // src(y,x+1) stream load
        "ss.sta.ld.w.v         u6, %[src_5] \n"
        "ss.app                u6, zero, %[inm2], %[jn] \n"
        "ss.end                u6, zero, %[jnm2], %[one] \n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.w.v         u7, %[src_6] \n"
        "ss.app                u7, zero, %[inm2], %[jn] \n"
        "ss.end                u7, zero, %[jnm2], %[one] \n"

        // src(y+1,x) stream load
        "ss.sta.ld.w.v         u8, %[src_7] \n"
        "ss.app                u8, zero, %[inm2], %[jn] \n"
        "ss.end                u8, zero, %[jnm2], %[one] \n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.w.v         u9, %[src_8] \n"
        "ss.app                u9, zero, %[inm2], %[jn] \n"
        "ss.end                u9, zero, %[jnm2], %[one] \n"

        /* filter(k,j) stream load
        "ss.sta.ld.w.v         u10, %[filte \n"
        "ss.app                u10, zero,], %[inm2], zero \n"
        "ss.app                u10, zero, %[jnm2], %[one] \n""
        "ss.end                u10, zero, %[fsize], zero \n"*/

        // filter
        "so.v.dp.w  u10, %[filter0], p0\n"
        "so.v.dp.w  u11, %[filter1], p0\n"
        "so.v.dp.w  u12, %[filter2], p0\n"
        "so.v.dp.w  u13, %[filter3], p0\n"
        "so.v.dp.w  u14, %[filter4], p0\n"
        "so.v.dp.w  u15, %[filter5], p0\n"
        "so.v.dp.w  u16, %[filter6], p0\n"
        "so.v.dp.w  u17, %[filter7], p0\n"
        "so.v.dp.w  u18, %[filter8], p0\n"


        // dst(y,x) stream store
        "ss.sta.st.w.v    u0, %[dst] \n"
        "ss.app           u0, zero, %[inm2], %[jn] \n"
        "ss.end           u0, %[one], %[jnm2], %[one] \n"

        ".loop_y%=: \n"
        
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

        "rdinstret %[e] \n"

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
        "rdinstret %[s] \n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.w.v         u1, %[src_0] \n"
        "ss.app                u1, zero, %[inm2], %[jn] \n"
        "ss.end                u1, zero, %[jnm2], %[one] \n"

        // src(y-1,x) stream load
        "ss.sta.ld.w.v         u2, %[src_1] \n"
        "ss.app                u2, zero, %[inm2], %[jn] \n"
        "ss.end                u2, zero, %[jnm2], %[one] \n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.w.v         u3, %[src_2] \n"
        "ss.app                u3, zero, %[inm2], %[jn] \n"
        "ss.end                u3, zero, %[jnm2], %[one] \n"

        // src(y,x-1) stream load
        "ss.sta.ld.w.v         u4, %[src_3] \n"
        "ss.app                u4, zero, %[inm2], %[jn] \n"
        "ss.end                u4, zero, %[jnm2], %[one] \n"

        // src(y,x) stream load
        "ss.sta.ld.w.v         u5, %[src_4] \n"
        "ss.app                u5, zero, %[inm2], %[jn] \n"
        "ss.end                u5, zero, %[jnm2], %[one] \n"

        // src(y,x+1) stream load
        "ss.sta.ld.w.v         u6, %[src_5] \n"
        "ss.app                u6, zero, %[inm2], %[jn] \n"
        "ss.end                u6, zero, %[jnm2], %[one] \n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.w.v         u7, %[src_6] \n"
        "ss.app                u7, zero, %[inm2], %[jn] \n"
        "ss.end                u7, zero, %[jnm2], %[one] \n"

        // src(y+1,x) stream load
        "ss.sta.ld.w.v         u8, %[src_7] \n"
        "ss.app                u8, zero, %[inm2], %[jn] \n"
        "ss.end                u8, zero, %[jnm2], %[one] \n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.w.v         u9, %[src_8] \n"
        "ss.app                u9, zero, %[inm2], %[jn] \n"
        "ss.end                u9, zero, %[jnm2], %[one] \n"

        /* filter(k,j) stream load
        "ss.sta.ld.w.v         u10, %[filte \n"
        "ss.app                u10, zero,], %[inm2], zero \n"
        "ss.app                u10, zero, %[jnm2], %[one] \n""
        "ss.end                u10, zero, %[fsize], zero \n"*/

        // filter
        "so.v.dp.w  u10, %[filter0], p0\n"
        "so.v.dp.w  u11, %[filter1], p0\n"
        "so.v.dp.w  u12, %[filter2], p0\n"
        "so.v.dp.w  u13, %[filter3], p0\n"
        "so.v.dp.w  u14, %[filter4], p0\n"
        "so.v.dp.w  u15, %[filter5], p0\n"
        "so.v.dp.w  u16, %[filter6], p0\n"
        "so.v.dp.w  u17, %[filter7], p0\n"
        "so.v.dp.w  u18, %[filter8], p0\n"


        // dst(y,x) stream store
        "ss.sta.st.w.v    u0, %[dst] \n"
        "ss.app           u0, zero, %[inm2], %[jn] \n"
        "ss.end           u0, %[one], %[jnm2], %[one] \n"

        ".loop_y%=: \n"
        
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

        "rdinstret %[e] \n"

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
        "rdinstret %[s] \n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.h.v         u1, %[src_0] \n"
        "ss.app                u1, zero, %[inm2], %[jn] \n"
        "ss.end                u1, zero, %[jnm2], %[one] \n"

        // src(y-1,x) stream load
        "ss.sta.ld.h.v         u2, %[src_1] \n"
        "ss.app                u2, zero, %[inm2], %[jn] \n"
        "ss.end                u2, zero, %[jnm2], %[one] \n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.h.v         u3, %[src_2] \n"
        "ss.app                u3, zero, %[inm2], %[jn] \n"
        "ss.end                u3, zero, %[jnm2], %[one] \n"

        // src(y,x-1) stream load
        "ss.sta.ld.h.v         u4, %[src_3] \n"
        "ss.app                u4, zero, %[inm2], %[jn] \n"
        "ss.end                u4, zero, %[jnm2], %[one] \n"

        // src(y,x) stream load
        "ss.sta.ld.h.v         u5, %[src_4] \n"
        "ss.app                u5, zero, %[inm2], %[jn] \n"
        "ss.end                u5, zero, %[jnm2], %[one] \n"

        // src(y,x+1) stream load
        "ss.sta.ld.h.v         u6, %[src_5] \n"
        "ss.app                u6, zero, %[inm2], %[jn] \n"
        "ss.end                u6, zero, %[jnm2], %[one] \n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.h.v         u7, %[src_6] \n"
        "ss.app                u7, zero, %[inm2], %[jn] \n"
        "ss.end                u7, zero, %[jnm2], %[one] \n"

        // src(y+1,x) stream load
        "ss.sta.ld.h.v         u8, %[src_7] \n"
        "ss.app                u8, zero, %[inm2], %[jn] \n"
        "ss.end                u8, zero, %[jnm2], %[one] \n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.h.v         u9, %[src_8] \n"
        "ss.app                u9, zero, %[inm2], %[jn] \n"
        "ss.end                u9, zero, %[jnm2], %[one] \n"

        /* filter(k,j) stream load
        "ss.sta.ld.h.v         u10, %[filter] \n"
        "ss.app                u10, zero, %[inm2], zero \n"
        "ss.app                u10, zero, %[jnm2], %[one] \n""
        "ss.end                u10, zero, %[fsize], zero \n"*/

        // filter
        "so.v.dp.h  u10, %[filter0], p0\n"
        "so.v.dp.h  u11, %[filter1], p0\n"
        "so.v.dp.h  u12, %[filter2], p0\n"
        "so.v.dp.h  u13, %[filter3], p0\n"
        "so.v.dp.h  u14, %[filter4], p0\n"
        "so.v.dp.h  u15, %[filter5], p0\n"
        "so.v.dp.h  u16, %[filter6], p0\n"
        "so.v.dp.h  u17, %[filter7], p0\n"
        "so.v.dp.h  u18, %[filter8], p0\n"


        // dst(y,x) stream store
        "ss.sta.st.h.v    u0, %[dst] \n"
        "ss.app           u0, zero, %[inm2], %[jn] \n"
        "ss.end           u0, %[one], %[jnm2], %[one] \n"

        ".loop_y%=: \n"
        
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

        "rdinstret %[e] \n"

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
        "rdinstret %[s] \n"

        // src(y-1,x-1) stream load
        "ss.sta.ld.b.v         u1, %[src_0] \n"
        "ss.app                u1, zero, %[inm2], %[jn] \n"
        "ss.end                u1, zero, %[jnm2], %[one] \n"

        // src(y-1,x) stream load
        "ss.sta.ld.b.v         u2, %[src_1] \n"
        "ss.app                u2, zero, %[inm2], %[jn] \n"
        "ss.end                u2, zero, %[jnm2], %[one] \n"

        // src(y-1,x+1) stream load
        "ss.sta.ld.b.v         u3, %[src_2] \n"
        "ss.app                u3, zero, %[inm2], %[jn] \n"
        "ss.end                u3, zero, %[jnm2], %[one] \n"

        // src(y,x-1) stream load
        "ss.sta.ld.b.v         u4, %[src_3] \n"
        "ss.app                u4, zero, %[inm2], %[jn] \n"
        "ss.end                u4, zero, %[jnm2], %[one] \n"

        // src(y,x) stream load
        "ss.sta.ld.b.v         u5, %[src_4] \n"
        "ss.app                u5, zero, %[inm2], %[jn] \n"
        "ss.end                u5, zero, %[jnm2], %[one] \n"

        // src(y,x+1) stream load
        "ss.sta.ld.b.v         u6, %[src_5] \n"
        "ss.app                u6, zero, %[inm2], %[jn] \n"
        "ss.end                u6, zero, %[jnm2], %[one] \n"

        // src(y+1,x-1) stream load
        "ss.sta.ld.b.v         u7, %[src_6] \n"
        "ss.app                u7, zero, %[inm2], %[jn] \n"
        "ss.end                u7, zero, %[jnm2], %[one] \n"

        // src(y+1,x) stream load
        "ss.sta.ld.b.v         u8, %[src_7] \n"
        "ss.app                u8, zero, %[inm2], %[jn] \n"
        "ss.end                u8, zero, %[jnm2], %[one] \n"

        // src(y+1,x+1) stream load
        "ss.sta.ld.b.v         u9, %[src_8] \n"
        "ss.app                u9, zero, %[inm2], %[jn] \n"
        "ss.end                u9, zero, %[jnm2], %[one] \n"

        /* filter(k,j) stream load
        "ss.sta.ld.b.v         u10, %[filte \n"
        "ss.app                u10, zero,], %[inm2], zero \n"
        "ss.app                u10, zero, %[jnm2], %[one] \n""
        "ss.end                u10, zero, %[fsize], zero \n"*/

        // filter
        "so.v.dp.b  u10, %[filter0], p0\n"
        "so.v.dp.b  u11, %[filter1], p0\n"
        "so.v.dp.b  u12, %[filter2], p0\n"
        "so.v.dp.b  u13, %[filter3], p0\n"
        "so.v.dp.b  u14, %[filter4], p0\n"
        "so.v.dp.b  u15, %[filter5], p0\n"
        "so.v.dp.b  u16, %[filter6], p0\n"
        "so.v.dp.b  u17, %[filter7], p0\n"
        "so.v.dp.b  u18, %[filter8], p0\n"


        // dst(y,x) stream store
        "ss.sta.st.b.v    u0, %[dst] \n"
        "ss.app           u0, zero, %[inm2], %[jn] \n"
        "ss.end           u0, %[one], %[jnm2], %[one] \n"

        ".loop_y%=: \n"
        
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

        "rdinstret %[e] \n"

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
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

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
    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
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