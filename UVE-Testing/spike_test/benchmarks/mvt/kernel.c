#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \n"

		// A stream load
		"ss.sta.ld.d.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		   u4, zero, %[sizeN], %[one]  \n"

		// y_1 stream load
		"ss.sta.ld.d.v.m   u5, %[y_1] \n"
		"ss.app 		   u5, zero, %[sizeN], zero \n"
		"ss.end 	       u5, zero, %[sizeN], %[one] \n"

		// x_1 stream load
		"ss.sta.ld.d u7, %[x_1] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"
		
		// x_1 stream store
		"ss.sta.st.d u1, %[x_1] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_1%=: \n"
			"so.v.dp.d u2, zero, p0 \n"

			".SLOOP_1_0%=: \n"
				/*"so.a.mul.fp  u3, u4, u5, p0 \n"
				"so.a.add.fp  u2, u2, u3, p0 \n"*/
				"so.a.mac.fp  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_1_0%= \n"

			"so.a.adde.fp  u3, u2, p0 \n"
			"so.a.add.fp  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		// A stream load
		"ss.sta.ld.d.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[one] \n"
		"ss.end 		   u4, zero, %[sizeN], %[sizeN] \n"

		// y_2 stream load
		"ss.sta.ld.d.v.m   u5, %[y_2] \n"
		"ss.app			   u5, zero, %[sizeN], zero \n"
		"ss.end 		   u5, zero, %[sizeN], %[one] \n"

		// x_2 stream load
		"ss.sta.ld.d u7, %[x_2] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"

		// x_2 stream store
		"ss.sta.st.d u1, %[x_2] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_2%=: \n"
			"so.v.dp.d u2, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				/*"so.a.mul.fp  u3, u4, u5, p0 \n"
				"so.a.add.fp  u2, u2, u3, p0 \n"*/
				"so.a.mac.fp  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_2_0%= \n"

			"so.a.adde.fp  u3, u2, p0 \n"
			"so.a.add.fp  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"

		"rdinstret %[e] \n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \n"

		// A stream load
		"ss.sta.ld.w.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		   u4, zero, %[sizeN], %[one]  \n"

		// y_1 stream load
		"ss.sta.ld.w.v.m   u5, %[y_1] \n"
		"ss.app 		   u5, zero, %[sizeN], zero \n"
		"ss.end 	       u5, zero, %[sizeN], %[one] \n"

		// x_1 stream load
		"ss.sta.ld.w u7, %[x_1] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"
		
		// x_1 stream store
		"ss.sta.st.w u1, %[x_1] \n"
		"ss.end 	 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_1%=: \n"
			"so.v.dp.w u2, zero, p0 \n"

			".SLOOP_1_0%=: \n"
				/*"so.a.mul.fp  u3, u4, u5, p0 \n"
				"so.a.add.fp  u2, u2, u3, p0 \n"*/
				"so.a.mac.fp  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_1_0%= \n"

			"so.a.adde.fp  u3, u2, p0 \n"
			"so.a.add.fp  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		// A stream load
		"ss.sta.ld.w.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[one] \n"
		"ss.end 		   u4, zero, %[sizeN], %[sizeN] \n"

		// y_2 stream load
		"ss.sta.ld.w.v.m   u5, %[y_2] \n"
		"ss.app			   u5, zero, %[sizeN], zero \n"
		"ss.end 		   u5, zero, %[sizeN], %[one] \n"

		// x_2 stream load
		"ss.sta.ld.w u7, %[x_2] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"

		// x_2 stream store
		"ss.sta.st.w u1, %[x_2] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_2%=: \n"
			"so.v.dp.w u2, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				/*"so.a.mul.fp  u3, u4, u5, p0 \n"
				"so.a.add.fp  u2, u2, u3, p0 \n"*/
				"so.a.mac.fp  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_2_0%= \n"

			"so.a.adde.fp  u3, u2, p0 \n"
			"so.a.add.fp  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"

		"rdinstret %[e] \n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \n"

		// A stream load
		"ss.sta.ld.w.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		   u4, zero, %[sizeN], %[one]  \n"

		// y_1 stream load
		"ss.sta.ld.w.v.m   u5, %[y_1] \n"
		"ss.app 		   u5, zero, %[sizeN], zero \n"
		"ss.end 	       u5, zero, %[sizeN], %[one] \n"

		// x_1 stream load
		"ss.sta.ld.w u7, %[x_1] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"
		
		// x_1 stream store
		"ss.sta.st.w u1, %[x_1] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_1%=: \n"
			"so.v.dp.w u2, zero, p0 \n"

			".SLOOP_1_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_1_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		// A stream load
		"ss.sta.ld.w.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[one] \n"
		"ss.end            u4, zero, %[sizeN], %[sizeN] \n"

		// y_2 stream load
		"ss.sta.ld.w.v.m   u5, %[y_2] \n"
		"ss.app			   u5, zero, %[sizeN], zero \n"
		"ss.end 		   u5, zero, %[sizeN], %[one] \n"

		// x_2 stream load
		"ss.sta.ld.w u7, %[x_2] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"

		// x_2 stream store
		"ss.sta.st.w u1, %[x_2] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_2%=: \n"
			"so.v.dp.w u2, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_2_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"

		"rdinstret %[e] \n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \n"

		// A stream load
		"ss.sta.ld.h.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		   u4, zero, %[sizeN], %[one]  \n"

		// y_1 stream load
		"ss.sta.ld.h.v.m   u5, %[y_1] \n"
		"ss.app 		   u5, zero, %[sizeN], zero \n"
		"ss.end 	       u5, zero, %[sizeN], %[one] \n"

		// x_1 stream load
		"ss.sta.ld.h u7, %[x_1] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"
		
		// x_1 stream store
		"ss.sta.st.h u1, %[x_1] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_1%=: \n"
			"so.v.dp.h u2, zero, p0 \n"

			".SLOOP_1_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_1_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		// A stream load
		"ss.sta.ld.h.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[one] \n"
		"ss.end 		   u4, zero, %[sizeN], %[sizeN] \n"

		// y_2 stream load
		"ss.sta.ld.h.v.m   u5, %[y_2] \n"
		"ss.app			   u5, zero, %[sizeN], zero \n"
		"ss.end 		   u5, zero, %[sizeN], %[one] \n"

		// x_2 stream load
		"ss.sta.ld.h u7, %[x_2] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"

		// x_2 stream store
		"ss.sta.st.h u1, %[x_2]\n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_2%=: \n"
			"so.v.dp.h u2, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_2_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"

		"rdinstret %[e] \n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *A, DataType *x_1, DataType *x_2, DataType *y_1, DataType *y_2, uint64_t sizeN){
    asm volatile(
		"rdinstret %[s] \n"

		// A stream load
		"ss.sta.ld.b.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[sizeN] \n"
		"ss.end 		   u4, zero, %[sizeN], %[one]  \n"

		// y_1 stream load
		"ss.sta.ld.b.v.m   u5, %[y_1] \n"
		"ss.app 		   u5, zero, %[sizeN], zero \n"
		"ss.end 	       u5, zero, %[sizeN], %[one] \n"

		// x_1 stream load
		"ss.sta.ld.b u7, %[x_1] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"
		
		// x_1 stream store
		"ss.sta.st.b u1, %[x_1] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_1%=: \n"
			"so.v.dp.b u2, zero, p0 \n"

			".SLOOP_1_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_1_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		// A stream load
		"ss.sta.ld.b.v.m   u4, %[A] \n"
		"ss.app			   u4, zero, %[sizeN], %[one] \n"
		"ss.end 		   u4, zero, %[sizeN], %[sizeN] \n"

		// y_2 stream load
		"ss.sta.ld.b.v.m   u5, %[y_2] \n"
		"ss.app			   u5, zero, %[sizeN], zero \n"
		"ss.end            u5, zero, %[sizeN], %[one] \n"

		// x_2 stream load
		"ss.sta.ld.b u7, %[x_2] \n"
		"ss.end		 u7, zero, %[sizeN], %[one] \n"

		// x_2 stream store
		"ss.sta.st.b u1, %[x_2] \n"
		"ss.end		 u1, zero, %[sizeN], %[one] \n"

		".SLOOP_2%=: \n"
			"so.v.dp.b u2, zero, p0 \n"

			".SLOOP_2_0%=: \n"
				/*"so.a.mul.sg  u3, u4, u5, p0 \n"
				"so.a.add.sg  u2, u2, u3, p0 \n"*/
				"so.a.mac.sg  u2, u4, u5, p0 \n"
			"so.b.ndc.2 u4, .SLOOP_2_0%= \n"

			"so.a.adde.sg  u3, u2, p0 \n"
			"so.a.add.sg  u1, u7, u3, p0 \n"
		"so.b.nc u1, .SLOOP_2%= \n"

		"rdinstret %[e] \n"

		: [s] "=&r"(start), [e] "=&r"(end)
		: [A] "r"(A), [y_1] "r"(y_1), [x_1] "r"(x_1), 
		  [y_2] "r" (y_2), [x_2] "r" (x_2), 
		  [sizeN] "r"(sizeN), [one] "r" (1)
	);

	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType* A, DataType* x_1, DataType* x_2, DataType* y_1, DataType* y_2, uint64_t sizeN) {
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_1[i] = x_1[i] + A[i*sizeN+j] * y_1[j];

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x_2[i] = x_2[i] + A[j*sizeN+i] * y_2[j];

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE

