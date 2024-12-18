#include "Functions.h"

#ifdef RUN_UVE
#ifdef D_TYPE
long int uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
		// A stream (IxK)
		"ss.sta.ld.d.v.m       u1, %[src1] \n"			   // Load A, vectorised at D1
		"ss.app				   u1, zero, %[si], %[sk] \n"    // D1: slide vertically (stride sizeK), access size sizeI
		"ss.app                u1, zero, %[sj], zero \n"     // D2: repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \n"   // D3: slide horizontally by 1, access size sizeK

		// B stream (KxJ)
		"ss.sta.ld.d.v.m       u2, %[src2] \n"			   // Load B, vectorised at D1
		"ss.app				   u2, zero, %[si], zero \n"     // D1: repeat: for each 'i'     
		"ss.app                u2, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		"ss.end                u2, zero, %[sk], %[sj] \n"    // D3: slide vertically (stride sizeJ), access size sizeK

		// C stream store (IxJ)
		"ss.sta.st.d           u4, %[src3] \n"			   // Store C, scalar
		"ss.app				   u4, zero, %[si], %[sj] \n"    // D1: slide vertically (stride sizeJ), access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ

		".iLoop1%=: \n"    
            "so.v.dp.d u21, zero, p0 \n"
            ".kloop1%=: \n"
				"so.a.mac.fp u21, u1, u2, p0 \n" // tmp += (.A) * B
            	"so.b.ndc.3 u2, .kloop1%= \n"
            "so.a.adde.fp  u4, u21, p0 \n" // store tmp to C 
        	"so.b.nc	u2, .iLoop1%= \n"

		:: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));
		
		asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

		return end - start;
}
#endif // D_TYPE
#ifdef F_TYPE
long int uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
		// A stream (IxK)
		"ss.sta.ld.w.v.m       u1, %[src1] \n"			   // Load A, vectorised at D1
		"ss.app				   u1, zero, %[si], %[sk] \n"    // D1: slide vertically (stride sizeK), access size sizeI
		"ss.app                u1, zero, %[sj], zero \n"     // D2: repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \n"   // D3: slide horizontally by 1, access size sizeK

		// B stream (KxJ)
		"ss.sta.ld.w.v.m       u2, %[src2] \n"			   // Load B, vectorised at D1
		"ss.app				   u2, zero, %[si], zero \n"     // D1: repeat: for each 'i'     
		"ss.app                u2, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		"ss.end                u2, zero, %[sk], %[sj] \n"    // D3: slide vertically (stride sizeJ), access size sizeK

		// C stream store (IxJ)
		"ss.sta.st.w           u4, %[src3] \n"			   // Store C, scalar
		"ss.app				   u4, zero, %[si], %[sj] \n"    // D1: slide vertically (stride sizeJ), access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ

		".iLoop1%=: \n"    
            "so.v.dp.w u21, zero, p0 \n"
            ".kloop1%=: \n"
				"so.a.mac.fp u21, u1, u2, p0 \n" // tmp += (.A) * B
            	"so.b.ndc.3 u2, .kloop1%= \n"
            "so.a.adde.fp  u4, u21, p0 \n" // store tmp to C 
        	"so.b.nc	u2, .iLoop1%= \n"

		:: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));

		asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

		return end - start;
}
#endif // F_TYPE
#ifdef I_TYPE
long int uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
		"rdinstret %[s] \n"

		// A stream (IxK)
		"ss.sta.ld.w.v.m       u1, %[src1] \n"			   // Load A, vectorised at D1
		"ss.app				   u1, zero, %[si], %[sk] \n"    // D1: slide vertically (stride sizeK), access size sizeI
		"ss.app                u1, zero, %[sj], zero \n"     // D2: repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \n"   // D3: slide horizontally by 1, access size sizeK

		// B stream (KxJ)
		"ss.sta.ld.w.v.m       u2, %[src2] \n"			   // Load B, vectorised at D1
		"ss.app				   u2, zero, %[si], zero \n"     // D1: repeat: for each 'i'     
		"ss.app                u2, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		"ss.end                u2, zero, %[sk], %[sj] \n"    // D3: slide vertically (stride sizeJ), access size sizeK

		// C stream store (IxJ)
		"ss.sta.st.w           u4, %[src3] \n"			   // Store C, scalar
		"ss.app				   u4, zero, %[si], %[sj] \n"    // D1: slide vertically (stride sizeJ), access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ

		".iLoop1%=: \n"    
            "so.v.dp.w u21, zero, p0 \n"
            ".kloop1%=: \n"
				"so.a.mac.sg u21, u1, u2, p0 \n" // tmp += (.A) * B
            	"so.b.ndc.3 u2, .kloop1%= \n"
            "so.a.adde.sg  u4, u21, p0 \n" // store tmp to C 
        	"so.b.nc	u2, .iLoop1%= \n"

		:: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));

		asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

		return end - start;
}
#endif // I_TYPE
#ifdef H_TYPE
long int uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
		"rdinstret %[s] \n"

		// A stream (IxK)
		"ss.sta.ld.h.v.m       u1, %[src1] \n"			   // Load A, vectorised at D1
		"ss.app				   u1, zero, %[si], %[sk] \n"    // D1: slide vertically (stride sizeK), access size sizeI
		"ss.app                u1, zero, %[sj], zero \n"     // D2: repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \n"   // D3: slide horizontally by 1, access size sizeK

		// B stream (KxJ)
		"ss.sta.ld.h.v.m       u2, %[src2] \n"			   // Load B, vectorised at D1
		"ss.app				   u2, zero, %[si], zero \n"     // D1: repeat: for each 'i'     
		"ss.app                u2, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		"ss.end                u2, zero, %[sk], %[sj] \n"    // D3: slide vertically (stride sizeJ), access size sizeK

		// C stream store (IxJ)
		"ss.sta.st.h           u4, %[src3] \n"			   // Store C, scalar
		"ss.app				   u4, zero, %[si], %[sj] \n"    // D1: slide vertically (stride sizeJ), access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ

		".iLoop1%=: \n"    
            "so.v.dp.h u21, zero, p0 \n"
            ".kloop1%=: \n"
				"so.a.mac.sg u21, u1, u2, p0 \n" // tmp += (.A) * B
            	"so.b.ndc.3 u2, .kloop1%= \n"
            "so.a.adde.sg  u4, u21, p0 \n" // store tmp to C 
        	"so.b.nc	u2, .iLoop1%= \n"

		:: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));

		asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

		return end - start;
}
#endif // H_TYPE
#ifdef B_TYPE
long int uve_kernel(void* src1, void* src2, void* src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

    asm volatile(
		"rdinstret %[s] \n"

		// A stream (IxK)
		"ss.sta.ld.b.v.m       u1, %[src1] \n"			   // Load A, vectorised at D1
		"ss.app				   u1, zero, %[si], %[sk] \n"    // D1: slide vertically (stride sizeK), access size sizeI
		"ss.app                u1, zero, %[sj], zero \n"     // D2: repeat: for each 'j'
		"ss.end                u1, zero, %[sk], %[one] \n"   // D3: slide horizontally by 1, access size sizeK

		// B stream (KxJ)
		"ss.sta.ld.b.v.m       u2, %[src2] \n"			   // Load B, vectorised at D1
		"ss.app				   u2, zero, %[si], zero \n"     // D1: repeat: for each 'i'     
		"ss.app                u2, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		"ss.end                u2, zero, %[sk], %[sj] \n"    // D3: slide vertically (stride sizeJ), access size sizeK

		// C stream store (IxJ)
		"ss.sta.st.b           u4, %[src3] \n"			   // Store C, scalar
		"ss.app				   u4, zero, %[si], %[sj] \n"    // D1: slide vertically (stride sizeJ), access size sizeI
		"ss.end                u4, zero, %[sj], %[one] \n"   // D2: slide horizontally by 1, access size sizeJ
		".iLoop1%=: \n"    
            "so.v.dp.b u21, zero, p0 \n"
            ".kloop1%=: \n"
				"so.a.mac.sg u21, u1, u2, p0 \n" // tmp += (.A) * B
            	"so.b.ndc.3 u2, .kloop1%= \n"
            "so.a.adde.sg  u4, u21, p0 \n" // store tmp to C 
        	"so.b.nc	u2, .iLoop1%= \n"

		:: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));

		asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

		return end - start;
}
#endif // B_TYPE

