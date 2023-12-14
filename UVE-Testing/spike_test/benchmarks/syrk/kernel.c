#include "Functions.h"

#ifdef RUN_UVE
void core(int N, int M, DataType alpha, DataType beta, DataType *C, DataType *A) {
  asm volatile(
    "ss.sta.st.d 		u1, %[C], %[N], %[N] \n\t"
    "ss.app.mod.siz.inc u1, %[N], %[one] \n\t"
    "ss.end 			u1, zero, %[one], %[one] \n\t"
    "ss.cfg.vec 		u1 	\n\t"

    "ss.sta.ld.d 		u2, %[C], %[N], %[N] \n\t"
    "ss.app.mod.siz.inc u2, %[N], %[one] \n\t"
    "ss.end 			u2, zero, %[one], %[one] \n\t"
    "ss.cfg.vec 		u2 	\n\t"

    "ss.sta.st.d 		u3, %[C], %[N], %[N] \n\t"
    "ss.app 			u3, zero, %[M], zero \n\t"
    "ss.app.mod.siz.inc u3, %[N], %[one] \n\t"
    "ss.end 			u3, zero, %[one], %[one] \n\t"
    "ss.cfg.vec 		u3 \n\t"

    "ss.sta.ld.d 		u4, %[C], %[N], %[N] \n\t"
    "ss.app 			u4, zero, %[M], zero \n\t"
    "ss.app.mod.siz.inc u4, %[N], %[one] \n\t"
    "ss.end 			u4, zero, %[one], %[one] \n\t"
    "ss.cfg.vec			u4 \n\t"

    "ss.sta.ld.d 		u5, %[A], %[N], zero \n\t"
    "ss.app				u5, zero, %[M], %[one] \n\t"
    "ss.app.mod.siz.inc u5, %[N], %[one] \n\t"
    "ss.end 			u5, zero, %[one], %[M] \n\t"
    "ss.cfg.vec 		u5 \n\t"

    "ss.sta.ld.d 		u6, %[A], %[N], %[M] \n\t"
    "ss.app 			u6, zero, %[M], %[one] \n\t"
    "ss.app.mod.siz.inc u6, %[N], %[one] \n\t"
    "ss.end 			u6, zero, %[one], zero \n\t"
    "ss.cfg.vec 		u6 \n\t"

    "so.v.dp.d 			u7, %[beta], p0 \n\t"
    "so.v.dp.d 			u8, %[alpha], p0 \n\t"
	:
	: [N] "r" (N), [M] "r" (M), [alpha] "r" (alpha), [beta] "r" (beta), [C] "r" (C), [A] "r" (A), [one] "r" (1));

asm volatile (
    ".SLOOP_1: \n\t"
    	".SLOOP_1_0: \n\t"
    		"so.a.mul.fp  u1, u2, u7, p0 \n\t" // C[i][j] *= beta
    	"so.b.ndc.1 u1, .SLOOP_1_0 \n\t"

    	".SLOOP_1_1: \n\t"
    		"so.a.mul.fp  u9, u6, u8, p0 \n\t"  // aux = alpha * A[i][k]
    		"so.a.mul.fp  u10, u9, u5, p0 \n\t" // aux1 = aux * A[j][k]
    		"so.a.add.fp  u3, u4, u10, p0 \n\t" // C[i][j] += aux1
    	"so.b.ndc.2 u3, .SLOOP_1_1 \n\t"
    "so.b.nc u1, .SLOOP_1 \n\t":::);
 }

#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(int N, int M, DataType alpha, DataType beta, DataType *C /* N * N */, DataType *A /* N * M */) {
	int i, j, k;

	for (i = 0; i < N; i++) {
		for (j = 0; j <= i; j++) {
			printf("L1 C[%d][%d] = %lf * %lf = %lf\n", i, j, C[i*N+j], beta, C[i*N+j]*beta);
			C[i*N+j] *= beta;
		}
		printf("\n");
		for (k = 0; k < M; k++) {
			for (j = 0; j <= i; j++){
				printf("L2 C[%d][%d] = %lf + %lf = %lf\n", i, j, C[i*N+j], alpha * A[i*M+k] * A[j*M+k], C[i*N+j] + alpha * A[i*M+k] * A[j*M+k]);
				C[i*N+j] += alpha * A[i*M+k] * A[j*M+k];
			}
			printf("\n");
		}
	}
}

#endif // RUN_SIMPLE
