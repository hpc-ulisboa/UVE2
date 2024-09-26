#include "Functions.h"

#ifdef RUN_UVE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
    asm volatile(
        "ss.ld.d u1, %[src1], %[size], %[stride] \t\n"
        "ss.cfg.vec u1 \t\n"

        "ss.ld.d u2, %[src2], %[size], %[stride] \t\n"
        "ss.cfg.vec u2 \t\n"

        "ss.st.d u3, %[dest], %[size], %[stride] \t\n"
        "ss.cfg.vec u3 \t\n"

        "so.v.dp.d u4, %[value], p0\t\n"
        :
        : [src1] "r"(src),
          [src2] "r"(dest),
          [dest] "r"(dest),
          [size] "r"(SIZE),
          [stride] "r"(1),
          [value] "r"(value));

    asm volatile(
        ".uve_loop%=: \t\n"
			"so.a.mul.fp u5, u1, u4, p0\n\t"
            "so.p.vr p4, u1, p0 \n\t"
            "so.p.one p5, p0 \n\t"
            "so.p.zero p5, p0 \n\t"
			"so.a.add.fp u3, u2, u5, p0 \n\t"
        "so.b.nc u1, .uve_loop%= \n\t" :::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE], DataType A) {
    for (int i = 0; i < SIZE; i++) {
        dest[i] += src[i] * A;
    }
}
#endif // RUN_SIMPLE