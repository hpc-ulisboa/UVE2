#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(int N, int M, DataType alpha, DataType beta, DataType *C, DataType *A) {

  asm volatile("rdinstret %[s] \n":[s] "=&r"(start));

  asm volatile(
    "ss.sta.st.d 		    u1, %[C], %[N], %[N] \n"
    "ss.app.mod.siz.inc u1, %[N], %[one] \n"
    "ss.end 			      u1, zero, %[one], %[one] \n"
    "ss.cfg.vec 		    u1 	\n"

    "ss.sta.ld.d 		    u2, %[C], %[N], %[N] \n"
    "ss.app.mod.siz.inc u2, %[N], %[one] \n"
    "ss.end 			      u2, zero, %[one], %[one] \n"
    "ss.cfg.vec 		    u2 	\n"

    "ss.sta.st.d 		    u3, %[C], %[N], %[N] \n"
    "ss.app 			      u3, zero, %[M], zero \n"
    "ss.app.mod.siz.inc u3, %[N], %[one] \n"
    "ss.end 			      u3, zero, %[one], %[one] \n"
    "ss.cfg.vec 		    u3 \n"

    "ss.sta.ld.d 		    u4, %[C], %[N], %[N] \n"
    "ss.app 			      u4, zero, %[M], zero \n"
    "ss.app.mod.siz.inc u4, %[N], %[one] \n"
    "ss.end 			      u4, zero, %[one], %[one] \n"
    "ss.cfg.vec			    u4 \n"

    "ss.sta.ld.d 		    u5, %[A], %[N], zero \n"
    "ss.app				      u5, zero, %[M], %[one] \n"
    "ss.app.mod.siz.inc u5, %[N], %[one] \n"
    "ss.end 			      u5, zero, %[one], %[M] \n"
    "ss.cfg.vec 		    u5 \n"

    "ss.sta.ld.d 		    u6, %[A], %[N], %[M] \n"
    "ss.app 			      u6, zero, %[M], %[one] \n"
    "ss.app.mod.siz.inc u6, %[N], %[one] \n"
    "ss.end 			      u6, zero, %[one], zero \n"
    "ss.cfg.vec 		    u6 \n"

    "so.v.dp.d 			    u7, %[beta], p0 \n"
    "so.v.dp.d 			    u8, %[alpha], p0 \n"
	:
	: [N] "r" (N), [M] "r" (M), [alpha] "r" (alpha), [beta] "r" (beta), [C] "r" (C), [A] "r" (A), [one] "r" (1));

  asm volatile (
      ".SLOOP_1: \n"
        ".SLOOP_1_0: \n"
          "so.a.mul.fp  u1, u2, u7, p0 \n" // C[i][j] *= beta
        "so.b.ndc.1 u1, .SLOOP_1_0 \n"

        ".SLOOP_1_1: \n"
          "so.a.mul.fp  u9, u6, u8, p0 \n"  // aux = alpha * A[i][k]
          "so.a.mul.fp  u10, u9, u5, p0 \n" // aux1 = aux * A[j][k]
          "so.a.add.fp  u3, u4, u10, p0 \n" // C[i][j] += aux1
        "so.b.ndc.2 u3, .SLOOP_1_1 \n"
      "so.b.nc u1, .SLOOP_1 \n":::);

  asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
 }
#endif // D_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(int N, int M, DataType alpha, DataType beta, DataType *C /* N * N */, DataType *A /* N * M */) {

  asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

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
  
  asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
  printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE
