#include "Functions.h"

#ifdef RUN_UVE
#ifdef B_TYPE
void core(DataType src[SIZE]) {
    printf("Source array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%i ", src[i]);

    printf("\n\nConvertion from byte to byte (unsigned)\n");
    unsigned char dest1[SIZE];
    initZero(dest1, SIZE);
	asm volatile(
		"ss.sta.ld.b.v  u1, %[src] \n"
		"ss.end		    u1, zero, %[size], %[stride] \n"

        "ss.sta.st.b.v  u2, %[dest] \n"
		"ss.end		    u2, zero, %[size], %[stride] \n"

		".uve_loop%= : \n"
			"so.v.cv.us.b u2, u1 \n"
		"so.b.nc u2, .uve_loop%= \n"
		:
		: [dest] "r" (dest1),
		  [src] "r" (src),
	      [size] "r" (SIZE),
		  [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest1[i]);

    printf("\n\nConvertion from byte to byte (signed)\n");
    signed char dest2[SIZE];
    initZero(dest2, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.b.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.b u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest2),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest2[i]);

    printf("\n\nConvertion from byte to half-word (unsigned)\n");
    short dest3[SIZE];
    initZero(dest3, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest3),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest3[i]);

    printf("\n\nConvertion from byte to half-word (signed)\n");
    short dest4[SIZE];
    initZero(dest4, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest4),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest4[i]);

    printf("\n\nConvertion from byte to word (unsigned)\n");
    int dest5[SIZE];
    initZero(dest5, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest5),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest5[i]);

    printf("\n\nConvertion from byte to word (signed)\n");
    int dest6[SIZE];
    initZero(dest6, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest6),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest6[i]);

    printf("\n\nConvertion from byte to double-word (unsigned)\n");
    long dest7[SIZE];
    initZero(dest7, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest7),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%ld ", dest7[i]);

    printf("\n\nConvertion from byte to double-word (signed)\n");
    long dest8[SIZE];
    initZero(dest8, SIZE);
    asm volatile(
        "ss.sta.ld.b.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest8),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%ld ", dest8[i]);
}
#endif // B_TYPE
#ifdef H_TYPE
void core(DataType src[SIZE]){
    printf("Source array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%i ", src[i]);

    printf("\n\nConvertion from half-word to byte (unsigned)\n");
    unsigned char dest1[SIZE];
    initZero(dest1, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"

        "ss.sta.st.b.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.b u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest1),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest1[i]);

    printf("\n\nConvertion from half-word to byte (signed)\n");
    signed char dest2[SIZE];
    initZero(dest2, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.b.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.b u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest2),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest2[i]);

    printf("\n\nConvertion from half-word to half-word (unsigned)\n");
    short dest3[SIZE];
    initZero(dest3, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest3),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest3[i]);

    printf("\n\nConvertion from half-word to half-word (signed)\n");
    short dest4[SIZE];
    initZero(dest4, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest4),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest4[i]);

    printf("\n\nConvertion from half-word to word (unsigned)\n");
    int dest5[SIZE];
    initZero(dest5, SIZE);  
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest5),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest5[i]);

    printf("\n\nConvertion from half-word to word (signed)\n");
    int dest6[SIZE];
    initZero(dest6, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest6),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest6[i]);

    printf("\n\nConvertion from half-word to double-word (unsigned)\n");
    long dest7[SIZE];
    initZero(dest7, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest7),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest7[i]);

    printf("\n\nConvertion from half-word to double-word (signed)\n");
    long dest8[SIZE];
    initZero(dest8, SIZE);
    asm volatile(
        "ss.sta.ld.h.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest8),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest8[i]);
}
#endif // H_TYPE
#ifdef I_TYPE
void core(DataType src[SIZE]){
    printf("Source array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%i ", src[i]);

    printf("\n\nConvertion from word to byte (unsigned)\n");
    unsigned char dest1[SIZE];
    initZero(dest1, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"

        "ss.sta.st.b.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.b u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest1),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest1[i]);

    printf("\n\nConvertion from word to byte (signed)\n");
    signed char dest2[SIZE];
    initZero(dest2, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.b.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.b u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest2),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest2[i]);

    printf("\n\nConvertion from word to half-word (unsigned)\n");
    short dest3[SIZE];
    initZero(dest3, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest3),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest3[i]);

    printf("\n\nConvertion from word to half-word (signed)\n");
    short dest4[SIZE];
    initZero(dest4, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.h.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.h u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest4),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", (int)dest4[i]);
    
    printf("\n\nConvertion from word to word (unsigned)\n");
    int dest5[SIZE];
    initZero(dest5, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest5),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));
    
    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest5[i]);

    printf("\n\nConvertion from word to word (signed)\n");
    int dest6[SIZE];
    initZero(dest6, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest6),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%d ", dest6[i]);

    printf("\n\nConvertion from word to double-word (unsigned)\n");
    long dest7[SIZE];
    initZero(dest7, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.us.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest7),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%ld ", dest7[i]);

    printf("\n\nConvertion from word to double-word (signed)\n");
    long dest8[SIZE];
    initZero(dest8, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.sg.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest8),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%ld ", dest8[i]);

}
#endif // I_TYPE
#ifdef F_TYPE
void core(DataType src[SIZE]){
    printf("Source array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6f ", src[i]);

    printf("\n\nConvertion from float to float\n");
    float dest1[SIZE];
    initZero(dest1, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"

        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.fp.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest1),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6f ", dest1[i]);

    printf("\n\nConvertion from float to double\n");
    double dest10[SIZE];
    initZero(dest10, SIZE);
    asm volatile(
        "ss.sta.ld.w.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.fp.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest10),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6lf ", dest10[i]);
}
#endif // F_TYPE
#ifdef D_TYPE
void core(DataType src[SIZE]){
    printf("Source array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6lf ", src[i]);

    printf("\n\nConvertion from double to float\n");
    float dest1[SIZE];
    initZero(dest1, SIZE);
    asm volatile(
        "ss.sta.ld.d.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"

        "ss.sta.st.w.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.fp.w u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest1),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6f ", dest1[i]);

    printf("\n\nConvertion from double to double\n");
    double dest10[SIZE];
    initZero(dest10, SIZE);
    asm volatile(
        "ss.sta.ld.d.v  u1, %[src] \n"
        "ss.end		    u1, zero, %[size], %[stride] \n"
        
        "ss.sta.st.d.v  u2, %[dest] \n"
        "ss.end		    u2, zero, %[size], %[stride] \n"

        ".uve_loop%= : \n"
            "so.v.cv.fp.d u2, u1 \n"
        "so.b.nc u2, .uve_loop%= \n"
        :
        : [dest] "r" (dest10),
          [src] "r" (src),
          [size] "r" (SIZE),
          [stride] "r" (1));

    printf("\nDestination array:\n");
    for (int i = 0; i < SIZE; i++)
        printf( "%3.6lf ", dest10[i]);
    printf("\n");
}
#endif // D_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType src[SIZE]){};
#endif // RUN_SIMPLE