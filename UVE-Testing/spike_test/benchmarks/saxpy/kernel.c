#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.sta.ld.d.v u1, %[src1] \t\n"
        "ss.end        u1, zero, %[size], %[stride] \t\n"

        "ss.sta.ld.d.v u2, %[src2] \t\n"
        "ss.end        u2, zero, %[size], %[stride] \t\n"

        "ss.sta.st.d.v u3, %[dest] \t\n"
        "ss.end          u3, zero, %[size], %[stride] \t\n"

        "so.v.dp.d u4, %[value], p0\t\n"

        ".uve_loop%=: \t\n"
			"so.a.mul.fp u5, u1, u4, p0\n\t"
			"so.a.add.fp u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.sta.ld.w.v u1, %[src1] \t\n"
        "ss.end        u1, zero, %[size], %[stride] \t\n"

        "ss.sta.ld.w.v u2, %[src2] \t\n"
        "ss.end        u2, zero, %[size], %[stride] \t\n"

        "ss.sta.st.w.v u3, %[dest] \t\n"
        "ss.end        u3, zero, %[size], %[stride] \t\n"

        "so.v.dp.w u4, %[value], p0\t\n"

        ".uve_loop%=: \t\n"
			"so.a.mul.fp u5, u1, u4, p0\n\t"
			"so.a.add.fp u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.sta.ld.w.v u1, %[src1] \t\n"
        "ss.end        u1, zero, %[size], %[stride] \t\n"

        "ss.sta.ld.w.v u2, %[src2] \t\n"
        "ss.end        u2, zero, %[size], %[stride] \t\n"

        "ss.sta.st.w.v u3, %[dest] \t\n"
        "ss.end        u3, zero, %[size], %[stride] \t\n"

        "so.v.dp.w u4, %[value], p0\t\n"

        ".uve_loop%=: \t\n"
			"so.a.mul.sg u5, u1, u4, p0\n\t"
			"so.a.add.sg u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.sta.ld.h.v u1, %[src1] \t\n"
        "ss.end        u1, zero, %[size], %[stride] \t\n"

        "ss.sta.ld.h.v u2, %[src2] \t\n"
        "ss.end        u2, zero, %[size], %[stride] \t\n"

        "ss.sta.st.h.v u3, %[dest] \t\n"
        "ss.end        u3, zero, %[size], %[stride] \t\n"

        "so.v.dp.h u4, %[value], p0\t\n"

        ".uve_loop%=: \t\n"
			"so.a.mul.sg u5, u1, u4, p0\n\t"
			"so.a.add.sg u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.sta.ld.b.v u1, %[src1] \t\n"
        "ss.end        u1, zero, %[size], %[stride] \t\n"

        "ss.sta.ld.b.v u2, %[src2] \t\n"
        "ss.end        u2, zero, %[size], %[stride] \t\n"

        "ss.sta.st.b.v u3, %[dest] \t\n"
        "ss.end        u3, zero, %[size], %[stride] \t\n"

        "so.v.dp.b u4, %[value], p0\t\n"

        ".uve_loop%=: \t\n"
			"so.a.mul.sg u5, u1, u4, p0\n\t"
			"so.a.add.sg u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" 

        "rdinstret %[end] \t\n"

        : [start] "=&r"(start), [end] "=&r"(end)
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE], DataType A) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    for (int i = 0; i < SIZE; i++) {
        dest[i] += src[i] * A;
    }
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE
