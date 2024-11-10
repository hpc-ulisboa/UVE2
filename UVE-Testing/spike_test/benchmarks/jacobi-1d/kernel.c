#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(

        "ss.sta.ld.d.v u1, %[src1a] \n"
        "ss.end        u1, zero, %[sn], %[one] \n"

        "ss.sta.ld.d.v u2, %[src1b] \n"
        "ss.end        u2, zero, %[sn], %[one] \n"

        "ss.sta.ld.d.v u3, %[src1c] \n"
        "ss.end        u3, zero, %[sn], %[one] \n"

        "ss.sta.st.d.v u4, %[src2] \n"
        "ss.end        u4, zero, %[sn], %[one] \n"

        "so.v.dp.d  u5, %[ct], p0 \n"

        ".uve_loop1%=: \n"
			"so.a.add.fp   u10, u1, u2, p0 \n"
			"so.a.add.fp   u10, u10, u3, p0 \n"
			"so.a.mul.fp   u4, u10, u5, p0 \n"
        "so.b.nc  u1,  .uve_loop1%= \n"

        "ss.sta.ld.d.v u1, %[src2a] \n"
        "ss.end        u1, zero, %[sn], %[one] \n"

        "ss.sta.ld.d.v u2, %[src2b] \n"
        "ss.end        u2, zero, %[sn], %[one] \n"

        "ss.sta.ld.d.v u3, %[src2c] \n"
        "ss.end        u3, zero, %[sn], %[one] \n"

        "ss.sta.st.d.v u4, %[src1] \n"
        "ss.end        u4, zero, %[sn], %[one] \n"

        ".uve_loop2%=: \n"
			"so.a.add.fp   u10, u1,  u2,  p0 \n"
			"so.a.add.fp   u10, u10, u3,  p0 \n"
			"so.a.mul.fp   u4,  u10, u5, p0 \n"
        "so.b.nc  u1,  .uve_loop2%= \n"

        :: [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(

        "ss.sta.ld.w.v u1, %[src1a] \n"
        "ss.end        u1, zero, %[sn], %[one] \n"

        "ss.sta.ld.w.v u2, %[src1b] \n"
        "ss.end        u2, zero, %[sn], %[one] \n"

        "ss.sta.ld.w.v u3, %[src1c] \n"
        "ss.end        u3, zero, %[sn], %[one] \n"

        "ss.sta.st.w.v u4, %[src2] \n"
        "ss.end        u4, zero,%[sn], %[one] \n"

        "so.v.dp.w u5, %[ct], p0 \n"

        ".uve_loop1%=: \n"
			"so.a.add.fp   u10, u1,  u2,  p0 \n"
			"so.a.add.fp   u10, u10, u3,  p0 \n"
			"so.a.mul.fp   u4,  u10, u5, p0 \n"
        "so.b.nc  u1,  .uve_loop1%= \n"

        "ss.sta.ld.w.v u1, %[src2a] \n"
        "ss.end        u1, zero, %[sn], %[one] \n"

        "ss.sta.ld.w.v u2, %[src2b] \n"
        "ss.end        u2, zero, %[sn], %[one] \n"

        "ss.sta.ld.w.v u3, %[src2c] \n"
        "ss.end        u3, zero, %[sn], %[one] \n"

        "ss.sta.st.w.v u4, %[src1] \n"
        "ss.end        u4, zero,%[sn], %[one] \n"

        ".uve_loop2%=: \n"
			"so.a.add.fp   u10, u1,  u2,  p0 \n"
			"so.a.add.fp   u10, u10, u3,  p0 \n"
			"so.a.mul.fp   u4,  u10, u5, p0 \n"
        "so.b.nc  u1,  .uve_loop2%= \n"

        :: [src1a] "r"(A), [src1b] "r"(A + 1), [src1c] "r"(A + 2), [src1] "r"(A + 1),
          [src2a] "r"(B), [src2b] "r"(B + 1), [src2c] "r"(B + 2), [src2] "r"(B + 1),
          [sn] "r"(SIZE - 2), [one] "r"(1), [ct] "r"(ct));

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE

#if defined(I_TYPE) || defined(H_TYPE) || defined(B_TYPE)
#error *DataType not available for this benchmark.*
#endif // I_TYPE || H_TYPE || B_TYPE

#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType A[SIZE], DataType B[SIZE], DataType ct) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
 
    for (int i = 1; i < SIZE - 1; ++i)
        B[i] = ct * (A[i - 1] + A[i] + A[i + 1]);

    for (int i = 1; i < SIZE - 1; ++i)
        A[i] = ct * (B[i - 1] + B[i] + B[i + 1]);

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE
