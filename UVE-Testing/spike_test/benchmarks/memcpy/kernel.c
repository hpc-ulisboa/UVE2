#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile(
		"rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

		"ss.st.d  u1, %[dest], %[size], %[stride] \t\n"
		"ss.cfg.vec u1 \t\n"
		
		"ss.ld.d  u2,  %[src], %[size], %[stride] \t\n"
		"ss.cfg.vec u2 \t\n"

		".uve_loop%= : \t\n"
			"so.v.mv u1, u2, p0 \n\t"
		"so.b.nc u2, .uve_loop%= \n\t"

		"rdinstret %[end] \t\n"

		: [start] "=&r"(start), [end] "=&r"(end)
		: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#if defined(F_TYPE) || defined(I_TYPE) 
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile(
		"rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

		"ss.st.w  u1, %[dest], %[size], %[stride] \t\n"
		"ss.cfg.vec u1 \t\n"
		
		"ss.ld.w  u2,  %[src], %[size], %[stride] \t\n"
		"ss.cfg.vec u2 \t\n"

		".uve_loop%= : \t\n"
			"so.v.mv u1, u2, p0 \n\t"
		"so.b.nc u2, .uve_loop%= \n\t"

		"rdinstret %[end] \t\n"

		: [start] "=&r"(start), [end] "=&r"(end)
		: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE || I_TYPE
#ifdef H_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile(
		"rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

		"ss.st.h  u1, %[dest], %[size], %[stride] \t\n"
		"ss.cfg.vec u1 \t\n"
		
		"ss.ld.h  u2,  %[src], %[size], %[stride] \t\n"
		"ss.cfg.vec u2 \t\n"

		".uve_loop%= : \t\n"
			"so.v.mv u1, u2, p0 \n\t"
		"so.b.nc u2, .uve_loop%= \n\t"

		"rdinstret %[end] \t\n"

		: [start] "=&r"(start), [end] "=&r"(end)
		: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile(
		"rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

		"ss.st.b  u1, %[dest], %[size], %[stride] \t\n"
		"ss.cfg.vec u1 \t\n"
		
		"ss.ld.b  u2,  %[src], %[size], %[stride] \t\n"
		"ss.cfg.vec u2 \t\n"

		".uve_loop%= : \t\n"
			"so.v.mv u1, u2, p0 \n\t"
		"so.b.nc u2, .uve_loop%= \n\t"

		"rdinstret %[end] \t\n"

		: [start] "=&r"(start), [end] "=&r"(end)
		: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i];
	}

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

