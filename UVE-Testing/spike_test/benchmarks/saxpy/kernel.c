#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        //"csrrs %[start], instret, x0 \t\n" // start counting instructions after values have been loaded into registers
        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        "ss.ld.d u1, %[src1], %[size], %[stride] \t\n"
        "ss.cfg.vec u1 \t\n"

        "ss.ld.d u2, %[src2], %[size], %[stride] \t\n"
        "ss.cfg.vec u2 \t\n"

        "ss.st.d u3, %[dest], %[size], %[stride] \t\n"
        "ss.cfg.vec u3 \t\n"

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