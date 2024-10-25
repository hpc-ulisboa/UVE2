#include "Functions.h"

long int start = 0, end = 0;

// void core(DataType *A, int N) {
void core(DataType *A, int N) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int i=0, j=0, k=0;

    for (i = 0; i < N; i++) {
        for (j = 0; j < i; j++) {
            for (k = 0; k < j; k++) {
                A[i*N+j] -= A[i*N+k] * A[k*N+j];
                //printf("MUL     %lf * %lf = %lf\n", A[i*N+k], A[k*N+j], A[i*N+k]*A[k*N+j]);
                //printf("SUB     %lf - %lf = %lf\n", A[i*N+j], A[i*N+k]*A[k*N+j], A[i*N+j]-A[i*N+k]*A[k*N+j]);
            }
            A[i*N+j] /= A[j*N+j];
            //printf("DIV     %lf / %lf = %lf\n", A[i*N+j], A[j*N+j], A[i*N+j]/A[j*N+j]);
        }
        for (j = i; j < N; j++) {
            for (k = 0; k < i; k++) {
                A[i*N+j] -= A[i*N+k] * A[k*N+j];
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}