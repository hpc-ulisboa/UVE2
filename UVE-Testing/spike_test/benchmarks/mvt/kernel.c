#include "Functions.h"

#ifdef RUN_UVE

void core(void *src1, void *src2, void *src3, void *src4, void *src5, int sizeN) {
	int v_len = 16;
    asm volatile(
		// A stream load
		"ss.sta.ld.w           u1, %[src1], %[sn], %[one] \t\n" // D1: vector - linear access size V_len
		"ss.cfg.vec            u1 \t\n"
		"ss.end                u1, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N access size N

		// y_1 stream load (could be simplified to a linear access if vector fill is assured)
		"ss.sta.ld.w           u2, %[src3], %[sn], %[one] \t\n"    // D1: vector - linear access size V_len
		"ss.end                u2, zero, %[sn], zero \t\n"      // repeat: 'i' times

		// x_1 stream load
		"ss.sta.ld.w           u3, %[src2], %[vl], zero \t\n"      // D1: slide horizontaly by 1
		"ss.end                u3, zero, %[sn], %[one] \t\n"    // D1: slide horizontaly by 1

		// x_1 stream store
		"ss.st.w           u4, %[src2], %[sn], %[one] \t\n"    // D1: slide horizontaly by 1

		:
		: [src1] "r"(src1), [src3] "r"(src3), [src2] "r"(src2), 
		[sn] "r"(sizeN), [one] "r" (1),
		[vl] "r" (v_len), [nv] "r" (sizeN/v_len)
	);

	asm volatile(
		// A stream load
		"ss.sta.ld.w           u1, %[src1], %[sn], %[sn] \t\n" // D1: vector - slide verticaly by N
		"ss.cfg.vec            u1 \t\n"
		"ss.end                u1, zero, %[sn], %[one] \t\n"     // D2: slide verticaly by 1

		// y_2 stream load (could be simplified to a linear access if vector fill is assured)
		"ss.sta.ld.w           u2, %[src5], %[sn], %[one] \t\n"    // D1: vector - linear access size V_len
		"ss.end                u2, zero, %[sn], zero \t\n"      // repeat: 'i' times

		// x_2 stream load
		"ss.sta.ld.w           u3, %[src4], %[vl], zero \t\n"      // D1: slide horizontaly by 1
		"ss.end                u3, zero, %[sn], %[one] \t\n"    // D2: slide horizontaly by 1

		// x_2 stream store
		"ss.st.w               u4, %[src4], %[sn], %[one] \t\n"    // D1: slide horizontaly by 1
		:
		: [src1] "r" (src1), [src5] "r" (src5), [src4] "r" (src4), 
		[sn] "r"(sizeN), [one] "r" (1),
		[vl] "r" (v_len), [nv] "r" (sizeN/v_len)
	);

	asm volatile(
        ".fLoop1%=: \t\n"
			"so.v.dp.w  u5, zero, p0\n\t"

			".jloop1%=: \t\n"
				"so.a.mac.fp  u5, u1, u2, p0  \n\t" // inner prod A[i][] * j[]
			"so.b.ndc.1 u1, .jloop1%= \n\t"

			"so.v.mv        u6, u3, p0      \n\t" // load x[i]
			"so.a.adde.fp   u5, u5, p0      \n\t" // reduce vector
			"so.a.add.fp    u4, u5, u6, p0  \n\t" //  x = red + x

        "so.b.nc	u1, .fLoop1%= \n\t"
		:::
    );

}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(void* src1, void* src2, void* src3, void* src4, void* src5, uint64_t sizeN) {
    DataType *A = (DataType *)src1; /* NxN */
    DataType *x1 = (DataType *)src2; /* N */
    DataType *y_1 = (DataType *)src3; /* N */
    DataType *x2 = (DataType *)src4; /* N */
    DataType *y_2 = (DataType *)src5; /* N */

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x1[i] = x1[i] + A[i*sizeN+j] * y_1[j];

    for (int i = 0; i < sizeN; i++)
      for (int j = 0; j < sizeN; j++)
        x2[i] = x2[i] + A[j*sizeN+i] * y_2[j];
}

#endif // RUN_SIMPLE