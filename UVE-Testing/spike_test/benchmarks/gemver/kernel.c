#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \t\n"

		"ss.sta.st.d u1, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u1, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.sta.ld.d u10, %[v2], %[sizeN], zero \n\t"
		"ss.end u10, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u10 \n\t"

		"ss.sta.ld.d u9, %[u2], %[sizeN], %[one] \n\t"
		"ss.end u9, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u9 \n\t"

		"ss.sta.ld.d u7, %[v1], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.d u6, %[u1], %[sizeN], %[one] \n\t"
		"ss.end u6, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u6 \n\t"

		"ss.sta.ld.d u4, %[A], %[sizeN], %[sizeN]\n\t"
		"ss.end u4, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

        ".SLOOP_1%=: \n\t"
			"so.a.mul.fp  u5, u6, u7, p0 \n\t"
			"so.a.add.fp  u3, u4, u5, p0 \n\t"
			"so.a.mul.fp  u8, u9, u10, p0 \n\t"
			"so.a.add.fp  u1, u3, u8, p0 \n\t"
		"so.b.nc u1, .SLOOP_1%= \n\t"
		

		"ss.sta.ld.d u7, %[y], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.d u5, %[A], %[sizeN], %[one] \n\t"
		"ss.end u5, zero, %[sizeN], %[sizeN] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.d u1, %[x], %[sizeN], %[one] \n\t"

		"ss.ld.d u9, %[x], %[sizeN], %[one] \n\t"

		"so.v.dp.d u6, %[beta], p0 \n\t"

		
		".SLOOP_2%=: \n\t"
			"so.v.dp.d u11, zero, p0 \n\t"

			".SLOOP_2_0%=: \n\t"
				"so.a.mul.fp  u16, u5, u6, p0 \n\t"
				"so.a.mul.fp  u29, u16, u7, p0 \n\t"
				"so.a.add.fp  u11, u11, u29, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n\t"

			"so.a.adde.fp  u30, u11, p0 \n\t"
			"so.a.add.fp  u1, u9, u30, p0 \n\t"
		"so.b.nc u1, .SLOOP_2%= \n\t"
		

		"ss.st.d u1, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.ld.d u4, %[z], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

		"ss.ld.d u3, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u3 \n\t"

		".SLOOP_3%=: \n\t"
			"so.a.add.fp  u1, u3, u4, p0 \n\t"
		"so.b.nc u1, .SLOOP_3%= \n\t"
		


		"ss.sta.ld.d u7, %[x], %[sizeN], zero \n\t"
		"ss.end u7, zero,  %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.d u5, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u5, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.d u1, %[w], %[sizeN], %[one] \n\t"

		"ss.ld.d u9, %[w], %[sizeN], %[one] \n\t"

		"so.v.dp.d u12, %[alpha], p0 \n\t"

		
		".SLOOP_4%=: \n\t"
			"so.v.dp.d u13, zero, p0 \n\t"

			".SLOOP_4_0%=: \n\t"
				"so.a.mul.fp  u14, u5, u12, p0 \n\t"
				"so.a.mul.fp  u15, u14, u7, p0 \n\t"
				"so.a.add.fp  u13, u13, u15, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n\t"

			"so.a.adde.fp  u15, u13, p0 \n\t"
			"so.a.add.fp  u1, u9, u15, p0 \n\t"
		"so.b.nc u1, .SLOOP_4%= \n\t"

		"rdinstret %[e] \n\t"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [u1] "r"(u1),
		  [v1] "r"(v1), [u2] "r"(u2),
		  [v2] "r"(v2), [w] "r"(w),
		  [x] "r"(x), [y] "r"(y), [z] "r"(z),
		  [alpha] "r"(a), [beta] "r"(b),
		  [sizeN] "r"(sizeN), [one] "r"(1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \t\n"

		"ss.sta.st.w u1, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u1, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.sta.ld.w u10, %[v2], %[sizeN], zero \n\t"
		"ss.end u10, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u10 \n\t"

		"ss.sta.ld.w u9, %[u2], %[sizeN], %[one] \n\t"
		"ss.end u9, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u9 \n\t"

		"ss.sta.ld.w u7, %[v1], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u6, %[u1], %[sizeN], %[one] \n\t"
		"ss.end u6, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u6 \n\t"

		"ss.sta.ld.w u4, %[A], %[sizeN], %[sizeN]\n\t"
		"ss.end u4, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

        ".SLOOP_1%=: \n\t"
			"so.a.mul.fp  u5, u6, u7, p0 \n\t"
			"so.a.add.fp  u3, u4, u5, p0 \n\t"
			"so.a.mul.fp  u8, u9, u10, p0 \n\t"
			"so.a.add.fp  u1, u3, u8, p0 \n\t"
		"so.b.nc u1, .SLOOP_1%= \n\t"
		

		"ss.sta.ld.w u7, %[y], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u5, %[A], %[sizeN], %[one] \n\t"
		"ss.end u5, zero, %[sizeN], %[sizeN] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.w u1, %[x], %[sizeN], %[one] \n\t"

		"ss.ld.w u9, %[x], %[sizeN], %[one] \n\t"

		"so.v.dp.w u6, %[beta], p0 \n\t"

		
		".SLOOP_2%=: \n\t"
			"so.v.dp.w u11, zero, p0 \n\t"

			".SLOOP_2_0%=: \n\t"
				"so.a.mul.fp  u16, u5, u6, p0 \n\t"
				"so.a.mul.fp  u29, u16, u7, p0 \n\t"
				"so.a.add.fp  u11, u11, u29, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n\t"

			"so.a.adde.fp  u30, u11, p0 \n\t"
			"so.a.add.fp  u1, u9, u30, p0 \n\t"
		"so.b.nc u1, .SLOOP_2%= \n\t"
		

		"ss.st.w u1, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.ld.w u4, %[z], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

		"ss.ld.w u3, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u3 \n\t"

		".SLOOP_3%=: \n\t"
			"so.a.add.fp  u1, u3, u4, p0 \n\t"
		"so.b.nc u1, .SLOOP_3%= \n\t"
		


		"ss.sta.ld.w u7, %[x], %[sizeN], zero \n\t"
		"ss.end u7, zero,  %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u5, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u5, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.w u1, %[w], %[sizeN], %[one] \n\t"

		"ss.ld.w u9, %[w], %[sizeN], %[one] \n\t"

		"so.v.dp.w u12, %[alpha], p0 \n\t"

		
		".SLOOP_4%=: \n\t"
			"so.v.dp.w u13, zero, p0 \n\t"

			".SLOOP_4_0%=: \n\t"
				"so.a.mul.fp  u14, u5, u12, p0 \n\t"
				"so.a.mul.fp  u15, u14, u7, p0 \n\t"
				"so.a.add.fp  u13, u13, u15, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n\t"

			"so.a.adde.fp  u15, u13, p0 \n\t"
			"so.a.add.fp  u1, u9, u15, p0 \n\t"
		"so.b.nc u1, .SLOOP_4%= \n\t"

		"rdinstret %[e] \n\t"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [u1] "r"(u1),
		  [v1] "r"(v1), [u2] "r"(u2),
		  [v2] "r"(v2), [w] "r"(w),
		  [x] "r"(x), [y] "r"(y), [z] "r"(z),
		  [alpha] "r"(a), [beta] "r"(b),
		  [sizeN] "r"(sizeN), [one] "r"(1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \t\n"

		"ss.sta.st.w u1, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u1, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.sta.ld.w u10, %[v2], %[sizeN], zero \n\t"
		"ss.end u10, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u10 \n\t"

		"ss.sta.ld.w u9, %[u2], %[sizeN], %[one] \n\t"
		"ss.end u9, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u9 \n\t"

		"ss.sta.ld.w u7, %[v1], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u6, %[u1], %[sizeN], %[one] \n\t"
		"ss.end u6, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u6 \n\t"

		"ss.sta.ld.w u4, %[A], %[sizeN], %[sizeN]\n\t"
		"ss.end u4, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

        ".SLOOP_1%=: \n\t"
			"so.a.mul.sg  u5, u6, u7, p0 \n\t"
			"so.a.add.sg  u3, u4, u5, p0 \n\t"
			"so.a.mul.sg  u8, u9, u10, p0 \n\t"
			"so.a.add.sg  u1, u3, u8, p0 \n\t"
		"so.b.nc u1, .SLOOP_1%= \n\t"
		

		"ss.sta.ld.w u7, %[y], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u5, %[A], %[sizeN], %[one] \n\t"
		"ss.end u5, zero, %[sizeN], %[sizeN] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.w u1, %[x], %[sizeN], %[one] \n\t"

		"ss.ld.w u9, %[x], %[sizeN], %[one] \n\t"

		"so.v.dp.w u6, %[beta], p0 \n\t"

		
		".SLOOP_2%=: \n\t"
			"so.v.dp.w u11, zero, p0 \n\t"

			".SLOOP_2_0%=: \n\t"
				"so.a.mul.sg  u16, u5, u6, p0 \n\t"
				"so.a.mul.sg  u29, u16, u7, p0 \n\t"
				"so.a.add.sg  u11, u11, u29, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n\t"

			"so.a.adde.sg  u30, u11, p0 \n\t"
			"so.a.add.sg  u1, u9, u30, p0 \n\t"
		"so.b.nc u1, .SLOOP_2%= \n\t"
		

		"ss.st.w u1, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.ld.w u4, %[z], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

		"ss.ld.w u3, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u3 \n\t"

		".SLOOP_3%=: \n\t"
			"so.a.add.sg  u1, u3, u4, p0 \n\t"
		"so.b.nc u1, .SLOOP_3%= \n\t"
		


		"ss.sta.ld.w u7, %[x], %[sizeN], zero \n\t"
		"ss.end u7, zero,  %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.w u5, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u5, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.w u1, %[w], %[sizeN], %[one] \n\t"

		"ss.ld.w u9, %[w], %[sizeN], %[one] \n\t"

		"so.v.dp.w u12, %[alpha], p0 \n\t"

		
		".SLOOP_4%=: \n\t"
			"so.v.dp.w u13, zero, p0 \n\t"

			".SLOOP_4_0%=: \n\t"
				"so.a.mul.sg  u14, u5, u12, p0 \n\t"
				"so.a.mul.sg  u15, u14, u7, p0 \n\t"
				"so.a.add.sg  u13, u13, u15, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n\t"

			"so.a.adde.sg  u15, u13, p0 \n\t"
			"so.a.add.sg  u1, u9, u15, p0 \n\t"
		"so.b.nc u1, .SLOOP_4%= \n\t"

		"rdinstret %[e] \n\t"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [u1] "r"(u1),
		  [v1] "r"(v1), [u2] "r"(u2),
		  [v2] "r"(v2), [w] "r"(w),
		  [x] "r"(x), [y] "r"(y), [z] "r"(z),
		  [alpha] "r"(a), [beta] "r"(b),
		  [sizeN] "r"(sizeN), [one] "r"(1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \t\n"

		"ss.sta.st.h u1, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u1, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.sta.ld.h u10, %[v2], %[sizeN], zero \n\t"
		"ss.end u10, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u10 \n\t"

		"ss.sta.ld.h u9, %[u2], %[sizeN], %[one] \n\t"
		"ss.end u9, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u9 \n\t"

		"ss.sta.ld.h u7, %[v1], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.h u6, %[u1], %[sizeN], %[one] \n\t"
		"ss.end u6, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u6 \n\t"

		"ss.sta.ld.h u4, %[A], %[sizeN], %[sizeN]\n\t"
		"ss.end u4, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

        ".SLOOP_1%=: \n\t"
			"so.a.mul.sg  u5, u6, u7, p0 \n\t"
			"so.a.add.sg  u3, u4, u5, p0 \n\t"
			"so.a.mul.sg  u8, u9, u10, p0 \n\t"
			"so.a.add.sg  u1, u3, u8, p0 \n\t"
		"so.b.nc u1, .SLOOP_1%= \n\t"
		

		"ss.sta.ld.h u7, %[y], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.h u5, %[A], %[sizeN], %[one] \n\t"
		"ss.end u5, zero, %[sizeN], %[sizeN] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.h u1, %[x], %[sizeN], %[one] \n\t"

		"ss.ld.h u9, %[x], %[sizeN], %[one] \n\t"

		"so.v.dp.h u6, %[beta], p0 \n\t"

		
		".SLOOP_2%=: \n\t"
			"so.v.dp.h u11, zero, p0 \n\t"

			".SLOOP_2_0%=: \n\t"
				"so.a.mul.sg  u16, u5, u6, p0 \n\t"
				"so.a.mul.sg  u29, u16, u7, p0 \n\t"
				"so.a.add.sg  u11, u11, u29, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n\t"

			"so.a.adde.sg  u30, u11, p0 \n\t"
			"so.a.add.sg  u1, u9, u30, p0 \n\t"
		"so.b.nc u1, .SLOOP_2%= \n\t"
		

		"ss.st.h u1, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.ld.h u4, %[z], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

		"ss.ld.h u3, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u3 \n\t"

		".SLOOP_3%=: \n\t"
			"so.a.add.sg  u1, u3, u4, p0 \n\t"
		"so.b.nc u1, .SLOOP_3%= \n\t"
		


		"ss.sta.ld.h u7, %[x], %[sizeN], zero \n\t"
		"ss.end u7, zero,  %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.h u5, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u5, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.h u1, %[w], %[sizeN], %[one] \n\t"

		"ss.ld.h u9, %[w], %[sizeN], %[one] \n\t"

		"so.v.dp.h u12, %[alpha], p0 \n\t"

		
		".SLOOP_4%=: \n\t"
			"so.v.dp.h u13, zero, p0 \n\t"

			".SLOOP_4_0%=: \n\t"
				"so.a.mul.sg  u14, u5, u12, p0 \n\t"
				"so.a.mul.sg  u15, u14, u7, p0 \n\t"
				"so.a.add.sg  u13, u13, u15, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n\t"

			"so.a.adde.sg  u15, u13, p0 \n\t"
			"so.a.add.sg  u1, u9, u15, p0 \n\t"
		"so.b.nc u1, .SLOOP_4%= \n\t"

		"rdinstret %[e] \n\t"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [u1] "r"(u1),
		  [v1] "r"(v1), [u2] "r"(u2),
		  [v2] "r"(v2), [w] "r"(w),
		  [x] "r"(x), [y] "r"(y), [z] "r"(z),
		  [alpha] "r"(a), [beta] "r"(b),
		  [sizeN] "r"(sizeN), [one] "r"(1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \t\n"

		"ss.sta.st.b u1, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u1, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.sta.ld.b u10, %[v2], %[sizeN], zero \n\t"
		"ss.end u10, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u10 \n\t"

		"ss.sta.ld.b u9, %[u2], %[sizeN], %[one] \n\t"
		"ss.end u9, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u9 \n\t"

		"ss.sta.ld.b u7, %[v1], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.b u6, %[u1], %[sizeN], %[one] \n\t"
		"ss.end u6, zero, %[sizeN], zero \n\t"
		"ss.cfg.vec u6 \n\t"

		"ss.sta.ld.b u4, %[A], %[sizeN], %[sizeN]\n\t"
		"ss.end u4, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

        ".SLOOP_1%=: \n\t"
			"so.a.mul.sg  u5, u6, u7, p0 \n\t"
			"so.a.add.sg  u3, u4, u5, p0 \n\t"
			"so.a.mul.sg  u8, u9, u10, p0 \n\t"
			"so.a.add.sg  u1, u3, u8, p0 \n\t"
		"so.b.nc u1, .SLOOP_1%= \n\t"
		

		"ss.sta.ld.b u7, %[y], %[sizeN], zero \n\t"
		"ss.end u7, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.b u5, %[A], %[sizeN], %[one] \n\t"
		"ss.end u5, zero, %[sizeN], %[sizeN] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.b u1, %[x], %[sizeN], %[one] \n\t"

		"ss.ld.b u9, %[x], %[sizeN], %[one] \n\t"

		"so.v.dp.b u6, %[beta], p0 \n\t"

		
		".SLOOP_2%=: \n\t"
			"so.v.dp.b u11, zero, p0 \n\t"

			".SLOOP_2_0%=: \n\t"
				"so.a.mul.sg  u16, u5, u6, p0 \n\t"
				"so.a.mul.sg  u29, u16, u7, p0 \n\t"
				"so.a.add.sg  u11, u11, u29, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n\t"

			"so.a.adde.sg  u30, u11, p0 \n\t"
			"so.a.add.sg  u1, u9, u30, p0 \n\t"
		"so.b.nc u1, .SLOOP_2%= \n\t"
		

		"ss.st.b u1, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u1 \n\t"

		"ss.ld.b u4, %[z], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u4 \n\t"

		"ss.ld.b u3, %[x], %[sizeN], %[one] \n\t"
		"ss.cfg.vec u3 \n\t"

		".SLOOP_3%=: \n\t"
			"so.a.add.sg  u1, u3, u4, p0 \n\t"
		"so.b.nc u1, .SLOOP_3%= \n\t"
		


		"ss.sta.ld.b u7, %[x], %[sizeN], zero \n\t"
		"ss.end u7, zero,  %[sizeN], %[one] \n\t"
		"ss.cfg.vec u7 \n\t"

		"ss.sta.ld.b u5, %[A], %[sizeN], %[sizeN] \n\t"
		"ss.end u5, zero, %[sizeN], %[one] \n\t"
		"ss.cfg.vec u5 \n\t"

		"ss.st.b u1, %[w], %[sizeN], %[one] \n\t"

		"ss.ld.b u9, %[w], %[sizeN], %[one] \n\t"

		"so.v.dp.b u12, %[alpha], p0 \n\t"

		
		".SLOOP_4%=: \n\t"
			"so.v.dp.b u13, zero, p0 \n\t"

			".SLOOP_4_0%=: \n\t"
				"so.a.mul.sg  u14, u5, u12, p0 \n\t"
				"so.a.mul.sg  u15, u14, u7, p0 \n\t"
				"so.a.add.sg  u13, u13, u15, p0 \n\t"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n\t"

			"so.a.adde.sg  u15, u13, p0 \n\t"
			"so.a.add.sg  u1, u9, u15, p0 \n\t"
		"so.b.nc u1, .SLOOP_4%= \n\t"

		"rdinstret %[e] \n\t"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [u1] "r"(u1),
		  [v1] "r"(v1), [u2] "r"(u2),
		  [v2] "r"(v2), [w] "r"(w),
		  [x] "r"(x), [y] "r"(y), [z] "r"(z),
		  [alpha] "r"(a), [beta] "r"(b),
		  [sizeN] "r"(sizeN), [one] "r"(1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType* A, DataType* u1, DataType* v1, DataType* u2, DataType* v2, DataType* w,
 DataType* x, DataType* y, DataType* z, DataType alpha, DataType beta, uint64_t sizeN) {

	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

    int i,j;

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	A[i*sizeN + j] = A[i*sizeN + j] + u1[i] * v1[j] + u2[i] * v2[j];

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	x[i] = x[i] + beta * A[j*sizeN + i] * y[j];

    for (i = 0; i < sizeN; i++)
        x[i] = x[i] + z[i];

    for (i = 0; i < sizeN; i++)
        for (j = 0; j < sizeN; j++)
        	w[i] = w[i] + alpha * A[i*sizeN + j] * x[j];

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE
