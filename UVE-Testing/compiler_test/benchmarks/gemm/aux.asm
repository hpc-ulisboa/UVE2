%[beta] float %4
%[alpha] float %3
%[C] ptr %5
%[sizeI]   %12 = zext i32 %0 to i64
%[sizeJ]   %13 = zext i32 %1 to i64
zero i64 0
%[sizeJ]   %13 = zext i32 %1 to i64
%[one] i64 1
%[sizeJ]   %13 = zext i32 %1 to i64
%[sizeJ]   %13 = zext i32 %1 to i64
%[sizeK]   %14 = zext i32 %2 to i64
%[sizeJ]   %13 = zext i32 %1 to i64
%[sizeJ]   %13 = zext i32 %1 to i64
%[B] ptr %7
%[sizeJ]   %13 = zext i32 %1 to i64
%[A] ptr %6
%[sizeK]   %14 = zext i32 %2 to i64
Assembly Code:
ss.sta.st.w u1, %[C], %[sizeI], %[sizeJ]
ss.end u1, zero, %[sizeJ], %[one]

ss.sta.ld.w u3, %[C], %[sizeI], %[sizeJ]
ss.end u3, zero, %[sizeJ], %[one]
ss.cfg.vec u3

ss.sta.st.w u5, %[C], %[sizeI], %[sizeJ]
ss.app u5, zero, %[sizeK], zero
ss.end u5, zero, %[sizeJ], %[one]

ss.sta.ld.w u12, %[C], %[sizeI], %[sizeJ]
ss.app u12, zero, %[sizeK], zero
ss.end u12, zero, %[sizeJ], %[one]
ss.cfg.vec u12

ss.sta.ld.w u11, %[B], %[sizeI], zero
ss.app u11, zero, %[sizeK], %[sizeJ]
ss.end u11, zero, %[sizeJ], %[one]
ss.cfg.vec u11

ss.sta.ld.w u9, %[A], %[sizeI], %[sizeK]
ss.app u9, zero, %[sizeK], %[one]
ss.end u9, zero, %[sizeJ], zero
ss.cfg.vec u9

so.v.dp.w u4, %[beta], p0
so.v.dp.w u10, %[alpha], p0
.SLOOP_1:
.SLOOP_1_0:
so.a.mul.fp  u1, u3, u4, p0
so.b.ndc.1 u1, .SLOOP_1_0
.SLOOP_1_1:
so.a.mul.fp  u8, u9, u10, p0
so.a.mul.fp  u7, u8, u11, p0
so.a.add.fp  u5, u7, u12, p0
so.b.ndc.2 u5, .SLOOP_1_1
so.b.nc u1, .SLOOP_1