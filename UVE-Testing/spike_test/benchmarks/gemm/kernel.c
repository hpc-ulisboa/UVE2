#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		// C_ij Store
		"ss.sta.st.d.v   u1, %[C] \n"
		"ss.app		     u1, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 	     u1, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.d.v   u3, %[C] \n"
		"ss.app			 u3, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 		 u3, zero, %[sizeJ], %[one] \n"

		// C_ij Store
		"ss.sta.st.d.v   u5, %[C] \n"
		"ss.app		 	 u5, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u5, zero, %[sizeK], zero \n"
		"ss.end          u5, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.d.v   u12, %[C] \n"
		"ss.app			 u12, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u12, zero, %[sizeK], zero \n"
		"ss.end          u12, zero, %[sizeJ], %[one] \n"

		// B_kj
		"ss.sta.ld.d.v   u11, %[B] \n"
		"ss.app			 u11, zero, %[sizeI], zero \n"
		"ss.app 		 u11, zero, %[sizeK], %[sizeJ] \n"
		"ss.end          u11, zero, %[sizeJ], %[one] \n"

		// A_ik
		"ss.sta.ld.d.v   u9, %[A] \n"
		"ss.app			 u9, zero, %[sizeI], %[sizeK] \n"
		"ss.app 		 u9, zero, %[sizeK], %[one] \n"
		"ss.end 		 u9, zero, %[sizeJ], zero \n"

		"so.v.dp.d u4, %[beta], p0 \n"
		"so.v.dp.d u10, %[alpha], p0 \n"

		".SLOOP_1%=: \n"
			".SLOOP_1_0%=: \n"
				"so.a.mul.fp  u1, u3, u4, p0 \n" // C_ij = C_ij * beta
			"so.b.ndc.2 u1, .SLOOP_1_0%= \n"
			".SLOOP_1_1%=: \n"
				"so.a.mul.fp  u8, u9, u10, p0 \n" // tmp1 = A_ik * alpha
				"so.a.mul.fp  u7, u8, u11, p0 \n" // tmp2 = tmp1 * B_kj
				"so.a.add.fp  u5, u7, u12, p0 \n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		:: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		// C_ij Store
		"ss.sta.st.w.v   u1, %[C] \n"
		"ss.app		     u1, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 	     u1, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.w.v   u3, %[C] \n"
		"ss.app			 u3, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 		 u3, zero, %[sizeJ], %[one] \n"

		// C_ij Store
		"ss.sta.st.w.v   u5, %[C] \n"
		"ss.app		 	 u5, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u5, zero, %[sizeK], zero \n"
		"ss.end          u5, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.w.v   u12, %[C] \n"
		"ss.app			 u12, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u12, zero, %[sizeK], zero \n"
		"ss.end          u12, zero, %[sizeJ], %[one] \n"

		// B_kj
		"ss.sta.ld.w.v   u11, %[B] \n"
		"ss.app			 u11, zero, %[sizeI], zero \n"
		"ss.app 		 u11, zero, %[sizeK], %[sizeJ] \n"
		"ss.end          u11, zero, %[sizeJ], %[one] \n"

		// A_ik
		"ss.sta.ld.w.v   u9, %[A] \n"
		"ss.app			 u9, zero, %[sizeI], %[sizeK] \n"
		"ss.app 		 u9, zero, %[sizeK], %[one] \n"
		"ss.end 		 u9, zero, %[sizeJ], zero \n"

		"so.v.dp.w u4, %[beta], p0 \n"
		"so.v.dp.w u10, %[alpha], p0 \n"

		".SLOOP_1%=: \n"
			".SLOOP_1_0%=: \n"
				"so.a.mul.fp  u1, u3, u4, p0 \n" // C_ij = C_ij * beta
			"so.b.ndc.2 u1, .SLOOP_1_0%= \n"
			".SLOOP_1_1%=: \n"
				"so.a.mul.fp  u8, u9, u10, p0 \n" // tmp1 = A_ik * alpha
				"so.a.mul.fp  u7, u8, u11, p0 \n" // tmp2 = tmp1 * B_kj
				"so.a.add.fp  u5, u7, u12, p0 \n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		:: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		// C_ij Store
		"ss.sta.st.w.v   u1, %[C] \n"
		"ss.app		     u1, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 	     u1, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.w.v   u3, %[C] \n"
		"ss.app			 u3, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 		 u3, zero, %[sizeJ], %[one] \n"

		// C_ij Store
		"ss.sta.st.w.v   u5, %[C] \n"
		"ss.app		 	 u5, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u5, zero, %[sizeK], zero \n"
		"ss.end          u5, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.w.v   u12, %[C] \n"
		"ss.app			 u12, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u12, zero, %[sizeK], zero \n"
		"ss.end          u12, zero, %[sizeJ], %[one] \n"

		// B_kj
		"ss.sta.ld.w.v   u11, %[B] \n"
		"ss.app			 u11, zero, %[sizeI], zero \n"
		"ss.app 		 u11, zero, %[sizeK], %[sizeJ] \n"
		"ss.end          u11, zero, %[sizeJ], %[one] \n"

		// A_ik
		"ss.sta.ld.w.v   u9, %[A] \n"
		"ss.app			 u9, zero, %[sizeI], %[sizeK] \n"
		"ss.app 		 u9, zero, %[sizeK], %[one] \n"
		"ss.end 		 u9, zero, %[sizeJ], zero \n"

		"so.v.dp.w u4, %[beta], p0 \n"
		"so.v.dp.w u10, %[alpha], p0 \n"

		".SLOOP_1%=: \n"
			".SLOOP_1_0%=: \n"
				"so.a.mul.sg  u1, u3, u4, p0 \n" // C_ij = C_ij * beta
			"so.b.ndc.2 u1, .SLOOP_1_0%= \n"
			".SLOOP_1_1%=: \n"
				"so.a.mul.sg  u8, u9, u10, p0 \n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		:: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		// C_ij Store
		"ss.sta.st.h.v   u1, %[C] \n"
		"ss.app		     u1, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 	     u1, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.h.v   u3, %[C] \n"
		"ss.app			 u3, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 		 u3, zero, %[sizeJ], %[one] \n"

		// C_ij Store
		"ss.sta.st.h.v   u5, %[C] \n"
		"ss.app		 	 u5, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u5, zero, %[sizeK], zero \n"
		"ss.end          u5, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.h.v   u12, %[C] \n"
		"ss.app			 u12, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u12, zero, %[sizeK], zero \n"
		"ss.end          u12, zero, %[sizeJ], %[one] \n"

		// B_kj
		"ss.sta.ld.h.v   u11, %[B] \n"
		"ss.app			 u11, zero, %[sizeI], zero \n"
		"ss.app 		 u11, zero, %[sizeK], %[sizeJ] \n"
		"ss.end          u11, zero, %[sizeJ], %[one] \n"

		// A_ik
		"ss.sta.ld.h.v   u9, %[A] \n"
		"ss.app			 u9, zero, %[sizeI], %[sizeK] \n"
		"ss.app 		 u9, zero, %[sizeK], %[one] \n"
		"ss.end 		 u9, zero, %[sizeJ], zero \n"

		"so.v.dp.h u4, %[beta], p0 \n"
		"so.v.dp.h u10, %[alpha], p0 \n"

		".SLOOP_1%=: \n"
			".SLOOP_1_0%=: \n"
				"so.a.mul.sg  u1, u3, u4, p0 \n" // C_ij = C_ij * beta
			"so.b.ndc.2 u1, .SLOOP_1_0%= \n"
			".SLOOP_1_1%=: \n"
				"so.a.mul.sg  u8, u9, u10, p0 \n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		:: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		// C_ij Store
		"ss.sta.st.b.v   u1, %[C] \n"
		"ss.app		     u1, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 	     u1, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.b.v   u3, %[C] \n"
		"ss.app			 u3, zero, %[sizeI], %[sizeJ] \n"
		"ss.end 		 u3, zero, %[sizeJ], %[one] \n"

		// C_ij Store
		"ss.sta.st.b.v   u5, %[C] \n"
		"ss.app		 	 u5, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u5, zero, %[sizeK], zero \n"
		"ss.end          u5, zero, %[sizeJ], %[one] \n"

		// C_ij Load
		"ss.sta.ld.b.v   u12, %[C] \n"
		"ss.app			 u12, zero, %[sizeI], %[sizeJ] \n"
		"ss.app 	     u12, zero, %[sizeK], zero \n"
		"ss.end          u12, zero, %[sizeJ], %[one] \n"

		// B_kj
		"ss.sta.ld.b.v   u11, %[B] \n"
		"ss.app			 u11, zero, %[sizeI], zero \n"
		"ss.app 		 u11, zero, %[sizeK], %[sizeJ] \n"
		"ss.end          u11, zero, %[sizeJ], %[one] \n"

		// A_ik
		"ss.sta.ld.b.v   u9, %[A] \n"
		"ss.app			 u9, zero, %[sizeI], %[sizeK] \n"
		"ss.app 		 u9, zero, %[sizeK], %[one] \n"
		"ss.end 		 u9, zero, %[sizeJ], zero \n"

		"so.v.dp.b u4, %[beta], p0 \n"
		"so.v.dp.b u10, %[alpha], p0 \n"

		".SLOOP_1%=: \n"
			".SLOOP_1_0%=: \n"
				"so.a.mul.sg  u1, u3, u4, p0 \n" // C_ij = C_ij * beta
			"so.b.ndc.2 u1, .SLOOP_1_0%= \n"
			".SLOOP_1_1%=: \n"
				"so.a.mul.sg  u8, u9, u10, p0 \n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \n"
		"so.b.nc u1, .SLOOP_1%= \n"

		:: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
  
  // C (sizeI x sizeJ) * beta  += alpha * A (sizeI x sizeK) * B (sizeK x sizeJ)

  asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

  int i, j, k;
  
  for (i = 0; i < sizeI; i++) {
    for (j = 0; j < sizeJ; j++)
	    C[i * sizeI + j] *= beta;
    for (k = 0; k < sizeK; k++) {
       for (j = 0; j < sizeJ; j++)
	      C[i * sizeI + j] += alpha * A[i * sizeI + k] * B[k*sizeK + j];
    }
  }

  asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
  
  printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

