#include "Functions.h"

#ifdef RUN_UVE
void core(DataType *dest, DataType *src, int *ind) {
    asm volatile(
        "ss.ld.w  u1,  %[ind], %[size], %[one] \t\n"

        "ss.sta.ld.d  u2, %[src], %[size], zero \t\n"
        "ss.app.indl.ofs.sub u2, u1 \t\n"
        "ss.end		  u2, %[sizeS], zero, zero \t\n"
        //"ss.cfg.vec u2 \t\n"

        "ss.st.d  u3, %[dest], %[size], %[one] \t\n"
        //"ss.cfg.vec u3 \t\n"

        ::[dest] "r"(dest),
        [src] "r"(src),
        [ind] "r"(ind),
        [size] "r"(SIZE),
        [sizeS] "r"(SIZE*SIZE),
        [one] "r"(1));

    asm volatile(
        ".uve_loop%= : \t\n"
        	"so.v.mv u3, u2, p0 \n\t"
        "so.b.nc u3, .uve_loop%= \n\t" :::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *dest, DataType *src, int *ind) {
	for (int i = 0; i < SIZE; ++i)
		dest[i] = src[SIZE-ind[i]];
}
#endif // RUN_SIMPLE