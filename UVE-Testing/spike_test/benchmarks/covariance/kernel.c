#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(int sizeM /* %0 */, int sizeN /* %1 */, DataType datat_n /* %2 */, DataType *data /* %3 */, DataType *cov /* %4 */, DataType *mean /* %5 */) {
    // kernel 1
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.sta.ld.d    u1, %[data], %[M], %[one] \t\n"
        "ss.end         u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.st.d u2, %[mean], %[M], %[one] \t\n"
        
        "so.v.mvsv.d u3, %[datat_n] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.d    u4, zero \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.adde.acc.fp   u4, u1, p0 \t\n"
            "so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"

            "so.a.div.fp    u2, u4, u3, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n"
        
        // kernel 2
  
        "ss.sta.st.d    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.d    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.d    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        
        ".SLOOP_2%=: \t\n"
            "so.a.sub.fp  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n"

        // kernel 3
  
        "ss.sta.ld.d        u1, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.app             u1, zero, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u1 \t\n"

        "ss.sta.ld.d        u2, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], zero \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.st.d        u3, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.end             u3, zero, %[M], %[one] \t\n"

        "ss.sta.st.d        u4, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[M] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[M] \t\n"
        
        "so.v.mvsv.d u5, %[datat_nn] \t\n"

        ".SLOOP_3%=: \t\n"
            "so.v.dp.d      u6, zero, p0 \t\n"

            ".SLOOP_3_0_0%=: \t\n"
                "so.a.mul.fp  u7, u2, u1, p0 \t\n"
                "so.a.add.fp  u6, u6, u7, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_3_0_0%= \t\n"

            "so.a.adde.fp   u7, u6, p0 \t\n"
            "so.a.div.fp    u8, u7, u5, p0 \t\n"
            "so.v.mv        u3, u8, p0 \t\n"
            "so.v.mv        u4, u8, p0 \t\n"
        "so.b.nc u1, .SLOOP_3%= \t\n"

        "rdinstret %[e] \t\n"
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
        "rdinstret %[s] \t\n"

        "ss.sta.ld.w    u1, %[data], %[M], %[one] \t\n"
        "ss.end         u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.st.w u2, %[mean], %[M], %[one] \t\n"
        
        "so.v.mvsv.w u3, %[datat_n] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.w    u4, zero \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.adde.acc.fp   u4, u1, p0 \t\n"
            "so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"

            "so.a.div.fp    u2, u4, u3, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n"
        
        // kernel 2
  
        "ss.sta.st.w    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.w    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.w    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        
        ".SLOOP_2%=: \t\n"
            "so.a.sub.fp  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n"

        // kernel 3
  
        "ss.sta.ld.w        u1, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.app             u1, zero, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u1 \t\n"

        "ss.sta.ld.w        u2, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], zero \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.st.w        u3, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.end             u3, zero, %[M], %[one] \t\n"

        "ss.sta.st.w        u4, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[M] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[M] \t\n"
        
        "so.v.mvsv.w u5, %[datat_nn] \t\n"

        ".SLOOP_3%=: \t\n"
            "so.v.dp.w      u6, zero, p0 \t\n"

            ".SLOOP_3_0_0%=: \t\n"
                "so.a.mul.fp  u7, u2, u1, p0 \t\n"
                "so.a.add.fp  u6, u6, u7, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_3_0_0%= \t\n"

            "so.a.adde.fp   u7, u6, p0 \t\n"
            "so.a.div.fp    u8, u7, u5, p0 \t\n"
            "so.v.mv        u3, u8, p0 \t\n"
            "so.v.mv        u4, u8, p0 \t\n"
        "so.b.nc u1, .SLOOP_3%= \t\n"

        "rdinstret %[e] \t\n"
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
        "rdinstret %[s] \t\n"

        "ss.sta.ld.w    u1, %[data], %[M], %[one] \t\n"
        "ss.end         u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.st.w u2, %[mean], %[M], %[one] \t\n"
        
        "so.v.mvsv.w u3, %[datat_n] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.w    u4, zero \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.adde.acc.sg   u4, u1, p0 \t\n"
            "so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"

            "so.a.div.sg    u2, u4, u3, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n"
        
        // kernel 2
  
        "ss.sta.st.w    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.w    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.w    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        
        ".SLOOP_2%=: \t\n"
            "so.a.sub.sg  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n"

        // kernel 3
  
        "ss.sta.ld.w        u1, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.app             u1, zero, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u1 \t\n"

        "ss.sta.ld.w        u2, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], zero \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.st.w        u3, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.end             u3, zero, %[M], %[one] \t\n"

        "ss.sta.st.w        u4, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[M] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[M] \t\n"
        
        "so.v.mvsv.w u5, %[datat_nn] \t\n"

        ".SLOOP_3%=: \t\n"
            "so.v.dp.w      u6, zero, p0 \t\n"

            ".SLOOP_3_0_0%=: \t\n"
                "so.a.mul.sg  u7, u2, u1, p0 \t\n"
                "so.a.add.sg  u6, u6, u7, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_3_0_0%= \t\n"

            "so.a.adde.sg   u7, u6, p0 \t\n"
            "so.a.div.sg    u8, u7, u5, p0 \t\n"
            "so.v.mv        u3, u8, p0 \t\n"
            "so.v.mv        u4, u8, p0 \t\n"
        "so.b.nc u1, .SLOOP_3%= \t\n"

        "rdinstret %[e] \t\n"
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
        "rdinstret %[s] \t\n"

        "ss.sta.ld.h    u1, %[data], %[M], %[one] \t\n"
        "ss.end         u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.st.h u2, %[mean], %[M], %[one] \t\n"
        
        "so.v.mvsv.h u3, %[datat_n] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.h    u4, zero \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.adde.acc.sg   u4, u1, p0 \t\n"
            "so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"

            "so.a.div.sg    u2, u4, u3, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n"
        
        // kernel 2
  
        "ss.sta.st.h    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.h    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.h    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        
        ".SLOOP_2%=: \t\n"
            "so.a.sub.sg  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n"

        // kernel 3
  
        "ss.sta.ld.h        u1, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.app             u1, zero, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u1 \t\n"

        "ss.sta.ld.h        u2, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], zero \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.st.h        u3, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.end             u3, zero, %[M], %[one] \t\n"

        "ss.sta.st.h        u4, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[M] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[M] \t\n"
        
        "so.v.mvsv.h u5, %[datat_nn] \t\n"

        ".SLOOP_3%=: \t\n"
            "so.v.dp.h      u6, zero, p0 \t\n"

            ".SLOOP_3_0_0%=: \t\n"
                "so.a.mul.sg  u7, u2, u1, p0 \t\n"
                "so.a.add.sg  u6, u6, u7, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_3_0_0%= \t\n"

            "so.a.adde.sg   u7, u6, p0 \t\n"
            "so.a.div.sg    u8, u7, u5, p0 \t\n"
            "so.v.mv        u3, u8, p0 \t\n"
            "so.v.mv        u4, u8, p0 \t\n"
        "so.b.nc u1, .SLOOP_3%= \t\n"

        "rdinstret %[e] \t\n"
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
        "rdinstret %[s] \t\n"

        "ss.sta.ld.b    u1, %[data], %[M], %[one] \t\n"
        "ss.end         u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.st.b u2, %[mean], %[M], %[one] \t\n"
        
        "so.v.mvsv.b u3, %[datat_n] \t\n"

        ".SLOOP_1%=: \t\n"
            "so.v.mvsv.b    u4, zero \t\n"

            ".SLOOP_1_0%=: \t\n"
                "so.a.adde.acc.sg   u4, u1, p0 \t\n"
            "so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"

            "so.a.div.sg    u2, u4, u3, p0 \t\n"
        "so.b.nc u1, .SLOOP_1%= \t\n"
        
        // kernel 2
  
        "ss.sta.st.b    u1, %[data], %[N], %[M] \t\n"
        "ss.end         u1, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u1 \t\n"

        "ss.sta.ld.b    u2, %[mean], %[N], zero \t\n"
        "ss.end         u2, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u2 \t\n"

        "ss.sta.ld.b    u3, %[data], %[N], %[M] \t\n"
        "ss.end         u3, zero, %[M], %[one] \t\n"
        "ss.cfg.vec     u3 \t\n"
        
        ".SLOOP_2%=: \t\n"
            "so.a.sub.sg  u1, u3, u2, p0 \t\n"
        "so.b.nc u1, .SLOOP_2%= \t\n"

        // kernel 3
  
        "ss.sta.ld.b        u1, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u1, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u1, %[M], %[one] \t\n"
        "ss.app             u1, zero, %[M], %[one] \t\n"
        "ss.end             u1, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u1 \t\n"

        "ss.sta.ld.b        u2, %[data], %[M], %[one] \t\n"
        //"ss.app.mod.ofs.inc u2, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u2, %[M], %[one] \t\n"
        "ss.app             u2, zero, %[M], zero \t\n"
        "ss.end             u2, zero, %[N], %[M] \t\n"
        "ss.cfg.vec         u2 \t\n"

        "ss.sta.st.b        u3, %[cov], %[M], %[M] \t\n"
        "ss.app.mod.ofs.inc u3, %[M], %[one] \t\n"
        "ss.app.mod.siz.dec u3, %[M], %[one] \t\n"
        "ss.end             u3, zero, %[M], %[one] \t\n"

        "ss.sta.st.b        u4, %[cov], %[M], %[one] \t\n"
        "ss.app.mod.ofs.inc u4, %[M], %[M] \t\n"
        "ss.app.mod.siz.dec u4, %[M], %[one] \t\n"
        "ss.end             u4, zero, %[M], %[M] \t\n"
        
        "so.v.mvsv.b u5, %[datat_nn] \t\n"

        ".SLOOP_3%=: \t\n"
            "so.v.dp.b      u6, zero, p0 \t\n"

            ".SLOOP_3_0_0%=: \t\n"
                "so.a.mul.sg  u7, u2, u1, p0 \t\n"
                "so.a.add.sg  u6, u6, u7, p0 \t\n"
            "so.b.ndc.1 u2, .SLOOP_3_0_0%= \t\n"

            "so.a.adde.sg   u7, u6, p0 \t\n"
            "so.a.div.sg    u8, u7, u5, p0 \t\n"
            "so.v.mv        u3, u8, p0 \t\n"
            "so.v.mv        u4, u8, p0 \t\n"
        "so.b.nc u1, .SLOOP_3%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r" (start), [e] "=&r" (end)
        : [M] "r"(sizeM), [N] "r"(sizeN), [datat_n] "r"(datat_n), [data] "r"(data), [cov] "r"(cov), [mean] "r"(mean),
          [one] "r"(1), [datat_nn] "r"((DataType)(datat_n - 1.0)));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core(int sizeM, int sizeN, DataType datat_n, DataType *data, DataType *cov, DataType *mean) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

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
    
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE

