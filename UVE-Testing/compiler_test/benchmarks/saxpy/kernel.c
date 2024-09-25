#include "Functions.h"
#include <stdio.h>

long int start = 0, end = 0;

void core(DataType dest[SIZE], DataType src[SIZE], DataType A) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    for (int i = 0; i < SIZE; i++) {
        dest[i] += src[i] * A;
    }
    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}