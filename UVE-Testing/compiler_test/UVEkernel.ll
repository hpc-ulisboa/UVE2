; ModuleID = 'kernel.ll'
source_filename = "benchmarks/spmv_ellpack/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(ptr nocapture noundef readonly %0, ptr nocapture noundef readonly %1, ptr nocapture noundef readonly %2, ptr nocapture noundef %3, i32 noundef signext %4, i32 noundef signext %5) local_unnamed_addr #0 {
  %7 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !4
  store i64 %7, ptr @start, align 8, !tbaa !5
  %8 = icmp sgt i32 %4, 0
  br i1 %8, label %.loopexit1, label %12

.loopexit1:                                       ; preds = %6
  %9 = sext i32 %5 to i64
  %10 = zext i32 %4 to i64
  %11 = zext i32 %5 to i64
  call void asm sideeffect "ss.sta.ld.d.v u4, $0\0Ass.app u4, $1, $2, $3\0Ass.end u4, $1, $4, $5\0Ass.sta.ld.w.inds u6, $6\0Ass.app u6, $1, $2, $3\0Ass.end u6, $1, $4, $5\0Ass.sta.ld.d.v u5, $7\0Ass.app u5, $1, $2, $1\0Ass.app u5, $1, $4, $1\0Ass.end.ind.ofs.add.1 u5, u6\0Ass.sta.ld.d u8, $8\0Ass.end u8, $1, $2, $5\0Ass.sta.st.d u1, $8\0Ass.end u1, $1, $2, $5\0A.SLOOP_1:  \0Aso.v.dp.d u2, zero, p0\0A.SLOOP_1_0:  \0Aso.a.mul.fp  u3, u4, u5, p0\0Aso.a.add.fp  u2, u2, u3, p0\0Aso.b.ndc.1 u4, .SLOOP_1_0\0Aso.a.adde.fp  u3, u2, p0\0Aso.a.add.fp  u1, u8, u3, p0\0Aso.b.nc u1, .SLOOP_1\0A", "r,r,r,r,r,r,r,r,r"(ptr %0, i64 0, i64 %10, i64 %9, i64 %11, i64 1, ptr %1, ptr %2, ptr %3) #2
  br label %12

12:                                               ; preds = %.loopexit1, %6
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
!4 = !{i64 170}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 361}
