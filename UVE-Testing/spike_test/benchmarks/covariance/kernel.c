#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \n"

        "ss.sta.ld.d.v      u1, %[data] \n"
        "ss.app             u1, zero, %[M], %[one] \n"
        "ss.end             u1, zero, %[N], %[M] \n"

        "ss.sta.st.d u2, %[mean] \n"
        "ss.end      u2, zero, %[M], %[one] \n"
        
        "so.v.mvsv.d u3, %[datat_n] \n"

        ".SLOOP_1%=: \n"
            "so.v.mvsv.d    u4, zero \n"

            ".SLOOP_1_0%=: \n"
                "so.a.adde.acc.fp   u4, u1, p0 \n"
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.div.fp    u2, u4, u3, p0 \n"
        "so.b.nc u1, .SLOOP_1%= \n"
        
        // kernel 2
  
        "ss.sta.st.d.v      u1, %[data] \n"
        "ss.app             u1, zero, %[N], %[M] \n"
        "ss.end             u1, zero, %[M], %[one] \n"

        "ss.sta.ld.d.v      u2, %[mean] \n"
        "ss.app             u2, zero, %[N], zero \n"
        "ss.end             u2, zero, %[M], %[one] \n"

        "ss.sta.ld.d.v      u3, %[data] \n"
        "ss.app             u3, zero, %[N], %[M] \n"
        "ss.end             u3, zero, %[M], %[one] \n"
        
        ".SLOOP_2%=: \n"
            "so.a.sub.fp  u1, u3, u2, p0 \n"
        "so.b.nc u1, .SLOOP_2%= \n"

        // kernel 3
  
        "ss.sta.ld.d.v.m      u1, %[data] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u1, %[one] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.end               u1, zero, %[N], %[M] \n"

        "ss.sta.ld.d.v.m      u2, %[data] \n"
        "ss.app               u2, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u2, %[one] \n"
        "ss.app               u2, zero, %[M], zero \n"
        "ss.end               u2, zero, %[N], %[M] \n"

        "ss.sta.st.d          u3, %[cov] \n"
        "ss.app               u3, zero, %[M], %[M] \n"
        "ss.app.mod.ofs.inc.2 u3, %[one] \n"
        "ss.app.mod.siz.dec.2 u3, %[one] \n"
        "ss.end               u3, zero, %[M], %[one] \n"

        "ss.sta.st.d          u4, %[cov] \n"
        "ss.app               u4, zero, %[M], %[one] \n"
        "ss.app.mod.ofs.inc.2 u4, %[M] \n"
        "ss.app.mod.siz.dec.2 u4, %[one] \n"
        "ss.end               u4, zero, %[M], %[M] \n"
        
        "so.v.mvsv.d u5, %[datat_nn] \n"

        ".SLOOP_3%=: \n"
            "so.v.dp.d      u6, zero, p0 \n"

            ".SLOOP_3_0_0%=: \n"
                "so.a.mac.fp  u6, u2, u1, p0 \n"
                /*so.a.mul.fp  u7, u2, u1, p0 \n"
                "so.a.add.fp  u6, u6, u7, p0 \n"*/
            "so.b.ndc.3 u2, .SLOOP_3_0_0%= \n"

            "so.a.adde.fp   u7, u6, p0 \n"
            "so.a.div.fp    u8, u7, u5, p0 \n"
            "so.v.mv        u3, u8, p0 \n"
            "so.v.mv        u4, u8, p0 \n"
        "so.b.nc u1, .SLOOP_3%= \n"

        "rdinstret %[e] \n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \n"

        "ss.sta.ld.w.v      u1, %[data] \n"
        "ss.app             u1, zero, %[M], %[one] \n"
        "ss.end             u1, zero, %[N], %[M] \n"

        "ss.sta.st.w u2, %[mean] \n"
        "ss.end      u2, zero, %[M], %[one] \n"
        
        "so.v.mvsv.w u3, %[datat_n] \n"

        ".SLOOP_1%=: \n"
            "so.v.mvsv.w    u4, zero \n"

            ".SLOOP_1_0%=: \n"
                "so.a.adde.acc.fp   u4, u1, p0 \n"
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.div.fp    u2, u4, u3, p0 \n"
        "so.b.nc u1, .SLOOP_1%= \n"
        
        // kernel 2
  
        "ss.sta.st.w.v      u1, %[data] \n"
        "ss.app             u1, zero, %[N], %[M] \n"
        "ss.end             u1, zero, %[M], %[one] \n"

        "ss.sta.ld.w.v      u2, %[mean] \n"
        "ss.app             u2, zero, %[N], zero \n"
        "ss.end             u2, zero, %[M], %[one] \n"

        "ss.sta.ld.w.v      u3, %[data] \n"
        "ss.app             u3, zero, %[N], %[M] \n"
        "ss.end             u3, zero, %[M], %[one] \n"
        
        ".SLOOP_2%=: \n"
            "so.a.sub.fp  u1, u3, u2, p0 \n"
        "so.b.nc u1, .SLOOP_2%= \n"

        // kernel 3
  
        "ss.sta.ld.w.v.m      u1, %[data] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u1, %[one] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.end               u1, zero, %[N], %[M] \n"

        "ss.sta.ld.w.v.m      u2, %[data] \n"
        "ss.app               u2, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u2, %[one] \n"
        "ss.app               u2, zero, %[M], zero \n"
        "ss.end               u2, zero, %[N], %[M] \n"

        "ss.sta.st.w          u3, %[cov] \n"
        "ss.app               u3, zero, %[M], %[M] \n"
        "ss.app.mod.ofs.inc.2 u3, %[one] \n"
        "ss.app.mod.siz.dec.2 u3, %[one] \n"
        "ss.end               u3, zero, %[M], %[one] \n"

        "ss.sta.st.w          u4, %[cov] \n"
        "ss.app               u4, zero, %[M], %[one] \n"
        "ss.app.mod.ofs.inc.2 u4, %[M] \n"
        "ss.app.mod.siz.dec.2 u4, %[one] \n"
        "ss.end               u4, zero, %[M], %[M] \n"
        
        "so.v.mvsv.w u5, %[datat_nn] \n"

        ".SLOOP_3%=: \n"
            "so.v.dp.w      u6, zero, p0 \n"

            ".SLOOP_3_0_0%=: \n"
                /*"so.a.mul.fp  u7, u2, u1, p0 \n"
                "so.a.add.fp  u6, u6, u7, p0 \n"*/
                "so.a.mac.fp  u6, u2, u1, p0 \n"
            "so.b.ndc.3 u2, .SLOOP_3_0_0%= \n"

            "so.a.adde.fp   u7, u6, p0 \n"
            "so.a.div.fp    u8, u7, u5, p0 \n"
            "so.v.mv        u3, u8, p0 \n"
            "so.v.mv        u4, u8, p0 \n"
        "so.b.nc u1, .SLOOP_3%= \n"

        "rdinstret %[e] \n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \n"

        "ss.sta.ld.w.v      u1, %[data] \n"
        "ss.app             u1, zero, %[M], %[one] \n"
        "ss.end             u1, zero, %[N], %[M] \n"

        "ss.sta.st.w u2, %[mean] \n"
        "ss.end      u2, zero, %[M], %[one] \n"
        
        "so.v.mvsv.w u3, %[datat_n] \n"

        ".SLOOP_1%=: \n"
            "so.v.mvsv.w    u4, zero \n"

            ".SLOOP_1_0%=: \n"
                "so.a.adde.acc.sg   u4, u1, p0 \n"
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.div.sg    u2, u4, u3, p0 \n"
        "so.b.nc u1, .SLOOP_1%= \n"
        
        // kernel 2
  
        "ss.sta.st.w.v      u1, %[data] \n"
        "ss.app             u1, zero, %[N], %[M] \n"
        "ss.end             u1, zero, %[M], %[one] \n"

        "ss.sta.ld.w.v      u2, %[mean] \n"
        "ss.app             u2, zero, %[N], zero \n"
        "ss.end             u2, zero, %[M], %[one] \n"

        "ss.sta.ld.w.v      u3, %[data] \n"
        "ss.app             u3, zero, %[N], %[M] \n"
        "ss.end             u3, zero, %[M], %[one] \n"
        
        ".SLOOP_2%=: \n"
            "so.a.sub.sg  u1, u3, u2, p0 \n"
        "so.b.nc u1, .SLOOP_2%= \n"

        // kernel 3
  
        "ss.sta.ld.w.v.m      u1, %[data] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u1, %[one] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.end               u1, zero, %[N], %[M] \n"

        "ss.sta.ld.w.v.m      u2, %[data] \n"
        "ss.app               u2, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u2, %[one] \n"
        "ss.app               u2, zero, %[M], zero \n"
        "ss.end               u2, zero, %[N], %[M] \n"

        "ss.sta.st.w          u3, %[cov] \n"
        "ss.app               u3, zero, %[M], %[M] \n"
        "ss.app.mod.ofs.inc.2 u3, %[one] \n"
        "ss.app.mod.siz.dec.2 u3, %[one] \n"
        "ss.end               u3, zero, %[M], %[one] \n"

        "ss.sta.st.w          u4, %[cov] \n"
        "ss.app               u4, zero, %[M], %[one] \n"
        "ss.app.mod.ofs.inc.2 u4, %[M] \n"
        "ss.app.mod.siz.dec.2 u4, %[one] \n"
        "ss.end               u4, zero, %[M], %[M] \n"
        
        "so.v.mvsv.w u5, %[datat_nn] \n"

        ".SLOOP_3%=: \n"
            "so.v.dp.w      u6, zero, p0 \n"

            ".SLOOP_3_0_0%=: \n"
                /*"so.a.mul.sg  u7, u2, u1, p0 \n"
                "so.a.add.sg  u6, u6, u7, p0 \n"*/
                "so.a.mac.sg  u6, u2, u1, p0 \n"
            "so.b.ndc.3 u2, .SLOOP_3_0_0%= \n"

            "so.a.adde.sg   u7, u6, p0 \n"
            "so.a.div.sg    u8, u7, u5, p0 \n"
            "so.v.mv        u3, u8, p0 \n"
            "so.v.mv        u4, u8, p0 \n"
        "so.b.nc u1, .SLOOP_3%= \n"

        "rdinstret %[e] \n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \n"

        "ss.sta.ld.h.v      u1, %[data] \n"
        "ss.app             u1, zero, %[M], %[one] \n"
        "ss.end             u1, zero, %[N], %[M] \n"

        "ss.sta.st.h u2, %[mean] \n"
        "ss.end      u2, zero, %[M], %[one] \n"
        
        "so.v.mvsv.h u3, %[datat_n] \n"

        ".SLOOP_1%=: \n"
            "so.v.mvsv.h    u4, zero \n"

            ".SLOOP_1_0%=: \n"
                "so.a.adde.acc.sg   u4, u1, p0 \n"
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.div.sg    u2, u4, u3, p0 \n"
        "so.b.nc u1, .SLOOP_1%= \n"
        
        // kernel 2
  
        "ss.sta.st.h.v      u1, %[data] \n"
        "ss.app             u1, zero, %[N], %[M] \n"
        "ss.end             u1, zero, %[M], %[one] \n"

        "ss.sta.ld.h.v      u2, %[mean] \n"
        "ss.app             u2, zero, %[N], zero \n"
        "ss.end             u2, zero, %[M], %[one] \n"

        "ss.sta.ld.h.v      u3, %[data] \n"
        "ss.app             u3, zero, %[N], %[M] \n"
        "ss.end             u3, zero, %[M], %[one] \n"
        
        ".SLOOP_2%=: \n"
            "so.a.sub.sg  u1, u3, u2, p0 \n"
        "so.b.nc u1, .SLOOP_2%= \n"

        // kernel 3
  
        "ss.sta.ld.h.v.m      u1, %[data] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u1, %[one] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.end               u1, zero, %[N], %[M] \n"

        "ss.sta.ld.h.v.m      u2, %[data] \n"
        "ss.app               u2, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u2, %[one] \n"
        "ss.app               u2, zero, %[M], zero \n"
        "ss.end               u2, zero, %[N], %[M] \n"

        "ss.sta.st.h          u3, %[cov] \n"
        "ss.app               u3, zero, %[M], %[M] \n"
        "ss.app.mod.ofs.inc.2 u3, %[one] \n"
        "ss.app.mod.siz.dec.2 u3, %[one] \n"
        "ss.end               u3, zero, %[M], %[one] \n"

        "ss.sta.st.h          u4, %[cov] \n"
        "ss.app               u4, zero, %[M], %[one] \n"
        "ss.app.mod.ofs.inc.2 u4, %[M] \n"
        "ss.app.mod.siz.dec.2 u4, %[one] \n"
        "ss.end               u4, zero, %[M], %[M] \n"
        
        "so.v.mvsv.h u5, %[datat_nn] \n"

        ".SLOOP_3%=: \n"
            "so.v.dp.h      u6, zero, p0 \n"

            ".SLOOP_3_0_0%=: \n"
                /*"so.a.mul.sg  u7, u2, u1, p0 \n"
                "so.a.add.sg  u6, u6, u7, p0 \n"*/
                "so.a.mac.sg  u6, u2, u1, p0 \n"
            "so.b.ndc.3 u2, .SLOOP_3_0_0%= \n"

            "so.a.adde.sg   u7, u6, p0 \n"
            "so.a.div.sg    u8, u7, u5, p0 \n"
            "so.v.mv        u3, u8, p0 \n"
            "so.v.mv        u4, u8, p0 \n"
        "so.b.nc u1, .SLOOP_3%= \n"

        "rdinstret %[e] \n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \n"

        "ss.sta.ld.b.v      u1, %[data] \n"
        "ss.app             u1, zero, %[M], %[one] \n"
        "ss.end             u1, zero, %[N], %[M] \n"

        "ss.sta.st.b u2, %[mean] \n"
        "ss.end      u2, zero, %[M], %[one] \n"
        
        "so.v.mvsv.b u3, %[datat_n] \n"

        ".SLOOP_1%=: \n"
            "so.v.mvsv.b    u4, zero \n"

            ".SLOOP_1_0%=: \n"
                "so.a.adde.acc.sg   u4, u1, p0 \n"
            "so.b.ndc.2 u1, .SLOOP_1_0%= \n"

            "so.a.div.sg    u2, u4, u3, p0 \n"
        "so.b.nc u1, .SLOOP_1%= \n"
        
        // kernel 2
  
        "ss.sta.st.b.v      u1, %[data] \n"
        "ss.app             u1, zero, %[N], %[M] \n"
        "ss.end             u1, zero, %[M], %[one] \n"

        "ss.sta.ld.b.v      u2, %[mean] \n"
        "ss.app             u2, zero, %[N], zero \n"
        "ss.end             u2, zero, %[M], %[one] \n"

        "ss.sta.ld.b.v      u3, %[data] \n"
        "ss.app             u3, zero, %[N], %[M] \n"
        "ss.end             u3, zero, %[M], %[one] \n"
        
        ".SLOOP_2%=: \n"
            "so.a.sub.sg  u1, u3, u2, p0 \n"
        "so.b.nc u1, .SLOOP_2%= \n"

        // kernel 3
  
        "ss.sta.ld.b.v.m      u1, %[data] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u1, %[one] \n"
        "ss.app               u1, zero, %[M], %[one] \n"
        "ss.end               u1, zero, %[N], %[M] \n"

        "ss.sta.ld.b.v.m      u2, %[data] \n"
        "ss.app               u2, zero, %[M], %[one] \n"
        "ss.app.mod.siz.dec.2 u2, %[one] \n"
        "ss.app               u2, zero, %[M], zero \n"
        "ss.end               u2, zero, %[N], %[M] \n"

        "ss.sta.st.b          u3, %[cov] \n"
        "ss.app               u3, zero, %[M], %[M] \n"
        "ss.app.mod.ofs.inc.2 u3, %[one] \n"
        "ss.app.mod.siz.dec.2 u3, %[one] \n"
        "ss.end               u3, zero, %[M], %[one] \n"

        "ss.sta.st.b          u4, %[cov] \n"
        "ss.app               u4, zero, %[M], %[one] \n"
        "ss.app.mod.ofs.inc.2 u4, %[M] \n"
        "ss.app.mod.siz.dec.2 u4, %[one] \n"
        "ss.end               u4, zero, %[M], %[M] \n"
        
        "so.v.mvsv.b u5, %[datat_nn] \n"

        ".SLOOP_3%=: \n"
            "so.v.dp.b      u6, zero, p0 \n"

            ".SLOOP_3_0_0%=: \n"
                /*"so.a.mul.sg  u7, u2, u1, p0 \n"
                "so.a.add.sg  u6, u6, u7, p0 \n"*/
                "so.a.mac.sg  u6, u2, u1, p0 \n"
            "so.b.ndc.3 u2, .SLOOP_3_0_0%= \n"

            "so.a.adde.sg   u7, u6, p0 \n"
            "so.a.div.sg    u8, u7, u5, p0 \n"
            "so.v.mv        u3, u8, p0 \n"
            "so.v.mv        u4, u8, p0 \n"
        "so.b.nc u1, .SLOOP_3%= \n"

        "rdinstret %[e] \n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core(int sizeM, int sizeN, DataType datat_n, DataType *data, DataType *cov, DataType *mean) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    int i, j, k;

    /*print data
    printf("\ndata:\n");
    for (int i = 0; i < sizeN; i++) {
        for (int j = 0; j < sizeM; j++) {
            printf(DataFormat("", " "), data[i * sizeM + j]);
        }
        printf("\n");
    }
    printf("\n");*/

    // kernel 1
    for (j = 0; j < sizeM; j++) {
        mean[j] = 0;
        for (i = 0; i < sizeN; i++)
            mean[j] += data[i * sizeM + j];
        mean[j] /= datat_n;
    }

    /* print mean
    printf("\nmean:\n");
    for (int i = 0; i < sizeM; i++) {
        printf(DataFormat("", " "), mean[i]);
    }
    printf("\n");*/

    // kernel 2
    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeM; j++)
            data[i * sizeM + j] -= mean[j];

    /* print data
    printf("\ndata:\n");
    for (int i = 0; i < sizeN; i++) {
        for (int j = 0; j < sizeM; j++) {
            printf(DataFormat("", " "), data[i * sizeM + j]);
        }
        printf("\n");
    }*/

    // kernel 3
    for (i = 0; i < sizeM; i++)
        for (j = i; j < sizeM; j++) {
            cov[i * sizeM + j] = 0;
            for (k = 0; k < sizeN; k++)
                cov[i * sizeM + j] += data[k * sizeM + i] * data[k * sizeM + j];
            cov[i * sizeM + j] /= (DataType)(datat_n - 1.0);
            cov[j * sizeM + i] = cov[i * sizeM + j];

            /* print cov
            printf("\ncov at [%d][%d]:\n", i, j);
            for (int i = 0; i < sizeM; i++) {
                for (int j = 0; j < sizeM; j++)
                    printf( DataFormat("", " "), cov[i*sizeM + j]);
                printf("\n");
            }*/
        }
    
    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE

