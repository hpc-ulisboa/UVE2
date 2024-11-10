#include "Functions.h"

#ifdef RUN_UVE
#ifdef F_TYPE
/*void core(DataType x[SIZE], DataType y[SIZE], DataType A){
  asm volatile(
      "ss.st.w u1, %[input1], %[input4], %[input3] \n"
      "ss.cfg.vec u1\n"

      "ss.ld.w u2, %[input5], %[input4], %[input3] \n"
      "ss.cfg.vec u2\n"

      "ss.ld.w u3, %[input1], %[input4], %[input3] \n"
      "ss.cfg.vec u3\n"

      "so.v.dp.w u4, %[input0], p0 \n"

      ".Loop_0%=:\n"
        "so.a.mul.fp u5, u2, u4, p0 \n"
        "so.a.add.fp u1, u3, u5, p0 \n"
        "so.b.nc u1, .Loop_0%=\n"
      :
      : [input0] "r"(A),
        [input1] "r"(y),
        [input2] "r"(0),
        [input3] "r"(1),
        [input4] "r"(1),
        [input5] "r"(x));
}*/

void core(){
  // test all so.p.cv instructions

  printf("Testing so.p.cv instructions\n");

  printf("---------------------\nso.p.cv.b.d / so.p.cv.d.b\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.b.d p2, p1\n"
    "so.p.cv.d.b p4, p2\n"
  :::);

  printf("---------------------\nso.p.cv.b.h / so.p.cv.h.b\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.b.h p2, p1\n"
    "so.p.cv.h.b p4, p2\n"
  :::);

  printf("---------------------\nso.p.cv.b.w / so.p.cv.w.b\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.b.w p2, p1\n"
    "so.p.cv.w.b p4, p2\n"
  :::);

  printf("---------------------\nso.p.cv.h.d / so.p.cv.d.h\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.h.d p2, p1\n"
    "so.p.cv.d.h p4, p2\n"
  :::);

  printf("---------------------\nso.p.cv.h.w / so.p.cv.w.h\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.h.w p2, p1\n"
    "so.p.cv.w.h p4, p2\n"
  :::);

  printf("---------------------\nso.p.cv.w.d / so.p.cv.d.w\n");
  asm volatile(
    "so.p.one p1, p0 \n"
    "so.p.cv.w.d p2, p1\n"
    "so.p.cv.d.w p4, p2\n"
    "so.p.cv.w.d p5, p4\n"
  :::);
    
}
#endif // F_TYPE
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(){};
#endif // RUN_SIMPLE