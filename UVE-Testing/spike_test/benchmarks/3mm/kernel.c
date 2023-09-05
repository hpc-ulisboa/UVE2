#include "Functions.h"
#define v_len 16

#ifdef RUN_UVE
void core(DataType *src1, DataType *src2, DataType *src3) {
    asm volatile(/*offset, size, stride*/

		// A stream 
		"ss.sta.ld.w           u1, %[src1], %[vl], zero \t\n"  // D1: vector element - linear access size one
		"ss.app                u1, zero, %[sk], %[one] \t\n"    // D2: slide horizontaly by 1, access size K
		"ss.app                u1, zero, %[jv], zero \t\n"        // D3: slide horizontaly by V_len, access size J/V_len
		"ss.end                u1, zero, %[si], %[sk] \t\n"     // D3: slide verticaly stride K access size I

		// B stream
		"ss.sta.ld.w           u2, %[src2], %[vl], %[one] \t\n"    // D1: vector - linear access size V_len
		"ss.app                u2, zero, %[sk], %[sj] \t\n"        // D2: slide verticaly stride N ('k' times)
		"ss.cfg.vec            u2\n\t"
		"ss.app                u2, zero, %[jv], %[vl] \t\n"        // D3: slide horizontaly by V_len, access size J/V_len
		"ss.cfg.vec            u2\n\t"
		"ss.end                u2, zero, %[si], zero \t\n"    // repeat: for each 'i'

		// C stream store
		"ss.sta.st.w           u4, %[src3], %[vl], %[one] \t\n" // D1: vector - linear access size V_len
		"ss.app                u4, zero, %[jv], %[vl] \t\n"     // D2: slide horizontaly by V_len, access size J/V_len
		"ss.end                u4, zero, %[si], %[sj] \t\n"     // D3: slide verticaly stride J access size I

		:
		: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3), 
		[si] "r"(SIZE), [sj] "r"(SIZE), [sk] "r"(SIZE), [one] "r" (1), [vl] "r" (v_len), [jv] "r" (SIZE/v_len)
		);

    asm volatile(
        ".iLoop1%=: \t\n"    
            "so.v.dp.w u21, zero, p0 \t\n"
            ".kloop1%=: \t\n"
              "so.a.mac.fp u21, u1, u2, p0\n\t" // tmp += (.A) * B
            "so.b.ndc.2 u2, .kloop1%= \n\t"
            "so.v.mv  u4, u21, p0 \n\t" // store tmp to C 
        "so.b.nc	u2, .iLoop1%= \n\t"
		:::
        );
}

/*
		// A stream
		"ss.sta.ld.w           u1, %[src1], %[si], %[si] \t\n"
		"ss.app				   u1, zero, %[si], zero \t\n"
		"ss.cfg.vec			   u1 \n\t"
		"ss.end                u1, zero, %[si], %[one] \t\n"

		// B stream
		"ss.sta.ld.w           u2, %[src2], %[si], zero \t\n"
		"ss.app                u2, zero, %[si], %[one] \t\n"
		"ss.cfg.vec            u2 \n\t"
		"ss.end                u2, zero, %[si], %[si] \t\n"

		// C stream store
		"ss.sta.st.w           u4, %[src3], %[si], %[si] \t\n"
		"ss.end                u4, zero, %[si], %[one] \t\n"
		:
		: [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
		[si] "r"(SIZE), [sj] "r"(SIZE), [sk] "r"(SIZE), [one] "r"(1), [vl] "r"(v_len), [jv] "r"(SIZE / v_len)
	);

    asm volatile(
		"so.v.dp.w u21, zero, p0 \t\n"
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
void core(DataType *src1, DataType *src2, DataType *src3) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j)
            src3[i * SIZE + j] = 0;
        for (int k = 0; k < SIZE; ++k) {
            for (int j = 0; j < SIZE; ++j)
                src3[i * SIZE + j] += src1[i * SIZE + k] * src2[k * SIZE + j];
        }
    }
}
#endif // RUN_SIMPLE