#include "Functions.h"


#ifdef RUN_UVE
void core(DataType dest[SIZE], DataType src1[SIZE], DataType src2[SIZE]) {
	asm volatile(
		"ss.st.w  u1, %[dest], %[size], %[stride] \t\n"
		"ss.ld.w  u2, %[src1], %[size], %[stride] \t\n"
        "ss.ld.w  u3, %[src2], %[size], %[stride] \t\n"
		"ss.ld.w  u4, %[src1], %[size], %[stride] \t\n"
        "ss.ld.w  u5, %[src2], %[size], %[stride] \t\n"
		:: [dest] "r" (dest),
		[src1] "r" (src1),
		[src2] "r" (src2),
		[size] "r" (SIZE),
		[stride] "r" (1));
	asm volatile(
		"so.p.one p2, p0 \t\n"
		"so.a.abs.sg u1, u2, p2 \t\n"
		 :: [three] "r" ((float)3.0));
}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src1[SIZE], DataType src2[SIZE]) {
	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src1[i] > 0 ? src1[i] : -src1[i];
	}
}
#endif // RUN_SIMPLE