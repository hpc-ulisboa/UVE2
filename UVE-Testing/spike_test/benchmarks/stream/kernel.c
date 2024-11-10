#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
    asm volatile(
        // KERNEL COPY

        "ss.sta.ld.d.v  u1, %[a] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.d.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop1%=: \n"
          "so.v.mv  u30, u1 ,p0 \n" // c[] = a[]
        "so.b.nc	u1, .loop1%= \n"
        
        // KERNEL SCALE

        "ss.sta.ld.d.v  u1, %[c] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.d.v  u30, %[b] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.d    u10, %[sc], p0 \n"

        ".loop2%=: \n"
          "so.a.mul.fp  u30, u1, u10, p0 \n" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%= \n"
    
        // KERNEL ADD

        "ss.sta.ld.d.v  u1, %[a] \n"
        "ss.end         u1, zero,%[sn], %[one] \n" //z[i]

        "ss.sta.ld.d.v  u2, %[b] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.d.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop3%=: \n"
          "so.a.add.fp  u30, u1 ,u2 ,p0 \n" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%= \n"
        
        // KERNEL TRIAD

        "ss.sta.ld.d.v  u1, %[b] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.ld.d.v  u2, %[c] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.d.v  u30, %[a] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.d    u10, %[sc], p0 \n"

        ".loop4%=: \n"
          "so.a.mul.fp  u20, u2, u10, p0 \n" // tmp = scalar * c[]
          "so.a.add.fp  u30, u1, u20, p0 \n" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%= \n"
        
        :: [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
    asm volatile(
        // KERNEL COPY

        "ss.sta.ld.w.v  u1, %[a] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]
         
        "ss.sta.st.w.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop1%=: \n"
          "so.v.mv  u30, u1 ,p0 \n" // c[] = a[]
        "so.b.nc	u1, .loop1%= \n"
        
        // KERNEL SCALE

        "ss.sta.ld.w.v  u1, %[c] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[b] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \n"

        ".loop2%=: \n"
          "so.a.mul.fp  u30, u1, u10, p0 \n" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%= \n"
    
        // KERNEL ADD

        "ss.sta.ld.w.v  u1, %[a] \n"
        "ss.end         u1, zero,%[sn], %[one] \n" //z[i]

        "ss.sta.ld.w.v  u2, %[b] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop3%=: \n"
          "so.a.add.fp  u30, u1 ,u2 ,p0 \n" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%= \n"
        
        // KERNEL TRIAD

        "ss.sta.ld.w.v  u1, %[b] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.ld.w.v  u2, %[c] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[a] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \n"

        ".loop4%=: \n"
          "so.a.mul.fp  u20, u2, u10 ,p0 \n" // tmp = scalar * c[]
          "so.a.add.fp  u30, u1, u20,p0 \n" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%= \n"
        
        :: [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
    asm volatile(
        // KERNEL COPY

        "ss.sta.ld.w.v  u1, %[a] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]
 
        "ss.sta.st.w.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop1%=: \n"
          "so.v.mv  u30, u1 ,p0 \n" // c[] = a[]
        "so.b.nc	u1, .loop1%= \n"
        
        // KERNEL SCALE

        "ss.sta.ld.w.v  u1, %[c] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[b] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \n"

        ".loop2%=: \n"
          "so.a.mul.sg  u30, u1, u10, p0 \n" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%= \n"
    
        // KERNEL ADD

        "ss.sta.ld.w.v  u1, %[a] \n"
        "ss.end         u1, zero,%[sn], %[one] \n" //z[i]

        "ss.sta.ld.w.v  u2, %[b] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop3%=: \n"
          "so.a.add.sg  u30, u1 ,u2 ,p0 \n" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%= \n"
        
        // KERNEL TRIAD

        "ss.sta.ld.w.v  u1, %[b] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.ld.w.v  u2, %[c] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.w.v  u30, %[a] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \n"

        ".loop4%=: \n"
          "so.a.mul.sg  u20, u2, u10, p0 \n" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0 \n" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%= \n"
        
        :: [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
    asm volatile(
        // KERNEL COPY

        "ss.sta.ld.h.v  u1, %[a] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.h.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop1%=: \n"
          "so.v.mv  u30, u1 ,p0 \n" // c[] = a[]
        "so.b.nc	u1, .loop1%= \n"
        
        // KERNEL SCALE

        "ss.sta.ld.h.v  u1, %[c] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.h.v  u30, %[b] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.h    u10, %[sc], p0 \n"

        ".loop2%=: \n"
          "so.a.mul.sg  u30, u1, u10, p0 \n" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%= \n"
    
        // KERNEL ADD

        "ss.sta.ld.h.v  u1, %[a] \n"
        "ss.end         u1, zero,%[sn], %[one] \n" //z[i]

        "ss.sta.ld.h.v  u2, %[b] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.h.v  u30, %[c] \n"
        "ss.end         u30, zero,%[sn], %[one] \n" //x[i]

        ".loop3%=: \n"
          "so.a.add.sg  u30, u1 ,u2 ,p0 \n" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%= \n"
        
        // KERNEL TRIAD

        "ss.sta.ld.h.v  u1, %[b] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.ld.h.v  u2, %[c] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.h.v  u30, %[a] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.h    u10, %[sc], p0 \n"

        ".loop4%=: \n"
          "so.a.mul.sg  u20, u2, u10, p0 \n" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0 \n" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%= \n"
        
        :: [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile("rdinstret %[s] \n":[s] "=&r"(start));
    asm volatile(
        // KERNEL COPY

        "ss.sta.ld.b.v  u1, %[a] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.b.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop1%=: \n"
          "so.v.mv  u30, u1 ,p0 \n" // c[] = a[]
        "so.b.nc	u1, .loop1%= \n"
        
        // KERNEL SCALE

        "ss.sta.ld.b.v  u1, %[c] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.b.v  u30, %[b] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.b    u10, %[sc], p0 \n"

        ".loop2%=: \n"
          "so.a.mul.sg  u30, u1, u10, p0 \n" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%= \n"
    
        // KERNEL ADD

        "ss.sta.ld.b.v  u1, %[a] \n"
        "ss.end         u1, zero,%[sn], %[one] \n" //z[i]

        "ss.sta.ld.b.v  u2, %[b] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.b.v  u30, %[c] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        ".loop3%=: \n"
          "so.a.add.sg  u30, u1 ,u2 ,p0 \n" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%= \n"
        
        // KERNEL TRIAD

        "ss.sta.ld.b.v  u1, %[b] \n"
        "ss.end         u1, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.ld.b.v  u2, %[c] \n"
        "ss.end         u2, zero, %[sn], %[one] \n" //z[i]

        "ss.sta.st.b.v  u30, %[a] \n"
        "ss.end         u30, zero, %[sn], %[one] \n" //x[i]

        "so.v.dp.b    u10, %[sc], p0 \n"

        ".loop4%=: \n"
          "so.a.mul.sg  u20, u2, u10, p0 \n" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0 \n" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%= \n"
        
        :: [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    asm volatile("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile ("rdinstret %[s] \n":[s] "=&r"(start));
    int j;
    for (j=0; j<sizeN; j++)
        c[j] = a[j];
    
    for (j=0; j<sizeN; j++)
        b[j] = scalar*c[j];
    
    for (j=0; j<sizeN; j++)
        c[j] = a[j]+b[j];

    for (j=0; j<sizeN; j++)
        a[j] = b[j]+scalar*c[j];

    asm volatile ("rdinstret %[e] \n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE
