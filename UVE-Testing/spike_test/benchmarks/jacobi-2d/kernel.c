#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *A, DataType *B) {
  asm volatile(
    "rdinstret %[s] \t\n"

    // A1 stream load
    "ss.sta.ld.d           u1, %[src1a], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u1 \t\n"
    "ss.end                u1, zero, %[snm2], %[one] \t\n"
    // A2 stream load
    "ss.sta.ld.d           u2, %[src1b], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u2 \t\n"
    "ss.end                u2, zero, %[snm2], %[one] \t\n"
    // A3 stream load
    "ss.sta.ld.d           u3, %[src1c], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u3 \t\n"
    "ss.end                u3, zero, %[snm2], %[one] \t\n"
    // A4 stream load
    "ss.sta.ld.d           u4, %[src1d], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u4 \t\n"
    "ss.end                u4, zero, %[snm2], %[one] \t\n"
    // A5 stream load
    "ss.sta.ld.d           u5, %[src1e], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u5 \t\n"
    "ss.end                u5, zero, %[snm2], %[one] \t\n"
    // B stream store
    "ss.sta.st.d           u6, %[src2], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u6 \t\n"
    "ss.end                u6, zero, %[snm2], %[one] \t\n"

    "so.v.dp.d  u7, %[fval], p0\t\n"

    ".loop_1%=: \t\n"
      "so.a.add.fp u8, u1, u2, p0\n\t"
      "so.a.add.fp u9, u3, u4, p0\n\t"
      "so.a.add.fp u10, u8, u5, p0\n\t"
      "so.a.add.fp u11, u10, u9, p0\n\t"
      "so.a.mul.fp u6, u11, u7, p0\n\t"
    "so.b.nc	u1, .loop_1%= \n\t"

    // B1 stream load
    "ss.sta.ld.d           u1, %[src2a], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u1 \t\n"
    "ss.end                u1, zero, %[snm2], %[one] \t\n"
    // B2 stream load
    "ss.sta.ld.d           u2, %[src2b], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u2 \t\n"
    "ss.end                u2, zero, %[snm2], %[one] \t\n"
    // B3 stream load
    "ss.sta.ld.d           u3, %[src2c], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u3 \t\n"
    "ss.end                u3, zero, %[snm2], %[one] \t\n"
    // B4 stream load
    "ss.sta.ld.d           u4, %[src2d], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u4 \t\n"
    "ss.end                u4, zero, %[snm2], %[one] \t\n"
    // B5 stream load
    "ss.sta.ld.d           u5, %[src2e], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u5 \t\n"
    "ss.end                u5, zero, %[snm2], %[one] \t\n"
    // A stream store
    "ss.sta.st.d           u6, %[src1], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u6 \t\n"
    "ss.end                u6, zero, %[snm2], %[one] \t\n"

    ".loop_2%=: \t\n"
      "so.a.add.fp u8, u1, u2, p0\n\t"
      "so.a.add.fp u9, u3, u4, p0\n\t"
      "so.a.add.fp u10, u8, u5, p0\n\t"
      "so.a.add.fp u11, u10, u9, p0\n\t"
      "so.a.mul.fp u6, u11, u7, p0\n\t"
    "so.b.nc	u1, .loop_2%= \n\t"

    "rdinstret %[e] \t\n"

    : [s] "=&r" (start), [e] "=&r" (end)
    : [src1a] "r" (A+SIZE+1),
      [src1b] "r" (A+SIZE),
      [src1c] "r" (A+SIZE+2),
      [src1d] "r" (A+SIZE+SIZE+1),
      [src1e] "r" (A+1),
      [src1] "r" (A+1+SIZE),
      [src2a] "r" (B+SIZE+1),
      [src2b] "r" (B+SIZE),
      [src2c] "r" (B+SIZE+2),
      [src2d] "r" (B+SIZE+SIZE+1),
      [src2e] "r" (B+1),
      [src2] "r" (B+1+SIZE),
      [snm2] "r"(SIZE - 2),
      [sn] "r"(SIZE),
      [one] "r" (1),
      [fval] "r" ((DataType)0.2));

      printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *A, DataType *B) {
  asm volatile(
    "rdinstret %[s] \t\n"

    // A1 stream load
    "ss.sta.ld.w           u1, %[src1a], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u1 \t\n"
    "ss.end                u1, zero, %[snm2], %[one] \t\n"
    // A2 stream load
    "ss.sta.ld.w           u2, %[src1b], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u2 \t\n"
    "ss.end                u2, zero, %[snm2], %[one] \t\n"
    // A3 stream load
    "ss.sta.ld.w           u3, %[src1c], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u3 \t\n"
    "ss.end                u3, zero, %[snm2], %[one] \t\n"
    // A4 stream load
    "ss.sta.ld.w           u4, %[src1d], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u4 \t\n"
    "ss.end                u4, zero, %[snm2], %[one] \t\n"
    // A5 stream load
    "ss.sta.ld.w           u5, %[src1e], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u5 \t\n"
    "ss.end                u5, zero, %[snm2], %[one] \t\n"
    // B stream store
    "ss.sta.st.w           u6, %[src2], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u6 \t\n"
    "ss.end                u6, zero, %[snm2], %[one] \t\n"

    "so.v.dp.w  u7, %[fval], p0\t\n"

    ".loop_1%=: \t\n"
      "so.a.add.fp u8, u1, u2, p0\n\t"
      "so.a.add.fp u9, u3, u4, p0\n\t"
      "so.a.add.fp u10, u8, u5, p0\n\t"
      "so.a.add.fp u11, u10, u9, p0\n\t"
      "so.a.mul.fp u6, u11, u7, p0\n\t"
    "so.b.nc	u1, .loop_1%= \n\t"

    // B1 stream load
    "ss.sta.ld.w           u1, %[src2a], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u1 \t\n"
    "ss.end                u1, zero, %[snm2], %[one] \t\n"
    // B2 stream load
    "ss.sta.ld.w           u2, %[src2b], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u2 \t\n"
    "ss.end                u2, zero, %[snm2], %[one] \t\n"
    // B3 stream load
    "ss.sta.ld.w           u3, %[src2c], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u3 \t\n"
    "ss.end                u3, zero, %[snm2], %[one] \t\n"
    // B4 stream load
    "ss.sta.ld.w           u4, %[src2d], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u4 \t\n"
    "ss.end                u4, zero, %[snm2], %[one] \t\n"
    // B5 stream load
    "ss.sta.ld.w           u5, %[src2e], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u5 \t\n"
    "ss.end                u5, zero, %[snm2], %[one] \t\n"
    // A stream store
    "ss.sta.st.w           u6, %[src1], %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u6 \t\n"
    "ss.end                u6, zero, %[snm2], %[one] \t\n"

    ".loop_2%=: \t\n"
      "so.a.add.fp u8, u1, u2, p0\n\t"
      "so.a.add.fp u9, u3, u4, p0\n\t"
      "so.a.add.fp u10, u8, u5, p0\n\t"
      "so.a.add.fp u11, u10, u9, p0\n\t"
      "so.a.mul.fp u6, u11, u7, p0\n\t"
    "so.b.nc	u1, .loop_2%= \n\t"

    "rdinstret %[e] \t\n"

    : [s] "=&r" (start), [e] "=&r" (end)
    : [src1a] "r" (A+SIZE+1),
      [src1b] "r" (A+SIZE),
      [src1c] "r" (A+SIZE+2),
      [src1d] "r" (A+SIZE+SIZE+1),
      [src1e] "r" (A+1),
      [src1] "r" (A+1+SIZE),
      [src2a] "r" (B+SIZE+1),
      [src2b] "r" (B+SIZE),
      [src2c] "r" (B+SIZE+2),
      [src2d] "r" (B+SIZE+SIZE+1),
      [src2e] "r" (B+1),
      [src2] "r" (B+1+SIZE),
      [snm2] "r"(SIZE - 2),
      [sn] "r"(SIZE),
      [one] "r" (1),
      [fval] "r" ((DataType)0.2));

      printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE

#if defined(I_TYPE) || defined(H_TYPE) || defined(B_TYPE)
#error *DataType not available for this benchmark.*
#endif // I_TYPE || H_TYPE || B_TYPE

#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType *A, DataType *B) {
  asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start)); 

  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      B[i*SIZE+j] = 0.2  * (A[i*SIZE+j] + A[i*SIZE+j-1] + A[i*SIZE+j+1] + A[(i+1)*SIZE+j] + A[(i-1)*SIZE+j]);
    }
  }
  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      A[i*SIZE+j] = 0.2  * (B[i*SIZE+j] + B[i*SIZE+j-1] + B[i*SIZE+j+1] + B[(i+1)*SIZE+j] + B[(i-1)*SIZE+j]);
    }
  }

  asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE


#ifdef RUN_BLANK
void core(DataType *A, DataType *B) {
}
#endif // RUN_BLANK
