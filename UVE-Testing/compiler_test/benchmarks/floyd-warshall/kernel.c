#include "Functions.h"
#define v_len 16

#ifdef RUN_UVE
void core(DataType *path) {
    /*asm volatile(
         //offset, size, stride
        // path_ij stream load
        "ss.sta.ld.d           u1, %[path], %[sn], %[one] \t\n" // D1: vector - linear access size SN
        "ss.cfg.vec            u1 \t\n"
        "ss.app                u1, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N (for i)
        "ss.cfg.vec            u1 \t\n"
        "ss.end                u1, zero, %[sn], zero \t\n"      // repeat: for each 'k'

        // path_ik stream load
        "ss.sta.ld.d           u2, %[path], %[sn], zero \t\n" // D1: vector element - replicate (for j)
        "ss.cfg.vec            u2 \t\n"
        "ss.app                u2, zero, %[sn], %[sn] \t\n"   // D2: slide verticaly stride N access size N (for i)
        "ss.cfg.vec            u2 \t\n"
        "ss.end                u2, zero, %[sn], %[one] \t\n"  // D3: slide horizontaly N times (for k)

        // path_kj stream load
        "ss.sta.ld.d           u3, %[path], %[sn], %[one] \t\n" // D1: vector - linear access size V_len
        "ss.cfg.vec            u3 \t\n"
        "ss.app                u3, zero, %[sn], zero \t\n"  // repeat: for each 'i'
        "ss.cfg.vec            u3 \t\n"
        "ss.end                u3, zero, %[sn], %[sn] \t\n" // D3: slide verticaly stride N access size N (for k)

        // path_ij stream store
        "ss.sta.st.d           u4, %[path], %[sn], %[one] \t\n" // D1: vector - linear access size V_len
        "ss.cfg.vec            u4 \t\n"
        "ss.app                u4, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N ('N' times)
        "ss.cfg.vec            u4 \t\n"
        "ss.end                u4, zero, %[sn], zero \t\n"      // repeat: for each 'k'

        :
        : [path] "r"(path), [sn] "r"(SIZE),
          [one] "r"(1), [vl] "r"(v_len), [nv] "r"(SIZE / v_len)); */

    asm volatile(                        /*offset, size, stride*/
            // path_ij stream load
            "ss.sta.ld.d           u1, %[src1], %[vl], %[one] \t\n"    // D1: vector - linear access size V_len
            "ss.app                u1, zero, %[nv], %[vl] \t\n"     // D2: slide verticaly stride N ('' times)
            "ss.app                u1, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N ('' times)
            "ss.end                u1, zero, %[sn], zero \t\n"      // repeat: for each 'k'

            // path_ik stream load 
            "ss.sta.ld.d           u2, %[src1], %[vl], zero \t\n"      // D1: vector element - replicate 
            "ss.app                u2, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N access size N
            "ss.end                u2, zero, %[sn], %[one] \t\n"    // D3: slide horizontaly N times (for k)

            // path_kj stream load 
            "ss.sta.ld.d           u3, %[src1], %[vl], %[one] \t\n"    // D1: vector - linear access size V_len
            "ss.app                u3, zero, %[nv], %[vl] \t\n"     // D2: slide verticaly stride N ('' times)
            "ss.cfg.vec            u3 \t\n"
            "ss.app                u3, zero, %[sn], zero \t\n"   // repeat: for each 'i'
            "ss.end                u3, zero, %[sn], %[sn] \t\n"     // D3: slide verticaly stride N access size N (for k)

            // path_ij stream store
            "ss.sta.st.d           u4, %[src1], %[vl], %[one] \t\n"    // D1: vector - linear access size V_len
            "ss.app                u4, zero, %[nv], %[vl] \t\n"     // D2: slide verticaly stride N ('' times)
            "ss.app                u4, zero, %[sn], %[sn] \t\n"     // D2: slide verticaly stride N ('' times)
            "ss.end                u4, zero, %[sn], zero \t\n"      // repeat: for each 'k'

            :
            : [src1] "r"(path), [sn] "r"(SIZE),
            [one] "r" (1), [vl] "r" (v_len), [nv] "r" (/* SIZE/v_len */ 1) );

    asm volatile(
        ".fLoop1%=: \t\n"

        "so.v.mv u5, u2, p0 \n\t" // load path_ik

        ".jLoop1%=: \t\n"
            "so.v.mv u6, u1, p0 \n\t"         // load path_ij
            "so.a.add.sg u7, u5, u3, p0 \n\t" // load and add path_ik + path_kj
            "so.a.min.sg u4, u6, u7, p0 \n\t" // min path_ij and path_ik + path_kj
        "so.b.ndc.2 u3, .jLoop1%= \n\t"

        "so.b.nc	u1, .fLoop1%= \n\t" :::);
}
#endif // RUN_UVE

#ifdef RUN_SIMPLE
void core(DataType *path) {
    for (int k = 0; k < SIZE; k++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++){
                int sum = path[i * SIZE + k] + path[k * SIZE + j];
                printf("ADD element1: %d element2: %d result: %d\n", path[i * SIZE + k], path[k * SIZE + j], sum);
                int min = path[i * SIZE + j] < sum ? path[i * SIZE + j] : sum;
                printf("MIN element1: %d element2: %d result: %d\n", path[i * SIZE + j], sum, min);
                
                path[i * SIZE + j] = min;
            }
}
#endif // RUN_SIMPLE