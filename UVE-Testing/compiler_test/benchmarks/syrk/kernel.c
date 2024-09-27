#include "Functions.h"

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