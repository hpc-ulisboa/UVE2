#include "Functions.h"


#ifdef RUN_UVE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out) {
	asm volatile(
		"ss.st.d  u1, %[dest], %[size], %[stride] \t\n"
		"ss.cfg.vec u1 \t\n"
		"ss.ld.d  u2,  %[src], %[size], %[stride] \t\n"
		"ss.cfg.vec u2 \t\n"
		:: [dest] "r" (dest),
      [src] "r" (src),
      [size] "r" (SIZE),
      [stride] "r" (1));
	asm volatile(
		".uve_loop%= : \t\n"
			"so.v.mv u1, u2, p0 \n\t"
		"so.b.nc u2, .uve_loop%= \n\t"
		 :::);
}
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType *nzval, int32_t *cols, DataType *vec, DataType *out)
{
    int i, j;
    DataType Si;

    for (i=0; i<N; i++) {
        DataType sum = out[i];
        for (j=0; j<L; j++) {
                Si = nzval[j + i*L] * vec[cols[j + i*L]];
                sum += Si;
        }
        out[i] = sum;
    }
}
#endif // RUN_SIMPLE