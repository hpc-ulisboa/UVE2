#include "Functions.h"

long int start = 0, end = 0;

void core(DataType dest[SIZE], DataType src[SIZE]) {
	asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));

	for (int i = 0; i < SIZE; ++i) {
		dest[i] = src[i];
	}

	asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
	printf("%ld\n%ld\n", start, end);
}