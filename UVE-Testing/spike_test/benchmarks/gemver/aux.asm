/* //kernel_1
Operands:
$0 ptr %3
$1   %14 = zext i32 %0 to i64
$2   %14 = zext i32 %0 to i64
$3 i64 0
$4   %14 = zext i32 %0 to i64
%[one] i64 1
$6 ptr %7
$7 ptr %6
$8 ptr %5
$9 ptr %4
$10   %14 = zext i32 %0 to i64

Assembly Code:
ss.sta.st.w u1, %[A], %[sizeN], %[sizeN]
ss.end u1, zero, %[sizeN], %[one]

ss.sta.ld.w u10, %[u2], %[sizeN], zero
ss.end u10, zero, %[sizeN], %[one]
ss.cfg.vec u10

ss.sta.ld.w u9, %[v2], %[sizeN], %[one]
ss.end u9, zero, %[sizeN], zero
ss.cfg.vec u9

ss.sta.ld.w u7, %[v1], %[sizeN], zero
ss.end u7, zero, %[sizeN], %[one]
ss.cfg.vec u7

ss.sta.ld.w u6, %[u1], %[sizeN], %[one]
ss.end u6, zero, %[sizeN], zero
ss.cfg.vec u6

ss.sta.ld.w u4, %[A], %[sizeN], %[sizeN]
ss.end u4, zero, %[sizeN], %[one]
ss.cfg.vec u4

.SLOOP_1:
so.a.mul.fp  u5, u6, u7, p0
so.a.add.fp  u3, u4, u5, p0
so.a.mul.fp  u8, u9, u10, p0
so.a.add.fp  u1, u3, u8, p0
so.b.nc u1, .SLOOP_1

//kernel_2
Operands:
$0 float %2
$1 ptr %9
$2   %14 = zext i32 %0 to i64
$3 i64 0
$4   %14 = zext i32 %0 to i64
%[one] i64 1
$6 ptr %3
$7   %14 = zext i32 %0 to i64
$8 ptr %8

Assembly Code:
ss.sta.ld.w u7, %[y], %[sizeN], zero
ss.end u7, zero, %[sizeN], %[one]
ss.cfg.vec u7

ss.sta.ld.w u5, %[A], %[sizeN], %[one]
ss.end u5, zero, %[sizeN], %[sizeN]
ss.cfg.vec u5

ss.st.w u1, %[x], %[sizeN], %[one]

ss.ld.w u9, %[x], %[sizeN], %[one]

so.v.dp.w u6, %[beta], p0

.SLOOP_1:
so.v.dp.w u2, zero, p0
.SLOOP_1_0:
so.a.mul.fp  u4, u5, u6, p0
so.a.mul.fp  u3, u4, u7, p0
so.a.add.fp  u2, u2, u3, p0
so.b.ndc.1 u5, .SLOOP_1_0
so.a.adde.fp  u3, u2, p0
so.a.add.fp  u1, u9, u3, p0
so.b.nc u1, .SLOOP_1

//kernel_3
Operands:
$0 ptr %8
$1   %14 = zext i32 %0 to i64
$2 i64 1
$3 ptr %10

Assembly Code:
ss.st.w u1, %[x], %[sizeN], %[one]

ss.ld.w u4, %[z], %[sizeN], %[one]
ss.cfg.vec u4

ss.ld.w u3, %[x], %[sizeN], %[one]
ss.cfg.vec u3

.SLOOP_1:
so.a.add.fp  u1, u3, u4, p0
so.b.nc u1, .SLOOP_1

//kernel_4

0 sizeN,
1 alpha,
2 beta,
3 **A,
4 *u1,
5 *v1,
6 *u2,
7 *v2,
8 *x,
9 *y,
10 *z,
11 *w
Operands:
$0 float %1
$1 ptr %8
$2   %14 = zext i32 %0 to i64
$3 i64 0
$4   %14 = zext i32 %0 to i64
%[one] i64 1
$6 ptr %3
$7   %14 = zext i32 %0 to i64
$8 ptr %11

Assembly Code:
ss.sta.ld.w u7, %[x], %[sizeN], zero
ss.end u7, zero, %[sizeN], %[one]
ss.cfg.vec u7

ss.sta.ld.w u5, %[A], %[sizeN], %[sizeN]
ss.end u5, zero, %[sizeN], %[one]
ss.cfg.vec u5

ss.st.w u1, %[w], %[sizeN], %[one]

ss.ld.w u9, %[w], %[sizeN], %[one]

so.v.dp.w u6, %[alpha], p0

.SLOOP_1:
so.v.dp.w u2, zero, p0
.SLOOP_1_0:
so.a.mul.fp  u4, u5, u6, p0
so.a.mul.fp  u3, u4, u7, p0
so.a.add.fp  u2, u2, u3, p0
so.b.ndc.1 u5, .SLOOP_1_0
so.a.adde.fp  u3, u2, p0
so.a.add.fp  u1, u9, u3, p0
so.b.nc u1, .SLOOP_1 */