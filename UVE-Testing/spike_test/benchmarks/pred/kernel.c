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
		"so.p.egt.fp p5, u2, u3, p0 \t\n"
		"so.v.dp.w u6, %[three], p2 \t\n"
		"so.v.mv.stream u1, u6, p0 \t\n"
		".uve_loop%= : \t\n"
            "so.a.add.fp u1, u4, u5, p5 \t\n"
		"so.b.nc u4, .uve_loop%= \t\n"
		 :: [three] "r" ((float)3.0));
}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i]*3;
	}
}
#endif // RUN_SIMPLE