#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile(
		"rdinstret %[s] \t\n"

		// C_ij Store
		"ss.sta.st.d u1, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u1, zero, %[sizeJ], %[one]\t\n"
		"ss.cfg.vec u1 \t\n"

		// C_ij Load
		"ss.sta.ld.d u3, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u3, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u3 \t\n"

		// C_ij Store
		"ss.sta.st.d u5, %[C], %[sizeI], %[sizeJ]\t\n"
		"ss.app u5, zero, %[sizeK], zero \t\n"
		"ss.end u5, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// C_ij Load
		"ss.sta.ld.d u12, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.app u12, zero, %[sizeK], zero \t\n"
		"ss.end u12, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u12 \t\n"

		// B_kj
		"ss.sta.ld.d u11, %[B], %[sizeI], zero \t\n"
		"ss.end u11, zero, %[sizeK], %[sizeJ] \t\n"
		"ss.app u11, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u11 \t\n"

		// A_ik
		"ss.sta.ld.d u9, %[A], %[sizeI], %[sizeK] \t\n"
		"ss.app u9, zero, %[sizeK], %[one]\t\n"
		"ss.end u9, zero, %[sizeJ], zero \t\n"
		"ss.cfg.vec u9 \t\n"

		"so.v.dp.d u4, %[beta], p0 \t\n"
		"so.v.dp.d u10, %[alpha], p0 \t\n"

		".SLOOP_1%=: \t\n"
			".SLOOP_1_0%=: \t\n"
				"so.a.mul.fp  u1, u3, u4, p0 \t\n" // C_ij = C_ij * beta
			"so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"
			".SLOOP_1_1%=: \t\n"
				"so.a.mul.fp  u8, u9, u10, p0 \t\n" // tmp1 = A_ik * alpha
				"so.a.mul.fp  u7, u8, u11, p0 \t\n" // tmp2 = tmp1 * B_kj
				"so.a.add.fp  u5, u7, u12, p0 \t\n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r" (start), [e] "=&r" (end)
		: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile(
		"rdinstret %[s] \t\n"

		// C_ij Store
		"ss.sta.st.w u1, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u1, zero, %[sizeJ], %[one]\t\n"
		"ss.cfg.vec u1 \t\n"

		// C_ij Load
		"ss.sta.ld.w u3, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u3, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u3 \t\n"

		// C_ij Store
		"ss.sta.st.w u5, %[C], %[sizeI], %[sizeJ]\t\n"
		"ss.app u5, zero, %[sizeK], zero \t\n"
		"ss.end u5, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// C_ij Load
		"ss.sta.ld.w u12, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.app u12, zero, %[sizeK], zero \t\n"
		"ss.end u12, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u12 \t\n"

		// B_kj
		"ss.sta.ld.w u11, %[B], %[sizeI], zero \t\n"
		"ss.end u11, zero, %[sizeK], %[sizeJ] \t\n"
		"ss.app u11, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u11 \t\n"

		// A_ik
		"ss.sta.ld.w u9, %[A], %[sizeI], %[sizeK] \t\n"
		"ss.app u9, zero, %[sizeK], %[one]\t\n"
		"ss.end u9, zero, %[sizeJ], zero \t\n"
		"ss.cfg.vec u9 \t\n"

		"so.v.dp.w u4, %[beta], p0 \t\n"
		"so.v.dp.w u10, %[alpha], p0 \t\n"

		".SLOOP_1%=: \t\n"
			".SLOOP_1_0%=: \t\n"
				"so.a.mul.fp  u1, u3, u4, p0 \t\n" // C_ij = C_ij * beta
			"so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"
			".SLOOP_1_1%=: \t\n"
				"so.a.mul.fp  u8, u9, u10, p0 \t\n" // tmp1 = A_ik * alpha
				"so.a.mul.fp  u7, u8, u11, p0 \t\n" // tmp2 = tmp1 * B_kj
				"so.a.add.fp  u5, u7, u12, p0 \t\n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r" (start), [e] "=&r" (end)
		: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile(
		"rdinstret %[s] \t\n"

		// C_ij Store
		"ss.sta.st.w u1, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u1, zero, %[sizeJ], %[one]\t\n"
		"ss.cfg.vec u1 \t\n"

		// C_ij Load
		"ss.sta.ld.w u3, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u3, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u3 \t\n"

		// C_ij Store
		"ss.sta.st.w u5, %[C], %[sizeI], %[sizeJ]\t\n"
		"ss.app u5, zero, %[sizeK], zero \t\n"
		"ss.end u5, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// C_ij Load
		"ss.sta.ld.w u12, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.app u12, zero, %[sizeK], zero \t\n"
		"ss.end u12, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u12 \t\n"

		// B_kj
		"ss.sta.ld.w u11, %[B], %[sizeI], zero \t\n"
		"ss.end u11, zero, %[sizeK], %[sizeJ] \t\n"
		"ss.app u11, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u11 \t\n"

		// A_ik
		"ss.sta.ld.w u9, %[A], %[sizeI], %[sizeK] \t\n"
		"ss.app u9, zero, %[sizeK], %[one]\t\n"
		"ss.end u9, zero, %[sizeJ], zero \t\n"
		"ss.cfg.vec u9 \t\n"

		"so.v.dp.w u4, %[beta], p0 \t\n"
		"so.v.dp.w u10, %[alpha], p0 \t\n"

		".SLOOP_1%=: \t\n"
			".SLOOP_1_0%=: \t\n"
				"so.a.mul.sg  u1, u3, u4, p0 \t\n" // C_ij = C_ij * beta
			"so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"
			".SLOOP_1_1%=: \t\n"
				"so.a.mul.sg  u8, u9, u10, p0 \t\n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \t\n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \t\n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r" (start), [e] "=&r" (end)
		: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile(
		"rdinstret %[s] \t\n"

		// C_ij Store
		"ss.sta.st.h u1, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u1, zero, %[sizeJ], %[one]\t\n"
		"ss.cfg.vec u1 \t\n"

		// C_ij Load
		"ss.sta.ld.h u3, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u3, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u3 \t\n"

		// C_ij Store
		"ss.sta.st.h u5, %[C], %[sizeI], %[sizeJ]\t\n"
		"ss.app u5, zero, %[sizeK], zero \t\n"
		"ss.end u5, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// C_ij Load
		"ss.sta.ld.h u12, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.app u12, zero, %[sizeK], zero \t\n"
		"ss.end u12, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u12 \t\n"

		// B_kj
		"ss.sta.ld.h u11, %[B], %[sizeI], zero \t\n"
		"ss.end u11, zero, %[sizeK], %[sizeJ] \t\n"
		"ss.app u11, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u11 \t\n"

		// A_ik
		"ss.sta.ld.h u9, %[A], %[sizeI], %[sizeK] \t\n"
		"ss.app u9, zero, %[sizeK], %[one]\t\n"
		"ss.end u9, zero, %[sizeJ], zero \t\n"
		"ss.cfg.vec u9 \t\n"

		"so.v.dp.h u4, %[beta], p0 \t\n"
		"so.v.dp.h u10, %[alpha], p0 \t\n"

		".SLOOP_1%=: \t\n"
			".SLOOP_1_0%=: \t\n"
				"so.a.mul.sg  u1, u3, u4, p0 \t\n" // C_ij = C_ij * beta
			"so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"
			".SLOOP_1_1%=: \t\n"
				"so.a.mul.sg  u8, u9, u10, p0 \t\n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \t\n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \t\n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r" (start), [e] "=&r" (end)
		: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
	asm volatile(
		"rdinstret %[s] \t\n"

		// C_ij Store
		"ss.sta.st.b u1, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u1, zero, %[sizeJ], %[one]\t\n"
		"ss.cfg.vec u1 \t\n"

		// C_ij Load
		"ss.sta.ld.b u3, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.end u3, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u3 \t\n"

		// C_ij Store
		"ss.sta.st.b u5, %[C], %[sizeI], %[sizeJ]\t\n"
		"ss.app u5, zero, %[sizeK], zero \t\n"
		"ss.end u5, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u5 \t\n"

		// C_ij Load
		"ss.sta.ld.b u12, %[C], %[sizeI], %[sizeJ] \t\n"
		"ss.app u12, zero, %[sizeK], zero \t\n"
		"ss.end u12, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u12 \t\n"

		// B_kj
		"ss.sta.ld.b u11, %[B], %[sizeI], zero \t\n"
		"ss.end u11, zero, %[sizeK], %[sizeJ] \t\n"
		"ss.app u11, zero, %[sizeJ], %[one] \t\n"
		"ss.cfg.vec u11 \t\n"

		// A_ik
		"ss.sta.ld.b u9, %[A], %[sizeI], %[sizeK] \t\n"
		"ss.app u9, zero, %[sizeK], %[one]\t\n"
		"ss.end u9, zero, %[sizeJ], zero \t\n"
		"ss.cfg.vec u9 \t\n"

		"so.v.dp.b u4, %[beta], p0 \t\n"
		"so.v.dp.b u10, %[alpha], p0 \t\n"

		".SLOOP_1%=: \t\n"
			".SLOOP_1_0%=: \t\n"
				"so.a.mul.sg  u1, u3, u4, p0 \t\n" // C_ij = C_ij * beta
			"so.b.ndc.1 u1, .SLOOP_1_0%= \t\n"
			".SLOOP_1_1%=: \t\n"
				"so.a.mul.sg  u8, u9, u10, p0 \t\n" // tmp1 = A_ik * alpha
				"so.a.mul.sg  u7, u8, u11, p0 \t\n" // tmp2 = tmp1 * B_kj
				"so.a.add.sg  u5, u7, u12, p0 \t\n" // C_ij = tmp2 + C_ij
			"so.b.ndc.2 u5, .SLOOP_1_1%= \t\n"
		"so.b.nc u1, .SLOOP_1%= \t\n"

		"rdinstret %[e] \t\n"

		: [s] "=&r" (start), [e] "=&r" (end)
		: [sizeI] "r" (sizeI), [sizeJ] "r" (sizeJ), [sizeK] "r" (sizeK),
		[alpha] "r" (alpha), [beta] "r" (beta),
		[C] "r" (C), [A] "r" (A), [B] "r" (B),
		[one] "r" (1)
	);
	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType alpha, DataType beta, DataType *C, DataType *A, DataType *B, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK){
  
  // C (sizeI x sizeJ) * beta  += alpha * A (sizeI x sizeK) * B (sizeK x sizeJ)

  asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

  int i, j, k;
  
  for (i = 0; i < sizeI; i++) {
    for (j = 0; j < sizeJ; j++)
	    C[i * sizeI + j] *= beta;
    for (k = 0; k < sizeK; k++) {
       for (j = 0; j < sizeJ; j++)
	      C[i * sizeI + j] += alpha * A[i * sizeI + k] * B[k*sizeK + j];
    }
  }

  asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

