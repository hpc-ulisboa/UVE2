/*
ss.sta.ld.w.v.2       u1, &A 
ss.app	              u1, 0, M, N 
ss.app.mod.siz.inc.1  u1, 1
ss.end 	              u1, 0, 1, 1

ss.sta.st.w	 u2, &a
ss.end 	 u2, 0, N, 1

.iloop:
    li a1, 0
    .jloop:
        so.a.adds.acc.sg a1, u1, p0
        so.b.ndc.1   u1, .jloop
    so.v.mvsv.w u2, a1
    so.b.nc      u1, .iloop 
*/
#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#if defined(I_TYPE)
void core(DataType dest[SIZE], DataType src[SIZE]) {
    DataType aux = 0;
	asm volatile(
		"rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

		"ss.sta.ld.w           u1, %[src], %[sn], %[sn] \t\n"
        "ss.app.mod.siz.inc    u1, %[snm1], %[one] \t\n"
        "ss.end                u1, zero, zero, %[one] \t\n"
        "ss.cfg.vec            u1 \t\n"

        "ss.st.w               u2, %[dest], %[sn], %[one] \t\n"

		".iLoop%=: \t\n"

            "li %[aux], 0 \t\n"

            ".jloop%= : \t\n"
                "so.a.adds.acc.sg %[aux], u1, p0\n\t"
            "so.b.ndc.1 u1, .jloop%= \n\t"

            "so.v.mvsv.w u2, %[aux] \t\n"

        "so.b.nc	u1, .iLoop%= \n\t" 

		"rdinstret %[end] \t\n"

		: [start] "=&r"(start), [end] "=&r"(end), [aux] "=&r"(aux)
		: [dest] "r" (dest),
		  [src] "r" (src),
	      [sn] "r" (SIZE),
	      [snm1] "r" (SIZE-1),
		  [one] "r" (1));

	printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < i; ++j) {
            dest[i] += src[i*SIZE+j];
        }
    }

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

