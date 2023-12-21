#include "Functions.h"

long int start = 0, end = 0;

#ifdef RUN_UVE

void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar) {
    asm volatile(

        "rdinstret %[start] \t\n" // start counting instructions after values have been loaded into registers

        // KERNEL COPY

        "ss.ld.d  u1,  %[a],  %[sn],  %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
         
        "ss.st.d  u30, %[c],  %[sn],  %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"

        ".loop1%=: \t\n"
          "so.v.mv  u30, u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, .loop1%=  \n\t"
        
        // KERNEL SCALE

        "ss.ld.d  u1,  %[c],  %[sn],  %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.st.d  u30, %[b],  %[sn],  %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"
        "so.v.dp.d    u10, %[sc], p0 \t\n"

        ".loop2%=: \t\n"
          "so.a.mul.fp  u30, u1, u10, p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, .loop2%=  \n\t"
    
        // KERNEL ADD

        "ss.ld.d  u1,  %[a], %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d  u2,  %[b], %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u2 \t\n"
        "ss.st.d  u30, %[c], %[sn], %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"

        ".loop3%=: \t\n"
          "so.a.add.fp  u30,u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, .loop3%=  \n\t"
        
        // KERNEL TRIAD

        "ss.ld.d  u1,  %[b],  %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"

        "ss.ld.d  u2,  %[c],  %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u2 \t\n"

        "ss.st.d  u30, %[a],  %[sn], %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"

        "so.v.dp.d    u10, %[sc], p0 \t\n"

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

#endif

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

#endif

#ifdef RUN_BLANK
void core(DataType *a, DataType *b, DataType *c, uint64_t sizeN, DataType scalar){
}
#endif // RUN_BLANK