void core(DataType *A, DataType *B, DataType *C, DataType *D, DataType *E, DataType *F, DataType *G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M) {

	long int a = uve_kernel(A, B, E, I, J, K);
	a += uve_kernel(C, D, F, J, L, M);
	a += uve_kernel(E, F, G, I, L, J);

	printf("%d\n%ld\n", 0, a);
}

#endif // RUN_UVE

#ifdef RUN_SIMPLE
long int core_kernel(DataType *src1, DataType *src2, DataType *src3, uint64_t sizeI, uint64_t sizeJ, uint64_t sizeK) {
	long int start = 0, end = 0;
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

	int i,j,k;

    for (i = 0; i < sizeI; i++) {
        for (j = 0; j < sizeJ; j++){
        	for (k = 0; k < sizeK; k++)
            	src3[i*sizeJ+j] += src1[i*sizeK+k] * src2[k*sizeJ+j];
    	}
	}

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));

	return end - start;
}

void core(DataType* A, DataType* B, DataType* C, DataType* D, DataType* E, DataType* F, DataType* G, uint64_t I, uint64_t J, uint64_t K, uint64_t L, uint64_t M){

	long int a = core_kernel(A, B, E, I, J, K);
	a += core_kernel(C, D, F, J, L, M);
	a += core_kernel(E, F, G, I, L, J);

	printf("%d\n%ld\n", 0, a);
}
#endif // RUN_SIMPLE
