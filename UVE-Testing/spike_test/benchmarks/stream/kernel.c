#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE
#ifdef D_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.sta.ld.d.v  u1, %[a] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.d.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.sta.ld.d.v  u1, %[c] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.d.v  u30, %[b] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.d    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.fp  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.sta.ld.d.v  u1, %[a] \t\n"
        "ss.end         u1, zero,%[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.d.v  u2, %[b] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.d.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop3%=: \t\n"
          "so.a.add.fp  u30, u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.sta.ld.d.v  u1, %[b] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.d.v  u2, %[c] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.d.v  u30, %[a] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.d    u10, %[sc], p0 \t\n"

        ".loop4%=: \t\n"
          "so.a.mul.fp  u20, u2, u10, p0  \n\t" // tmp = scalar * c[]
          "so.a.add.fp  u30, u1, u20, p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%=  \n\t"

        "rdinstret %[end] \t\n"
        
        : [start] "=&r"(start), [end] "=&r"(end)
        : [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    printf("%ld\n%ld\n", start, end);
}
#endif // D_TYPE
#ifdef F_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.sta.ld.w.v  u1, %[a] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]
         
        "ss.sta.st.w.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.sta.ld.w.v  u1, %[c] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[b] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.fp  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.sta.ld.w.v  u1, %[a] \t\n"
        "ss.end         u1, zero,%[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.w.v  u2, %[b] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop3%=: \t\n"
          "so.a.add.fp  u30, u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.sta.ld.w.v  u1, %[b] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.w.v  u2, %[c] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[a] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \t\n"

        ".loop4%=: \t\n"
          "so.a.mul.fp  u20, u2, u10 ,p0  \n\t" // tmp = scalar * c[]
          "so.a.add.fp  u30, u1, u20,p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%=  \n\t"

        "rdinstret %[end] \t\n"
        
        : [start] "=&r"(start), [end] "=&r"(end)
        : [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    printf("%ld\n%ld\n", start, end);
}
#endif // F_TYPE
#ifdef I_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.sta.ld.w.v  u1, %[a] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]
 
        "ss.sta.st.w.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.sta.ld.w.v  u1, %[c] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[b] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.sg  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.sta.ld.w.v  u1, %[a] \t\n"
        "ss.end         u1, zero,%[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.w.v  u2, %[b] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop3%=: \t\n"
          "so.a.add.sg  u30, u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.sta.ld.w.v  u1, %[b] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.w.v  u2, %[c] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.w.v  u30, %[a] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.w    u10, %[sc], p0 \t\n"

        ".loop4%=: \t\n"
          "so.a.mul.sg  u20, u2, u10, p0  \n\t" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%=  \n\t"

        "rdinstret %[end] \t\n"
        
        : [start] "=&r"(start), [end] "=&r"(end)
        : [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    printf("%ld\n%ld\n", start, end);
}
#endif // I_TYPE
#ifdef H_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.sta.ld.h.v  u1, %[a] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.h.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.sta.ld.h.v  u1, %[c] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.h.v  u30, %[b] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.h    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.sg  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.sta.ld.h.v  u1, %[a] \t\n"
        "ss.end         u1, zero,%[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.h.v  u2, %[b] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.h.v  u30, %[c] \t\n"
        "ss.end         u30, zero,%[sn], %[one] \t\n" //x[i]

        ".loop3%=: \t\n"
          "so.a.add.sg  u30, u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.sta.ld.h.v  u1, %[b] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.h.v  u2, %[c] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.h.v  u30, %[a] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.h    u10, %[sc], p0 \t\n"

        ".loop4%=: \t\n"
          "so.a.mul.sg  u20, u2, u10, p0  \n\t" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%=  \n\t"

        "rdinstret %[end] \t\n"
        
        : [start] "=&r"(start), [end] "=&r"(end)
        : [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    printf("%ld\n%ld\n", start, end);
}
#endif // H_TYPE
#ifdef B_TYPE
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.sta.ld.b.v  u1, %[a] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.b.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.sta.ld.b.v  u1, %[c] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.b.v  u30, %[b] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.b    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.sg  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.sta.ld.b.v  u1, %[a] \t\n"
        "ss.end         u1, zero,%[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.b.v  u2, %[b] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.b.v  u30, %[c] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        ".loop3%=: \t\n"
          "so.a.add.sg  u30, u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.sta.ld.b.v  u1, %[b] \t\n"
        "ss.end         u1, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.ld.b.v  u2, %[c] \t\n"
        "ss.end         u2, zero, %[sn], %[one] \t\n" //z[i]

        "ss.sta.st.b.v  u30, %[a] \t\n"
        "ss.end         u30, zero, %[sn], %[one] \t\n" //x[i]

        "so.v.dp.b    u10, %[sc], p0 \t\n"

        ".loop4%=: \t\n"
          "so.a.mul.sg  u20, u2, u10, p0  \n\t" // tmp = scalar * c[]
          "so.a.add.sg  u30, u1, u20, p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, .loop4%=  \n\t"

        "rdinstret %[end] \t\n"
        
        : [start] "=&r"(start), [end] "=&r"(end)
        : [a] "r" (a),
          [b] "r" (b),
          [c] "r" (c),
          [sc] "r" (scalar),
          [sn] "r" (sizeN),
          [one] "r" (1));

    printf("%ld\n%ld\n", start, end);
}
#endif // B_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE

void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile ("rdinstret %[s] \t\n":[s] "=&r"(start));
    int j;
    for (j=0; j<sizeN; j++)
        c[j] = a[j];
    
    for (j=0; j<sizeN; j++)
        b[j] = scalar*c[j];
    
    for (j=0; j<sizeN; j++)
        c[j] = a[j]+b[j];

    for (j=0; j<sizeN; j++)
        a[j] = b[j]+scalar*c[j];

    asm volatile ("rdinstret %[e] \t\n":[e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}

#endif // RUN_SIMPLE
