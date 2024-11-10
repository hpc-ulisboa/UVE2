#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	
	asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		"ss.sta.st.d.v  u1, %[dest] \n"
		"ss.end		    u1, zero, %[size], %[stride] \n"
		
		"ss.sta.ld.d.v  u2, %[src] \n"
		"ss.end		    u2, zero, %[size], %[stride] \n"

		".uve_loop%= : \n"
			"so.v.mv u1, u2, p0 \n"
		"so.b.nc u2, .uve_loop%= \n"

		:: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#if defined(F_TYPE) || defined(I_TYPE) 
void core(DataType dest[SIZE], DataType src[SIZE]) {
	
	asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		"ss.sta.st.w.v  u1, %[dest] \n"
		"ss.end		    u1, zero, %[size], %[stride] \n"
		
		"ss.sta.ld.w.v  u2, %[src] \n"
		"ss.end		    u2, zero, %[size], %[stride] \n"

		".uve_loop%= : \n"
			"so.v.mv u1, u2, p0 \n"
		"so.b.nc u2, .uve_loop%= \n"

		:: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE || I_TYPE
#ifdef H_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	
	asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		"ss.sta.st.h.v  u1, %[dest] \n"
		"ss.end		    u1, zero, %[size], %[stride] \n"
		
		"ss.sta.ld.h.v  u2, %[src] \n"
		"ss.end		    u2, zero, %[size], %[stride] \n"

		".uve_loop%= : \n"
			"so.v.mv u1, u2, p0 \n"
		"so.b.nc u2, .uve_loop%= \n"

		:: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	
	asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
	
	asm volatile(
		"ss.sta.st.b.v  u1, %[dest] \n"
		"ss.end		    u1, zero, %[size], %[stride] \n"
		
		"ss.sta.ld.b.v  u2, %[src] \n"
		"ss.end		    u2, zero, %[size], %[stride] \n"

		".uve_loop%= : \n"
			"so.v.mv u1, u2, p0 \n"
		"so.b.nc u2, .uve_loop%= \n"

		:: [dest] "r" (dest),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));

	asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE


#ifdef RUN_SIMPLE
void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));

	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i];
	}

	asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}
#endif // RUN_SIMPLE

