#include "Functions.h"

#ifdef RUN_UVE
void core_uve(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "ss.ld.d    u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d    u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.d    u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.d    u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.d  u5, %[ct], p0\t\n"
        :
        : [src1a] "r"(A), [src1b] "r"(A + 1),
          [src1c] "r"(A + 2), [sn] "r"(SIZE - 2),
          [src2] "r"(B + 1), [one] "r"(1), [ct] "r"(ct)
        :);
    asm volatile(
        ".uve_loop%=: \t\n"
			"so.a.add.fp   u10, u1,  u2,  p0 \t\n"
			"so.a.add.fp   u10, u10, u3,  p0 \t\n"
			"so.a.mul.fp   u4,  u10, u5, p0 \t\n"
        "so.b.nc       u1,  .uve_loop%= \t\n" :::);
}

void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    core_uve(A, B, ct);
    core_uve(B, A, ct);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    for (int i = 1; i < SIZE - 1; ++i)
        B[i] = ct * (A[i - 1] + A[i] + A[i + 1]);

    for (int i = 1; i < SIZE - 1; ++i)
        A[i] = ct * (B[i - 1] + B[i] + B[i + 1]);
}
#endif // RUN_SIMPLE
