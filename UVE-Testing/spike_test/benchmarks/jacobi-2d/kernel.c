#include "Functions.h"


#ifdef RUN_UVE
void core(DataType A[SIZE][SIZE], DataType B[SIZE][SIZE], DataType ct) {
  asm volatile(
    // A1 stream load
    "ss.sta.ld.d           u1, %[src1a], %[sn], %[one] \t\n"
    "ss.end                u1, zero, %[sn], %[sn] \t\n"
    // A2 stream load
    "ss.sta.ld.d           u2, %[src1b], %[sn], %[one] \t\n"
    "ss.end                u2, zero, %[sn], %[sn] \t\n"
    // A3 stream load
    "ss.sta.ld.d           u3, %[src1c], %[sn], %[one] \t\n"
    "ss.end                u3, zero, %[sn], %[sn] \t\n"
    // A4 stream load
    "ss.sta.ld.d           u4, %[src1d], %[sn], %[one] \t\n"
    "ss.end                u4, zero, %[sn], %[sn] \t\n"
    // A5 stream load
    "ss.sta.ld.d           u5, %[src1e], %[sn], %[one] \t\n"
    "ss.end                u5, zero, %[sn], %[sn] \t\n"
    // B stream store
    "ss.sta.st.d           u6, %[src2], %[sn], %[one] \t\n"
    "ss.end                u6, zero, %[sn], %[sn] \t\n"
    "so.v.dp.d  u7, %[fval], p0\t\n"
    :
    : [src1a] "r" (A+SIZE+1),
      [src1b] "r" (A+SIZE),
      [src1c] "r" (A+SIZE+2),
      [src1d] "r" (A+SIZE+SIZE+1),
      [src1e] "r" (A+1),
      [src2] "r" (B+1+SIZE),
      [sn] "r"(SIZE - 2),
      [one] "r" (1), [fval] "r" (ct)
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
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType A[SIZE][SIZE], DataType B[SIZE][SIZE], DataType ct) {
  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      B[i][j] = ct  * (A[i][j] + A[i][j-1] + A[i][j+1] + A[i+1][j] + A[i-1][j]);
    }
  } 
  for (int i = 1; i < SIZE - 1; i++) {
    for (int j = 1; j < SIZE - 1; j++) {
      A[i][j] = ct  * (B[i][j] + B[i][j-1] + B[i][j+1] + B[i+1][j] + B[i-1][j]);
    }
  }
}
#endif // RUN_SIMPLE
