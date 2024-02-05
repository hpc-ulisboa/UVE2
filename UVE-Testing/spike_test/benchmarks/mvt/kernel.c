#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE

void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \t\n"

		// A stream load
		"ss.sta.ld.d u4, %[A], %[sizeN], %[sizeN] \t\n"
		"ss.end u4, zero, %[sizeN], %[one]  \t\n"
		"ss.cfg.vec u4 \t\n"

		// y_1 stream load
		"ss.sta.ld.d u5, %[y_1], %[sizeN], zero \t\n"
		"ss.end u5, zero, %[sizeN], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// x_1 stream load
		"ss.ld.d u7, %[x_1], %[sizeN], %[one] \t\n"
		
		// x_1 stream store
		"ss.st.d u1, %[x_1], %[sizeN], %[one] \t\n"

		".SLOOP_1%=: \t\n"
			"so.v.dp.d u2, zero, p0 \t\n"

			".SLOOP_1_0%=: \t\n"
				"so.a.mul.fp  u3, u4, u5, p0 \t\n"
				"so.a.add.fp  u2, u2, u3, p0 \t\n"
			"so.b.ndc.1 u4, .SLOOP_1_0%= \t\n"

			"so.a.adde.fp  u3, u2, p0 \t\n"
			"so.a.add.fp  u1, u7, u3, p0 \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		// A stream load
		"ss.sta.ld.d u4, %[A], %[sizeN], %[one] \t\n"
		"ss.end u4, zero, %[sizeN], %[sizeN] \t\n"
		"ss.cfg.vec u4 \t\n"

		// y_2 stream load
		"ss.sta.ld.d u5, %[y_2], %[sizeN], zero \t\n"
		"ss.end u5, zero, %[sizeN], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// x_2 stream load
		"ss.ld.d u7, %[x_2], %[sizeN], %[one] \t\n"

		// x_2 stream store
		"ss.st.d u1, %[x_2], %[sizeN], %[one] \t\n"

		".SLOOP_2%=: \t\n"
			"so.v.dp.d u2, zero, p0 \t\n"

			".SLOOP_2_0%=: \t\n"
				"so.a.mul.fp  u3, u4, u5, p0 \t\n"
				"so.a.add.fp  u2, u2, u3, p0 \t\n"
			"so.b.ndc.1 u4, .SLOOP_2_0%= \t\n"

			"so.a.adde.fp  u3, u2, p0 \t\n"
			"so.a.add.fp  u1, u7, u3, p0 \t\n"
		"so.b.nc u1, .SLOOP_2%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType* A, DataType* x_1, DataType* x_2, DataType* y_1, DataType* y_2, uint64_t sizeN) {
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_1[i] = x_1[i] + A[i*sizeN+j] * y_1[j];

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_2[i] = x_2[i] + A[j*sizeN+i] * y_2[j];

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE

#ifdef RUN_BLANK
void core(DataType* A, DataType* x_1, DataType* x_2, DataType* y_1, DataType* y_2, uint64_t sizeN) {
}
#endif // RUN_BLANK