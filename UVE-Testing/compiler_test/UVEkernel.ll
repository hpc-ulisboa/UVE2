; ModuleID = 'kernel.ll'
source_filename = "benchmarks/gesummv/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(i32 noundef signext %0, double noundef %1, double noundef %2, ptr nocapture noundef readonly %3, ptr nocapture noundef readonly %4, ptr nocapture noundef %5, ptr nocapture noundef readonly %6, ptr nocapture noundef %7) local_unnamed_addr #0 {
  %9 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !4
  store i64 %9, ptr @start, align 8, !tbaa !5
  %10 = icmp sgt i32 %0, 0
  br i1 %10, label %.loopexit, label %12

.loopexit:                                        ; preds = %8
  %11 = zext i32 %0 to i64
  call void asm sideeffect "ss.sta.st.d u1, $3\0Ass.end u1, $4, $5, $6\0Ass.sta.st.d u3, $7\0Ass.end u3, $4, $5, $6\0Ass.sta.ld.d.v u7, $8\0Ass.app u7, $4, $5, $9\0Ass.end u7, $4, $5, $6\0Ass.sta.ld.d.v u8, $10\0Ass.app u8, $4, $5, $4\0Ass.end u8, $4, $5, $6\0Ass.sta.ld.d u24, $3\0Ass.end u24, $4, $5, $6\0Ass.sta.st.d u4, $3\0Ass.end u4, $4, $5, $6\0Ass.sta.ld.d.v u13, $11\0Ass.app u13, $4, $5, $9\0Ass.end u13, $4, $5, $6\0Ass.sta.ld.d.v u14, $10\0Ass.app u14, $4, $5, $4\0Ass.end u14, $4, $5, $6\0Ass.sta.ld.d u26, $7\0Ass.end u26, $4, $5, $6\0Ass.sta.ld.d u19, $3\0Ass.end u19, $4, $5, $6\0Ass.sta.st.d u16, $7\0Ass.end u16, $4, $5, $6\0Aso.v.mvsv.d u22, $2\0Aso.v.mvsv.d u20, $1\0A.SLOOP_1:  \0Aso.v.mvsv.d u1, $0\0Aso.v.mvsv.d u3, $0\0Aso.v.dp.d u5, zero, p0\0Aso.v.dp.d u11, zero, p0\0A.SLOOP_1_0:  \0Aso.a.mul.fp  u6, u7, u8, p0\0Aso.a.add.fp  u5, u6, u5, p0\0Aso.a.mul.fp  u12, u13, u14, p0\0Aso.a.add.fp  u11, u12, u11, p0\0Aso.b.ndc.2 u7, .SLOOP_1_0\0Aso.a.adde.fp  u12, u11, p0\0Aso.a.add.fp  u25, u12, u26, p0\0Aso.a.adde.fp  u6, u5, p0\0Aso.a.add.fp  u4, u6, u24, p0\0Aso.a.mul.fp  u18, u19, u20, p0\0Aso.a.mul.fp  u21, u25, u22, p0\0Aso.a.add.fp  u16, u18, u21, p0\0Aso.b.nc u1, .SLOOP_1\0A", "r,r,r,r,r,r,r,r,r,r,r,r"(double 0.000000e+00, double %1, double %2, ptr %5, i64 0, i64 %11, i64 1, ptr %7, ptr %3, i64 %11, ptr %6, ptr %4) #2
  br label %12

12:                                               ; preds = %.loopexit, %8
  %13 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !9
  store i64 %13, ptr @end, align 8, !tbaa !5
  %14 = load i64, ptr @start, align 8, !tbaa !5
  %15 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %14, i64 noundef %13)
  ret void
}

; Function Attrs: nofree nounwind
declare dso_local noundef signext i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,-e,-experimental-xuve,-experimental-zawrs,-experimental-zca,-experimental-zcd,-experimental-zcf,-experimental-zihintntl,-experimental-ztso,-experimental-zvfh,-h,-save-restore,-svinval,-svnapot,-svpbmt,-v,-xtheadvdot,-xventanacondops,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zdinx,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zicbom,-zicbop,-zicboz,-zihintpause,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }
attributes #1 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,-e,-experimental-xuve,-experimental-zawrs,-experimental-zca,-experimental-zcd,-experimental-zcf,-experimental-zihintntl,-experimental-ztso,-experimental-zvfh,-h,-save-restore,-svinval,-svnapot,-svpbmt,-v,-xtheadvdot,-xventanacondops,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zdinx,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zicbom,-zicbop,-zicboz,-zihintpause,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 1, !"SmallDataLimit", i32 8}
!3 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project/ 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!4 = !{i64 191}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 522}
