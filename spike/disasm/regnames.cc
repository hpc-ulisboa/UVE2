// See LICENSE for license details.

#include "disasm.h"

const char* xpr_name[] = {
  "zero", "ra", "sp",  "gp",  "tp", "t0",  "t1",  "t2",
  "s0",   "s1", "a0",  "a1",  "a2", "a3",  "a4",  "a5",
  "a6",   "a7", "s2",  "s3",  "s4", "s5",  "s6",  "s7",
  "s8",   "s9", "s10", "s11", "t3", "t4",  "t5",  "t6"
};

const char* fpr_name[] = {
  "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft7",
  "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
  "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
  "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"
};

const char* vr_name[] = {
  "v0",  "v1",  "v2",  "v3",  "v4",  "v5",  "v6",  "v7",
  "v8",  "v9",  "v10", "v11", "v12", "v13", "v14", "v15",
  "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
  "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
};

const char* ur_name[] = {
  "u0",  "u1",  "u2",  "u3",  "u4",  "u5",  "u6",  "u7",
  "u8",  "u9",  "u10", "u11", "u12", "u13", "u14", "u15",
  "u16", "u17", "u18", "u19", "u20", "u21", "u22", "u23",
  "u24", "u25", "u26", "u27", "u28", "u29", "u30", "u31"
};

const char* pr_name[] = {
  "p0",  "p1",  "p2",  "p3",  "p4",  "p5",  "p6",  "p7",
  "p8",  "p9",  "p10", "p11", "p12", "p13", "p14", "p15"
};

const char* csr_name(int which) {
  switch (which) {
    #define DECLARE_CSR(name, number)  case number: return #name;
    #include "encoding.h"
    #undef DECLARE_CSR
  }
  return "unknown-csr";
}
