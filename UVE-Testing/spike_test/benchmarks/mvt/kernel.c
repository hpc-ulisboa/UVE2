#include "Functions.h"

#ifdef RUN_UVE

void configuration1(DataType *A, DataType *x_1, DataType *y_1, uint64_t sizeN){
    asm volatile(
		// A stream load
		"ss.sta.ld.w u4, %[A], %[sizeN], %[one] \t\n"
		"ss.cfg.vec u4 \t\n"
		"ss.end u4, zero,  %[sizeN], %[sizeN] \t\n"

		// y_1 stream load
		"ss.sta.ld.w u5, %[y_1], %[sizeN], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"
		"ss.end u5, zero, %[sizeN], zero \t\n"

		// x_1 stream load
		//"ss.ld.w u7, %[x_1], %[sizeN], %[one] \t\n"
		
		//until scalar streams are defined:
		"ss.ld.w u7, %[x_1], %[one], zero \t\n"
		"ss.cfg.vec u7 \t\n"
		"ss.end u7, zero, %[sizeN], %[one] \t\n"

		// x_1 stream store
		"ss.st.w u1, %[x_1], %[sizeN], %[one] \t\n"

		:
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		[sizeN] "r"(sizeN), [one] "r" (1)
	);
}

void configuration2(DataType *A, DataType *x_2, DataType *y_2, uint64_t sizeN){
	asm volatile(
		// A stream load
		"ss.sta.ld.w u4, %[A], %[sizeN], %[sizeN] \t\n"
		"ss.cfg.vec u4 \t\n"
		"ss.end u4, zero, %[sizeN], %[one] \t\n"

		// y_2 stream load
		"ss.sta.ld.w u5, %[y_2], %[sizeN], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"
		"ss.end u5, zero, %[sizeN], zero \t\n"

		// x_2 stream load
		//"ss.ld.w u7, %[x_2], %[sizeN], %[one] \t\n"

		//until scalar streams are defined:
		"ss.ld.w u7, %[x_2], %[one], zero \t\n"
		"ss.cfg.vec u7 \t\n"
		"ss.end u7, zero, %[sizeN], %[one] \t\n"

		// x_2 stream store
		"ss.st.w u1, %[x_2], %[sizeN], %[one] \t\n"

		:
		: [A] "r" (A), [y_2] "r" (y_2), [x_2] "r" (x_2), 
		[sizeN] "r"(sizeN), [one] "r" (1)
	);
}

void computation(){
	asm volatile(
        ".SLOOP_1%=: \t\n"
			"so.v.dp.w u2, zero, p0 \t\n"

			".SLOOP_1_0%=: \t\n"
				"so.a.mul.fp  u3, u4, u5, p0 \t\n"
				"so.a.add.fp  u2, u2, u3, p0 \t\n"
			"so.b.ndc.1 u4, .SLOOP_1_0%= \t\n"

			"so.a.adde.fp  u3, u2, p0 \t\n"
			"so.a.add.fp  u1, u7, u3, p0 \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"
		:::
    );
}

void core(DataType *src1, DataType *src2, DataType *src3, DataType *src4, DataType *src5, uint64_t sizeN) {
	configuration1(src1, src2, src4, sizeN);
	computation();
	configuration2(src1, src3, src5, sizeN);
	computation();
}

#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType* A, DataType* x_1, DataType* x_2, DataType* y_1, DataType* y_2, uint64_t sizeN) {
    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_1[i] = x_1[i] + A[i*sizeN+j] * y_1[j];

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_2[i] = x_2[i] + A[j*sizeN+i] * y_2[j];
}

#endif // RUN_SIMPLE