#include "Functions.h"

#ifdef RUN_UVE

static inline void __kernel_copy(void * c, void * a, uint64_t N){
    asm volatile(                        /*offset, size, stride*/
        "ss.ld.d  u1,  %[a],  %[sn],  %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.st.d  u30, %[c],  %[sn],  %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"

        "1: \t\n"
          "so.v.mv  u30,u1 ,p0  \n\t" // c[] = a[]
        "so.b.nc	u1, 1b  \n\t"
        ::  [a]"r"(a), [c]"r"(c),
            [sn]"r"(N), [one]"r"(1)
        );
}

static inline void __kernel_scale(void * b, void * c, uint64_t N, float scalar){
    asm volatile(                        /*offset, size, stride*/
        "ss.ld.d  u1,  %[c],  %[sn],  %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.st.d  u30, %[b],  %[sn],  %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"
        "so.v.dp.d    u10, %[sc], p0 \t\n"

        "1: \t\n"
          "so.a.mul.fp  u30, u1,u10,p0  \n\t" // b[] = scalar * c[]
        "so.b.nc	u1, 1b  \n\t"
        ::  [c]"r"(c), [b]"r"(b),
            [sc]"r"(scalar),
            [sn]"r"(N), [one]"r"(1)
        );
}


static inline void __kernel_add(void * c, void * a, void * b, uint64_t N){
    asm volatile(                        /*offset, size, stride*/
        "ss.ld.d  u1,  %[a], %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d  u2,  %[b], %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u2 \t\n"
        "ss.st.d  u30, %[c], %[sn], %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"

        "1: \t\n"
          "so.a.add.fp  u30,u1 ,u2 ,p0  \n\t" // c[] = a[] + c[]
        "so.b.nc	u1, 1b  \n\t"
        ::  [c]"r"(c), [a]"r"(a), [b]"r"(b),
            [sn]"r"(N), [one]"r"(1)
        );
}

static inline void __kernel_triad(void * a, void * b, void * c, uint64_t N, float scalar){
    asm volatile(                        /*offset, size, stride*/
        "ss.ld.d  u1,  %[b],  %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u1 \t\n"
        "ss.ld.d  u2,  %[c],  %[sn], %[one] \t\n" //z[i]
        "ss.cfg.vec u2 \t\n"
        "ss.st.d  u30, %[a],  %[sn], %[one] \t\n" //x[i]
        "ss.cfg.vec u30 \t\n"
        "so.v.dp.d    u10, %[sc], p0 \t\n"

        "1: \t\n"
          "so.a.mul.fp  u20, u2, u10 ,p0  \n\t" // tmp = scalar * c[]
          "so.a.add.fp  u30, u1, u20,p0  \n\t" // a[] = tmp + b[]
        "so.b.nc	u1, 1b  \n\t"
        ::  [c]"r"(c), [a]"r"(a), [b]"r"(b),
            [sc]"r"(scalar),
            [sn]"r"(N), [one]"r"(1)
        );
}

void
core(void* __a, void* __b, void* __c, uint64_t sizeN, float scalar) {
    DataType *a = (DataType *)__a; /* N */
    DataType *b = (DataType *)__b; /* N */
    DataType *c = (DataType *)__c; /* N */

    __kernel_copy(c, a, sizeN);

    __kernel_scale(b, c, sizeN, scalar);

    __kernel_add(c, a, b, sizeN);

    __kernel_triad(a, b, c, sizeN, scalar);
}

#endif

#ifdef RUN_SIMPLE

void
core(void* _a, void* _b, void* _c, uint64_t sizeN, int ntimes, float scalar) {
    DataType *a = (DataType *)_a; /* N */
    DataType *b = (DataType *)_b; /* N */
    DataType *c = (DataType *)_c; /* N */
    int j;
    for (j=0; j<sizeN; j++)
        c[j] = a[j];
    
    for (j=0; j<sizeN; j++)
        b[j] = scalar*c[j];
    
    for (j=0; j<sizeN; j++)
        c[j] = a[j]+b[j];

    for (j=0; j<sizeN; j++)
        a[j] = b[j]+scalar*c[j];
}

#endif
