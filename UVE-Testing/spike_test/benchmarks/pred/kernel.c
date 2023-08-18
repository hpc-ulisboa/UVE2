#include "Functions.h"


#ifdef RUN_UVE
void core(DataType dest[SIZE], DataType src1[SIZE], DataType src2[SIZE]) {
	asm volatile(
		"ss.st.d  u1, %[dest], %[size], %[stride] \t\n"
		"ss.ld.d  u2, %[src1], %[size], %[stride] \t\n"
        "ss.ld.d  u3, %[src2], %[size], %[stride] \t\n"
		"ss.ld.d  u4, %[src1], %[size], %[stride] \t\n"
        "ss.ld.d  u5, %[src2], %[size], %[stride] \t\n"
		:: [dest] "r" (dest),
		[src1] "r" (src1),
		[src2] "r" (src2),
		[size] "r" (SIZE),
		[stride] "r" (1));
	asm volatile(
		"so.p.zero p2, p0 \t\n"
		"so.p.egt.fp p5, u2, u3, p0 \t\n"
		"li t0, 3 \t\n"
		"so.v.dp.d u1, t0, p5 \t\n"
		//".uve_loop%= : \t\n"
        //    "so.a.add.fp u1, u4, u6, p5 \t\n"
		//"so.b.nc u4, .uve_loop%= \t\n"
		 :::
		 );
}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i]*3;
	}
}
#endif // RUN_SIMPLE