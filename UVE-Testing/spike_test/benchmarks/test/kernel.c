#include "Functions.h"

#ifdef RUN_UVE
#ifdef F_TYPE
void core(DataType x[SIZE], DataType y[SIZE], DataType A){
  asm volatile(
      "ss.st.w u1, %[input1], %[input4], %[input3]\n"
      "ss.cfg.vec u1\n"

      "ss.ld.w u2, %[input5], %[input4], %[input3]\n"
      "ss.cfg.vec u2\n"

      "ss.ld.w u3, %[input1], %[input4], %[input3]\n"
      "ss.cfg.vec u3\n"

      "so.v.dp.w u4, %[input0], p0\n"

      ".Loop_0%=:\n"
        "so.a.mul.fp u5, u2, u4, p0\n"
        "so.a.add.fp u1, u3, u5, p0\n"
        "so.b.nc u1, .Loop_0%=\n"
      :
      : [input0] "r"(A),
        [input1] "r"(y),
        [input2] "r"(0),
        [input3] "r"(1),
        [input4] "r"(1),
        [input5] "r"(x));
}
#endif // F_TYPE
#endif // RUN_UVE