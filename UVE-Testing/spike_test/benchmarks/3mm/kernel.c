#include "Functions.h"
#define v_len 16

#ifdef RUN_UVE
void
uve_config(void* src1, void* src2, void* src3, unsigned long int sizeI, unsigned long int sizeJ, unsigned long int sizeK) {
    asm volatile(                        /*offset, size, stride*/
		// B stream (KxJ)
		"ss.sta.ld.d           u2, %[src2], %[sk], %[sj] \t\n"    // D1: slide verticaly stride sizeJ ('sizeK' times)
		"ss.cfg.vec            u2                        \t\n"
		"ss.app                u2, zero, %[sj], %[one] \t\n"      // D2: slide horizontaly by 1, access size sizeJ
		"ss.end                u2, zero, %[si], zero \t\n"        // repeat: for each 'i'

		// A stream (IxK)
		"ss.sta.ld.d           u1, %[src1], %[sk], %[one] \t\n"    // D1: slide horizontaly by 1, access size sizeK
		"ss.cfg.vec            u1                         \t\n"
		"ss.app                u1, zero, %[sj], zero \t\n"        // repeat: for each 'j'
		"ss.end                u1, zero, %[si], %[sk] \t\n"       // D2: slide verticaly stride sizeK access size sizeI

		// C stream store (IxJ)
		"ss.sta.st.d           u4, %[src3], %[sj], %[one] \t\n"   // D1: slide horizontaly by 1, access size sizeJ
		"ss.end                u4, zero, %[si], %[sj] \t\n"        // D2: slide verticaly stride sizeJ access size sizeI

		:
		: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(sizeI), [sj] "r"(sizeJ), [sk] "r"(sizeK), [one] "r" (1));
}

void
uve_kernel() {
    asm volatile(
        "iLoop1: \t\n"    
            "so.v.dp.d u21, zero, p0 \t\n"
            "kloop1: \t\n"
              "so.a.mac.fp u21, u1, u2, p0\n\t" // tmp += (.A) * B
            "so.b.ndc.1 u2, kloop1 \n\t"
            "so.a.adde.fp  u4, u21, p0 \n\t" // store tmp to C 
        "so.b.nc	u2, iLoop1 \n\t"
        );
}

void
core(void* src1, void* src2, void* src3, unsigned long int sizeI, unsigned long int sizeJ, unsigned long int sizeK){
	uve_config(src1, src2, src3, sizeI, sizeJ, sizeK);
	uve_kernel();
}

/*
		// A stream
		"ss.sta.ld.d           u1, %[src1], %[si], %[si] \t\n"
		"ss.app				   u1, zero, %[si], zero \t\n"
		"ss.cfg.vec			   u1 \n\t"
		"ss.end                u1, zero, %[si], %[one] \t\n"

		// B stream
		"ss.sta.ld.d           u2, %[src2], %[si], zero \t\n"
		"ss.app                u2, zero, %[si], %[one] \t\n"
		"ss.cfg.vec            u2 \n\t"
		"ss.end                u2, zero, %[si], %[si] \t\n"

		// C stream store
		"ss.sta.st.d           u4, %[src3], %[si], %[si] \t\n"
		"ss.end                u4, zero, %[si], %[one] \t\n"
		:
		: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
		[si] "r"(SIZE), [sj] "r"(SIZE), [sk] "r"(SIZE), [one] "r"(1), [vl] "r"(v_len), [jv] "r"(SIZE / v_len)
	);

    asm volatile(
		"so.v.dp.d u21, zero, p0 \t\n"
        ".iLoop1%=: \t\n"
			".kloop1%=: \t\n"
				"so.a.mac.fp u21, u1, u2, p0\n\t" // tmp += (.A) * B
			"so.b.ndc.2 u2, .kloop1%= \n\t"
        "so.b.nc	u2, .iLoop1%= \n\t"
		"so.v.mv  u4, u21, p0 \n\t" // store tmp to C
		:::
	);

*/

#endif // RUN_UVE

#ifdef RUN_SIMPLE
void
core(void* src1, void* src2, void* src3, unsigned long int sizeI, unsigned long int sizeJ, unsigned long int sizeK) {
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
#endif // RUN_SIMPLE