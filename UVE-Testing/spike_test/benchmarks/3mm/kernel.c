#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
void uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
    asm volatile(
		// A stream (IxK)
		"ss.sta.ld.d           u1, %[src1], %[si], %[sk] \t\n" // D2: slide verticaly stride sizeK access size sizeI
		"ss.app                u1, zero, %[sj], zero \t\n"     // repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \t\n"   // D1: slide horizontaly by 1, access size sizeK
		"ss.cfg.vec            u1                      \t\n"

		// B stream (KxJ)
		"ss.sta.ld.d           u2, %[src2], %[si], zero \t\n"  // D2: slide horizontaly by 1, access size sizeJ  
		"ss.app                u2, zero, %[sj], %[one] \t\n"   // repeat: for each 'i'   
		"ss.end                u2, zero, %[sk], %[sj] \t\n"    // D1: slide verticaly stride sizeJ ('sizeK' times)
		"ss.cfg.vec            u2                     \t\n"

		// C stream store (IxJ)
		"ss.sta.st.d           u4, %[src3], %[si], %[sj] \t\n"   // D2: slide verticaly stride sizeJ access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \t\n"     // D1: slide horizontaly by 1, access size sizeJ

		".iLoop1%=: \t\n"    
            "so.v.dp.d u21, zero, p0 \t\n"
            ".kloop1%=: \t\n"
				//"so.a.mac.fp u21, u1, u2, p0\n\t" // tmp += (.A) * B
				"so.a.mul.fp u22, u1, u2, p0\n\t" // tmp1 = (.A) * B
				"so.a.add.fp u21, u21, u22, p0\n\t" // tmp += tmp1
            "so.b.ndc.1 u2, .kloop1%= \n\t"
            "so.a.adde.fp  u4, u21, p0 \n\t" // store tmp to C 
        "so.b.nc	u2, .iLoop1%= \n\t"

		:
		: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));
}

void core(void* A, void* B, void* C, void* D, void* E, void* F, void* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M) {
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	uve_kernel(A, B, E, I, J, K);
	uve_kernel(C, D, F, J, L, M);
	uve_kernel(E, F, G, I, L, J);

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
    DataType *A = (DataType *)src1; /* IxK */
    DataType *B = (DataType *)src2; /* KxJ */
    DataType *C = (DataType *)src3; /* IxJ */

	int i,j,k;

    for (i = 0; i < sizeI; i++) {
        for (j = 0; j < sizeJ; j++){
        	C[i*sizeJ+j] = 0;
        	for (k = 0; k < sizeK; k++)
            	C[i*sizeJ+j] += (DataType) A[i*sizeK+k] * (DataType) B[k*sizeJ+j];
    	}
	}
}

void core(void* A, void* B, void* C, void* D, void* E, void* F, void* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M){
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	core_kernel(A, B, E, I, J, K);
	core_kernel(C, D, F, J, L, M);
	core_kernel(E, F, G, I, L, J);

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

#ifdef RUN_BLANK
void core(void* A, void* B, void* C, void* D, void* E, void* F, void* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M){
}
#endif // RUN_BLANK
