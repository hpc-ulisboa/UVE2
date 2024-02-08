#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.ld.d    u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d    u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.d    u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.d    u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.d  u5, %[ct], p0\t\n"

        ".uve_loop1%=: \t\n"
			"so.a.add.fp   u10, u1,  u2,  p0 \t\n"
			"so.a.add.fp   u10, u10, u3,  p0 \t\n"
			"so.a.mul.fp   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop1%= \t\n"

        "ss.ld.d    u1, %[src2a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d    u2, %[src2b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.d    u3, %[src2c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.d    u4, %[src1],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        ".uve_loop2%=: \t\n"
			"so.a.add.fp   u10, u1,  u2,  p0 \t\n"
			"so.a.add.fp   u10, u10, u3,  p0 \t\n"
			"so.a.mul.fp   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop2%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r"(start), [e] "=&r"(end)
        : [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.ld.w   u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.w   u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.w   u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.w   u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.w u5, %[ct], p0\t\n"

        ".uve_loop1%=: \t\n"
			"so.a.add.fp   u10, u1,  u2,  p0 \t\n"
			"so.a.add.fp   u10, u10, u3,  p0 \t\n"
			"so.a.mul.fp   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop1%= \t\n"

        "ss.ld.w   u1, %[src2a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.w   u2, %[src2b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.w   u3, %[src2c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.w   u4, %[src1],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        ".uve_loop2%=: \t\n"
			"so.a.add.fp   u10, u1,  u2,  p0 \t\n"
			"so.a.add.fp   u10, u10, u3,  p0 \t\n"
			"so.a.mul.fp   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop2%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r"(start), [e] "=&r"(end)
        : [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.ld.w   u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.w   u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.w   u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.w   u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.w u5, %[ct], p0\t\n"

        ".uve_loop1%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop1%= \t\n"

        "ss.ld.w   u1, %[src2a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.w   u2, %[src2b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.w   u3, %[src2c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.w   u4, %[src1],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        ".uve_loop2%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop2%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r"(start), [e] "=&r"(end)
        : [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.ld.h   u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.h   u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.h   u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.h   u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.h u5, %[ct], p0\t\n"

        ".uve_loop1%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop1%= \t\n"

        "ss.ld.h   u1, %[src2a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.h   u2, %[src2b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.h   u3, %[src2c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.h   u4, %[src1],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        ".uve_loop2%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop2%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r"(start), [e] "=&r"(end)
        : [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile(
        "rdinstret %[s] \t\n"

        "ss.ld.b   u1, %[src1a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.b   u2, %[src1b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.b   u3, %[src1c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.b   u4, %[src2],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        "so.v.dp.b u5, %[ct], p0\t\n"

        ".uve_loop1%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop1%= \t\n"

        "ss.ld.b   u1, %[src2a], %[sn], %[one] \t\n"
        "ss.cfg.vec u1 \t\n"
        "ss.ld.b   u2, %[src2b], %[sn], %[one] \t\n"
        "ss.cfg.vec u2 \t\n"
        "ss.ld.b   u3, %[src2c], %[sn], %[one] \t\n"
        "ss.cfg.vec u3 \t\n"
        "ss.st.b   u4, %[src1],  %[sn], %[one] \t\n"
        "ss.cfg.vec u4 \t\n"

        ".uve_loop2%=: \t\n"
			"so.a.add.sg   u10, u1,  u2,  p0 \t\n"
			"so.a.add.sg   u10, u10, u3,  p0 \t\n"
			"so.a.mul.sg   u4,  u10, u5, p0 \t\n"
        "so.b.nc  u1,  .uve_loop2%= \t\n"

        "rdinstret %[e] \t\n"
        : [s] "=&r"(start), [e] "=&r"(end)
        : [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
 
    for (int i = 1; i < SIZE - 1; ++i)
        B[i] = ct * (A[i - 1] + A[i] + A[i + 1]);

    for (int i = 1; i < SIZE - 1; ++i)
        A[i] = ct * (B[i - 1] + B[i] + B[i + 1]);

    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE


#ifdef RUN_BLANK
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
}
#endif // RUN_BLANK
