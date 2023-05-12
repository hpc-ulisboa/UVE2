#include "Functions.h"

#ifdef RUN_UVE
#define v_len 16
void core(void *src1, void *src2, void *src3)
{
    asm volatile(/*offset, size, stride*/ /*mod-> size, disp*/
        // L(i,j) stream load
        "ss.sta.ld.w           u1, %[src1], %[sndx], %[one] \t\n" // D1: linear access (initial size: 0)
        "ss.app                u1, zero, %[snm1], %[sn] \t\n"     // D2: slide verticaly stride N access size N-1
        "ss.end.mod.siz.inc    u1, %[snm1], %[one] \t\n"          // Modifier->D1: increment D1 size N-1

        // x(j) stream load
        "ss.sta.ld.w           u2, %[src3], %[sndx], %[one] \t\n" // D1: vector - linear access (initial size: 0)
        "ss.app                u2, zero, %[snm1], zero \t\n"      // Repeat N-1 times
        "ss.end.mod.siz.inc    u2, %[snm1], %[one] \t\n"          // Modifier->D1: increment D1 size N-1

        // b stream scalar load (?)
        "ss.sta.ld.w           u3, %[src2], %[vl], zero \t\n"  // D0: scalar access
        "ss.end                u3, zero, %[snm1], %[one] \t\n" // D1: vector - linear access

        // L(i,i) stream scalar load (?)
        "ss.sta.ld.w               u4, %[src1], %[vl], zero \t\n"   // D0: scalar access
        "ss.end                    u4, zero, %[snm1], %[snp1] \t\n" // D1: diagonal access size N

        // x stream scalar store (?)
        "ss.st.w               u5, %[src2], %[snm1], %[one] \t\n" // D1: vector - linear access

        :
        : [src1] "r"(src1), [src2] "r"(src2), [src3] "r"(src3),
        [sn] "r"(SIZE), [snm1] "r"(SIZE - 1), [snp1] "r"(SIZE + 1), [one] "r"(1),
        [vl] "r"(v_len), [iv] "r"(SIZE / v_len), [sndx] "r"(SIZE / 2)
    );

    asm volatile(
        // "so.a.div.fp    u5, u3, u4, p0  \n\t" //  x = b / L

        ".fLoop1%=: \t\n"

          "so.v.dp.w  u6, zero, p0\n\t" // u6 = 0
          ".jloop1%= : \t\n"
            "so.a.mul.fp u7, u1, u2, p0\n\t" // u7 = L(i,j) * x(j)
            "so.a.sub.fp u6, u6, u7, p0\n\t" // u6 = u6 - u7 
          "so.b.ndc.1 u1, .jloop1%= \n\t"
          
          //"so.a.adde.fp   u7, u6, p0      \n\t" // reduce vector
          "so.a.adde.fp   u5, u6, p0      \n\t" // reduce vector
          //"so.a.add.fp    u7, u3, u7, p0  \n\t" //  t = b + red
          //"so.a.div.fp    u5, u7, u4, p0  \n\t" //  x = t / L
        
        "so.b.nc	u1, .fLoop1%= \n\t"
        :::
    );
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType L[SIZE][SIZE], DataType b[SIZE], DataType x[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        x[i] = b[i];
        for (int j = 0; j < i; j++)
            x[i] -= L[i][j] * x[j];
        x[i] = x[i] / L[i][i];
    }
}
#endif // RUN_SIMPLE
