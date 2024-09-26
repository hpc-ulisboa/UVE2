#include "Functions.h"

#ifdef RUN_UVE
void core(DataType *dest, DataType *src, int *ind) {
    asm volatile(
        "ss.sta.ld.w.inds  u1, %[ind] \n"
        "ss.end            u1, zero, %[size], %[one] \n"

        "ss.sta.ld.d.v        u2, %[src] \n"
        //"ss.app               u2, zero, %[size], zero \n"
        //"ss.app.ind.ofs.sub.1 u2, u1 \n"
        //"ss.end		        u2, %[size], zero, zero \n"
        "ss.app                u2, %[sizeS], %[size], zero \n"
        "ss.app.ind.ofs.sg.sub u2, u1 \n"

        "ss.sta.st.d.v  u3, %[dest] \n"
        "ss.end         u3, zero, %[size], %[one] \n"

        ::[dest] "r"(dest),
        [src] "r"(src),
        [ind] "r"(ind),
        [size] "r"(SIZE),
        [sizeS] "r"(SIZE*SIZE),
        [one] "r"(1));

    asm volatile(
        ".uve_loop%= : \n"
        	"so.v.mv u3, u2, p0 \n\t"
        "so.b.nc u3, .uve_loop%= \n\t" :::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *dest, DataType *src, int *ind) {
	for (int i = 0; i < SIZE; ++i)
		dest[i] = src[SIZE*SIZE-ind[i]];
}
#endif // RUN_SIMPLE