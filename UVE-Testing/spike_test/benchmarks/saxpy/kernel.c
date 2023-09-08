#include "Functions.h"


#ifdef RUN_UVE
void core(DataType dest[SIZE], DataType src[SIZE], DataType value) {
  asm volatile(
    "ss.ld.d u1, %[src1], %[size], %[stride] \t\n"
    "ss.ld.d u10, %[src2], %[size], %[stride] \t\n"
    "ss.st.d u3, %[dest], %[size], %[stride] \t\n"
    "so.v.dp.d u4, %[value], p0\t\n"
    : 
    : [src1] "r"(src),
      [src2] "r"(dest),
      [dest] "r"(dest),
      [size] "r"(SIZE),
      [stride] "r" (1),
      [value] "r" (value));

	asm volatile(
		".uve_loop%=: \t\n"
      "so.a.mul.fp u5, u1, u4, p0\n\t"
      "so.a.add.fp u3, u10, u5, p0 \n\t"
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