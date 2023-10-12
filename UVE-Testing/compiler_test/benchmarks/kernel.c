void
core_kernel(void* src1, void* src2, void* src3, int sizeI, int sizeJ, int sizeK) {
    double *A = (double *)src1; /* IxK */
    double *B = (double *)src2; /* KxJ */
    double *C = (double *)src3; /* IxJ */

	int i,j,k;

    for (i = 0; i < sizeI; i++) {
        for (j = 0; j < sizeJ; j++){
        	C[i*sizeJ+j] = 0;
        	for (k = 0; k < sizeK; k++)
            	C[i*sizeJ+j] += (double) A[i*sizeK+k] * (double) B[k*sizeJ+j];
    	}
	}
}
