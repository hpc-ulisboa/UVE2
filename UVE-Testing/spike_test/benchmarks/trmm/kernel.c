#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE

void core(int N, int M, /*DataType alpha,*/ DataType *A, DataType *B) {
    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));
    asm volatile(
        "ss.sta.ld.d.v        u3, %[A] \n"
        "ss.app               u3, zero, %[M], %[Mp1] \n"
        "ss.app.mod.siz.dec.3 u3, %[one] \n"
        "ss.app               u3, zero, %[N], zero\n"
        "ss.end               u3, %[M], %[Mm1], %[M] \n"

        "ss.sta.ld.d u5, %[B] \n"
        "ss.app      u5, zero, %[M], %[N] \n"
        "ss.end      u5, zero, %[N], %[one] \n"

        "ss.sta.st.d u1, %[B] \n"
        "ss.app      u1, zero, %[M], %[N] \n"
        "ss.end      u1, zero, %[N], %[one] \n"

        ".SLOOP_1: \n"
        "so.v.dp.d u2, zero, p0 \n"
        ".SLOOP_1_0_0: \n"
            "so.a.adde.acc.fp  u2, u3, p0 \n"
            "so.b.ndc.3 u3, .SLOOP_1_0_0 \n"
        "so.a.add.fp  u1, u2, u5, p0 \n"
        "so.b.nc u3, .SLOOP_1\n"
        :: [A] "r"(A), [B] "r"(B), [M] "r"(M), [Mp1] "r"(M + 1), [Mm1] "r"(M - 1), [N] "r"(N), [one] "r"(1));

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif
#endif

#ifdef RUN_SIMPLE
void core(int N, int M, /*DataType alpha,*/ DataType *A, DataType *B) {

    asm volatile("rdinstret %[s] \n" : [s] "=&r"(start));

    int i, j, k;

    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++) {
            for (k = i + 1; k < M; k++){
                B[i * N + j] += A[k * M + i]; // * B[k * N + j];
                //printf("(%d) + %lf", k*M+i, A[k * M + i]);
            }
            // B[i * N + j] *= alpha;
        }

    asm volatile("rdinstret %[e] \n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif