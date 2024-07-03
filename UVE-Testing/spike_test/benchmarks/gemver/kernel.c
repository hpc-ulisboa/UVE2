#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *A, DataType *u1, DataType *v1, DataType *u2, DataType *v2, DataType *w, DataType *x, DataType *y, DataType *z, DataType a, DataType b, uint64_t sizeN) {
	asm volatile(
		"rdinstret %[s] \n"

		"ss.sta.st.d.v.1 u1, %[A] \n"
		"ss.app			 u1, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v.1 u10, %[v2] \n"
		"ss.app			 u10, zero, %[sizeN], zero \n"
		"ss.end 		 u10, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v.1 u9, %[u2] \n"
		"ss.app		     u9, zero, %[sizeN], %[one] \n"
		"ss.end 		 u9, zero, %[sizeN], zero \n"

		"ss.sta.ld.d.v.1 u7, %[v1] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v.1 u6, %[u1] \n"
		"ss.app			 u6, zero, %[sizeN], %[one] \n"
		"ss.end 		 u6, zero, %[sizeN], zero \n"

		"ss.sta.ld.d.v.1 u4, %[A] \n"
		"ss.app			 u4, zero, %[sizeN], %[sizeN]\n"
		"ss.end 		 u4, zero, %[sizeN], %[one] \n"

        ".SLOOP_1%=: \n"
			"so.a.mul.fp  u5, u6, u7, p0 \n"
			"so.a.add.fp  u3, u4, u5, p0 \n"
			"so.a.mul.fp  u8, u9, u10, p0 \n"
			"so.a.add.fp  u1, u3, u8, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"
		

		"ss.sta.ld.d.v.1 u7, %[y] \n"
		"ss.app		     u7, zero, %[sizeN], zero \n"
		"ss.end 	     u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[one] \n"
		"ss.end 		 u5, zero, %[sizeN], %[sizeN] \n"

		"ss.sta.st.d u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d u9, %[x] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.d u6, %[beta], p0 \n"
		
		".SLOOP_2%=: \n"
			"so.v.dp.d u11, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				"so.a.mul.fp  u16, u5, u6, p0 \n"
				"so.a.mul.fp  u29, u16, u7, p0 \n"
				"so.a.adde.acc.fp  u11, u29, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n"

			//"so.a.adde.fp  u30, u11, p0 \n"
			//"so.a.add.fp  u1, u9, u30, p0 \n"
			"so.a.add.fp  u1, u9, u11, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"
		

		"ss.sta.st.d.v u1, %[x] \n"
		"ss.end		   u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v u4, %[z] \n"
		"ss.end 	   u4, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v u3, %[x] \n"
		"ss.end 	   u3, zero, %[sizeN], %[one] \n"

		".SLOOP_3%=: \n"
			"so.a.add.fp  u1, u3, u4, p0 \n"
		"so.b.nc u1, .SLOOP_3%= \n"
		

		"ss.sta.ld.d.v.1 u7, %[x] \n"
		"ss.app		 	 u7, zero, %[sizeN], zero \n"
		"ss.end      	 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u5, zero, %[sizeN], %[one] \n"

		"ss.sta.st.d u1, %[w] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.d u9, %[w] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.d u12, %[alpha], p0 \n"

		
		".SLOOP_4%=: \n"
			"so.v.dp.d u13, zero, p0 \n"

			".SLOOP_4_0%=: \n"
				"so.a.mul.fp  u14, u5, u12, p0 \n"
				"so.a.mul.fp  u15, u14, u7, p0 \n"
				"so.a.adde.acc.fp  u13, u15, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n"

			//"so.a.adde.fp  u15, u13, p0 \n"
			//"so.a.add.fp  u1, u9, u15, p0 \n"
			"so.a.add.fp  u1, u9, u13, p0 \n"
		"so.b.nc u1, .SLOOP_4%= \n"

		"rdinstret %[e] \n"

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
		"rdinstret %[s] \n"

		"ss.sta.st.w.v.1 u1, %[A] \n"
		"ss.app			 u1, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u10, %[v2] \n"
		"ss.app			 u10, zero, %[sizeN], zero \n"
		"ss.end 		 u10, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u9, %[u2] \n"
		"ss.app		     u9, zero, %[sizeN], %[one] \n"
		"ss.end 		 u9, zero, %[sizeN], zero \n"

		"ss.sta.ld.w.v.1 u7, %[v1] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u6, %[u1] \n"
		"ss.app			 u6, zero, %[sizeN], %[one] \n"
		"ss.end 		 u6, zero, %[sizeN], zero \n"

		"ss.sta.ld.w.v.1 u4, %[A] \n"
		"ss.app			 u4, zero, %[sizeN], %[sizeN]\n"
		"ss.end 		 u4, zero, %[sizeN], %[one] \n"

        ".SLOOP_1%=: \n"
			"so.a.mul.fp  u5, u6, u7, p0 \n"
			"so.a.add.fp  u3, u4, u5, p0 \n"
			"so.a.mul.fp  u8, u9, u10, p0 \n"
			"so.a.add.fp  u1, u3, u8, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"
		

		"ss.sta.ld.w.v.1 u7, %[y] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[one]	\n"
		"ss.end 		 u5, zero, %[sizeN], %[sizeN] \n"

		"ss.sta.st.w u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w u9, %[x] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.w u6, %[beta], p0 \n"

		
		".SLOOP_2%=: \n"
			"so.v.dp.w u11, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				"so.a.mul.fp  u16, u5, u6, p0 \n"
				"so.a.mul.fp  u29, u16, u7, p0 \n"
				"so.a.adde.acc.fp  u11, u29, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n"

			//"so.a.adde.fp  u30, u11, p0 \n"
			//"so.a.add.fp  u1, u9, u30, p0 \n"
			"so.a.add.fp  u1, u9, u11, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"
		

		"ss.sta.st.w.v u1, %[x] \n"
		"ss.end		   u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v u4, %[z] \n"
		"ss.end 	   u4, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v u3, %[x] \n"
		"ss.end 	   u3, zero, %[sizeN], %[one] \n"

		".SLOOP_3%=: \n"
			"so.a.add.fp  u1, u3, u4, p0 \n"
		"so.b.nc u1, .SLOOP_3%= \n"
		

		"ss.sta.ld.w.v.1 u7, %[x] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero,  %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u5, %[A] \n"
		"ss.app		     u5, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u5, zero, %[sizeN], %[one] \n"

		"ss.sta.st.w u1, %[w] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w u9, %[w] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.w u12, %[alpha], p0 \n"

		
		".SLOOP_4%=: \n"
			"so.v.dp.w u13, zero, p0 \n"

			".SLOOP_4_0%=: \n"
				"so.a.mul.fp  u14, u5, u12, p0 \n"
				"so.a.mul.fp  u15, u14, u7, p0 \n"
				"so.a.adde.acc.fp  u13, u15, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n"

			//"so.a.adde.fp  u15, u13, p0 \n"
			//"so.a.add.fp  u1, u9, u15, p0 \n"
			"so.a.add.fp  u1, u9, u13, p0 \n"
		"so.b.nc u1, .SLOOP_4%= \n"

		"rdinstret %[e] \n"

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
		"rdinstret %[s] \n"

		"ss.sta.st.w.v.1 u1, %[A] \n"
		"ss.app			 u1, zero, %[sizeN], %[sizeN] \n"
		"ss.end			 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u10, %[v2] \n"
		"ss.app			 u10, zero, %[sizeN], zero \n"
		"ss.end 		 u10, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u9, %[u2] \n"
		"ss.app		     u9, zero, %[sizeN], %[one] \n"
		"ss.end 		 u9, zero, %[sizeN], zero \n"

		"ss.sta.ld.w.v.1 u7, %[v1] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u6, %[u1] \n"
		"ss.app			 u6, zero, %[sizeN], %[one] \n"
		"ss.end 		 u6, zero, %[sizeN], zero \n"

		"ss.sta.ld.w.v.1 u4, %[A] \n"
		"ss.app			 u4, zero, %[sizeN], %[sizeN]\n"
		"ss.end 		 u4, zero, %[sizeN], %[one] \n"

        ".SLOOP_1%=: \n"
			"so.a.mul.sg  u5, u6, u7, p0 \n"
			"so.a.add.sg  u3, u4, u5, p0 \n"
			"so.a.mul.sg  u8, u9, u10, p0 \n"
			"so.a.add.sg  u1, u3, u8, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"
		

		"ss.sta.ld.w.v.1 u7, %[y] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[one]	\n"
		"ss.end 		 u5, zero, %[sizeN], %[sizeN] \n"

		"ss.sta.st.w u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w u9, %[x] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.w u6, %[beta], p0 \n"

		
		".SLOOP_2%=: \n"
			"so.v.dp.w u11, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				"so.a.mul.sg  u16, u5, u6, p0 \n"
				"so.a.mul.sg  u29, u16, u7, p0 \n"
				"so.a.adde.acc.sg  u11, u29, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n"

			//"so.a.adde.sg  u30, u11, p0 \n"
			//"so.a.add.sg  u1, u9, u30, p0 \n"
			"so.a.add.sg  u1, u9, u11, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"
		

		"ss.sta.st.w u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v u4, %[z] \n"
		"ss.end 	   u4, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w.v u3, %[x] \n"
		"ss.end 	   u3, zero, %[sizeN], %[one] \n"

		".SLOOP_3%=: \n"
			"so.a.add.sg  u1, u3, u4, p0 \n"
		"so.b.nc u1, .SLOOP_3%= \n"
		


		"ss.sta.ld.w.v.1 u7, %[x] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero,  %[sizeN], %[one] \n"

		"ss.sta.ld.w.v.1 u5, %[A] \n"
		"ss.app		     u5, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u5, zero, %[sizeN], %[one] \n"

		"ss.sta.st.w u1, %[w] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.w u9, %[w] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.w u12, %[alpha], p0 \n"

		
		".SLOOP_4%=: \n"
			"so.v.dp.w u13, zero, p0 \n"

			".SLOOP_4_0%=: \n"
				"so.a.mul.sg  u14, u5, u12, p0 \n"
				"so.a.mul.sg  u15, u14, u7, p0 \n"
				"so.a.adde.acc.sg  u13, u15, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n"

			//"so.a.adde.sg  u15, u13, p0 \n"
			//"so.a.add.sg  u1, u9, u15, p0 \n"
			"so.a.add.sg  u1, u9, u13, p0 \n"
		"so.b.nc u1, .SLOOP_4%= \n"

		"rdinstret %[e] \n"

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
		"rdinstret %[s] \n"

		"ss.sta.st.h.v.1 u1, %[A] \n"
		"ss.app			 u1, zero, %[sizeN], %[sizeN] \n"
		"ss.end			 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v.1 u10, %[v2] \n"
		"ss.app			 u10, zero, %[sizeN], zero \n"
		"ss.end 		 u10, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v.1 u9, %[u2] \n"
		"ss.app		     u9, zero, %[sizeN], %[one] \n"
		"ss.end 		 u9, zero, %[sizeN], zero \n"

		"ss.sta.ld.h.v.1 u7, %[v1] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v.1 u6, %[u1] \n"
		"ss.app			 u6, zero, %[sizeN], %[one] \n"
		"ss.end 		 u6, zero, %[sizeN], zero \n"

		"ss.sta.ld.h.v.1 u4, %[A] \n"
		"ss.app			 u4, zero, %[sizeN], %[sizeN]\n"
		"ss.end 		 u4, zero, %[sizeN], %[one] \n"

        ".SLOOP_1%=: \n"
			"so.a.mul.sg  u5, u6, u7, p0 \n"
			"so.a.add.sg  u3, u4, u5, p0 \n"
			"so.a.mul.sg  u8, u9, u10, p0 \n"
			"so.a.add.sg  u1, u3, u8, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"
		

		"ss.sta.ld.h.v.1 u7, %[y] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[one]	\n"
		"ss.end 		 u5, zero, %[sizeN], %[sizeN] \n"

		"ss.sta.st.h u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h u9, %[x] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.h u6, %[beta], p0 \n"

		
		".SLOOP_2%=: \n"
			"so.v.dp.h u11, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				"so.a.mul.sg  u16, u5, u6, p0 \n"
				"so.a.mul.sg  u29, u16, u7, p0 \n"
				"so.a.adde.acc.sg  u11, u29, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n"

			//"so.a.adde.sg  u30, u11, p0 \n"
			//"so.a.add.sg  u1, u9, u30, p0 \n"
			"so.a.add.sg  u1, u9, u11, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"
		

		"ss.sta.st.h u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v u4, %[z] \n"
		"ss.end 	   u4, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h.v u3, %[x] \n"
		"ss.end 	   u3, zero, %[sizeN], %[one] \n"

		".SLOOP_3%=: \n"
			"so.a.add.sg  u1, u3, u4, p0 \n"
		"so.b.nc u1, .SLOOP_3%= \n"
		


		"ss.sta.ld.h.v.1 u7, %[x] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero,  %[sizeN], %[one] \n"

		"ss.sta.ld.h.v.1 u5, %[A] \n"
		"ss.app		     u5, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u5, zero, %[sizeN], %[one] \n"

		"ss.sta.st.h u1, %[w] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.h u9, %[w] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.h u12, %[alpha], p0 \n"

		
		".SLOOP_4%=: \n"
			"so.v.dp.h u13, zero, p0 \n"

			".SLOOP_4_0%=: \n"
				"so.a.mul.sg  u14, u5, u12, p0 \n"
				"so.a.mul.sg  u15, u14, u7, p0 \n"
				"so.a.adde.acc.sg  u13, u15, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n"

			//"so.a.adde.sg  u15, u13, p0 \n"
			//"so.a.add.sg  u1, u9, u15, p0 \n"
			"so.a.add.sg  u1, u9, u13, p0 \n"
		"so.b.nc u1, .SLOOP_4%= \n"

		"rdinstret %[e] \n"

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
		"rdinstret %[s] \n"

		"ss.sta.st.b.v.1 u1, %[A] \n"
		"ss.app			 u1, zero, %[sizeN], %[sizeN] \n"
		"ss.end			 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v.1 u10, %[v2] \n"
		"ss.app			 u10, zero, %[sizeN], zero \n"
		"ss.end 		 u10, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v.1 u9, %[u2] \n"
		"ss.app		     u9, zero, %[sizeN], %[one] \n"
		"ss.end 		 u9, zero, %[sizeN], zero \n"

		"ss.sta.ld.b.v.1 u7, %[v1] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v.1 u6, %[u1] \n"
		"ss.app			 u6, zero, %[sizeN], %[one] \n"
		"ss.end 		 u6, zero, %[sizeN], zero \n"

		"ss.sta.ld.b.v.1 u4, %[A] \n"
		"ss.app			 u4, zero, %[sizeN], %[sizeN]\n"
		"ss.end 		 u4, zero, %[sizeN], %[one] \n"

        ".SLOOP_1%=: \n"
			"so.a.mul.sg  u5, u6, u7, p0 \n"
			"so.a.add.sg  u3, u4, u5, p0 \n"
			"so.a.mul.sg  u8, u9, u10, p0 \n"
			"so.a.add.sg  u1, u3, u8, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"
		

		"ss.sta.ld.b.v.1 u7, %[y] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v.1 u5, %[A] \n"
		"ss.app			 u5, zero, %[sizeN], %[one]	\n"
		"ss.end 		 u5, zero, %[sizeN], %[sizeN] \n"

		"ss.sta.st.b u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b u9, %[x] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.b u6, %[beta], p0 \n"

		
		".SLOOP_2%=: \n"
			"so.v.dp.b u11, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				"so.a.mul.sg  u16, u5, u6, p0 \n"
				"so.a.mul.sg  u29, u16, u7, p0 \n"
				"so.a.adde.acc.sg  u11, u29, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_2_0%= \n"

			//"so.a.adde.sg  u30, u11, p0 \n"
			//"so.a.add.sg  u1, u9, u30, p0 \n"
			"so.a.add.sg  u1, u9, u11, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"
		

		"ss.sta.st.b u1, %[x] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v u4, %[z] \n"
		"ss.end 	   u4, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b.v u3, %[x] \n"
		"ss.end 	   u3, zero, %[sizeN], %[one] \n"

		".SLOOP_3%=: \n"
			"so.a.add.sg  u1, u3, u4, p0 \n"
		"so.b.nc u1, .SLOOP_3%= \n"
		


		"ss.sta.ld.b.v.1 u7, %[x] \n"
		"ss.app			 u7, zero, %[sizeN], zero \n"
		"ss.end 		 u7, zero,  %[sizeN], %[one] \n"

		"ss.sta.ld.b.v.1 u5, %[A] \n"
		"ss.app		     u5, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		 u5, zero, %[sizeN], %[one] \n"

		"ss.sta.st.b u1, %[w] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		"ss.sta.ld.b u9, %[w] \n"
		"ss.end 	 u9, zero, %[sizeN], %[one] \n"

		"so.v.dp.b u12, %[alpha], p0 \n"

		
		".SLOOP_4%=: \n"
			"so.v.dp.b u13, zero, p0 \n"

			".SLOOP_4_0%=: \n"
				"so.a.mul.sg  u14, u5, u12, p0 \n"
				"so.a.mul.sg  u15, u14, u7, p0 \n"
				"so.a.adde.acc.sg  u13, u15, p0 \n"
			"so.b.ndc.1 u5, .SLOOP_4_0%= \n"

			//"so.a.adde.sg  u15, u13, p0 \n"
			//"so.a.add.sg  u1, u9, u15, p0 \n"
			"so.a.add.sg  u1, u9, u13, p0 \n"
		"so.b.nc u1, .SLOOP_4%= \n"

		"rdinstret %[e] \n"

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

	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

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

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE
