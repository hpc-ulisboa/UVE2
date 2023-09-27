#include "Functions.h"


#ifdef RUN_UVE
void core_uve(DataType *A, DataType *B) {
  asm volatile(
    // A1 stream load
    "ss.sta.ld.d           u1, %[src1a], %[snm2], %[one] \t\n"
    "ss.end                u1, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u1 \t\n"
    // A2 stream load
    "ss.sta.ld.d           u2, %[src1b], %[snm2], %[one] \t\n"
    "ss.end                u2, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u2 \t\n"
    // A3 stream load
    "ss.sta.ld.d           u3, %[src1c], %[snm2], %[one] \t\n"
    "ss.end                u3, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u3 \t\n"
    // A4 stream load
    "ss.sta.ld.d           u4, %[src1d], %[snm2], %[one] \t\n"
    "ss.end                u4, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u4 \t\n"
    // A5 stream load
    "ss.sta.ld.d           u5, %[src1e], %[snm2], %[one] \t\n"
    "ss.end                u5, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u5 \t\n"
    // B stream store
    "ss.sta.st.d           u6, %[src2], %[snm2], %[one] \t\n"
    "ss.end                u6, zero, %[snm2], %[sn] \t\n"
    "ss.cfg.vec            u6 \t\n"

    "so.v.dp.d  u7, %[fval], p0\t\n"
    :
    : [src1a] "r" (A+SIZE+1),
      [src1b] "r" (A+SIZE),
      [src1c] "r" (A+SIZE+2),
      [src1d] "r" (A+SIZE+SIZE+1),
      [src1e] "r" (A+1),
      [src2] "r" (B+1+SIZE),
      [snm2] "r"(SIZE - 2),
      [sn] "r"(SIZE),
      [one] "r" (1),
      [fval] "r" ((DataType)0.2)
    :);
  asm volatile(
    ".loop_%=: \t\n"
      "so.a.add.fp u8, u1, u2, p0\n\t"
      "so.a.add.fp u9, u3, u4, p0\n\t"
      "so.a.add.fp u10, u8, u5, p0\n\t"
      "so.a.add.fp u11, u10, u9, p0\n\t"
      "so.a.mul.fp u6, u11, u7, p0\n\t"
    "so.b.nc	u1, .loop_%= \n\t"
  :::);
}

void core(DataType *src1, DataType *src2) {
  core_uve(src1, src2);
  core_uve(src2, src1);
}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType *A, DataType *B) {
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
}
#endif // RUN_SIMPLE
