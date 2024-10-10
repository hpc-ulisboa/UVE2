; ModuleID = 'kernel.ll'
source_filename = "benchmarks/stream/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(ptr nocapture noundef %0, ptr nocapture noundef %1, ptr nocapture noundef %2, i64 noundef %3, double noundef %4) local_unnamed_addr #0 {
  %6 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !4
  store i64 %6, ptr @start, align 8, !tbaa !5
  %7 = icmp eq i64 %3, 0
  br i1 %7, label %8, label %.preheader3

.preheader3:                                      ; preds = %5
  call void asm sideeffect "ss.sta.ld.d.v u2, $0\0Ass.end u2, $1, $2, $3\0Ass.sta.st.d.v u1, $4\0Ass.end u1, $1, $2, $3\0A.SLOOP_4:  \0Aso.v.mv u1, u2, p0\0Aso.b.nc u1, .SLOOP_4\0A", "r,r,r,r,r"(ptr %0, i64 0, i64 %3, i64 1, ptr %2) #2
  call void asm sideeffect "ss.sta.ld.d.v u3, $1\0Ass.end u3, $2, $3, $4\0Ass.sta.st.d.v u1, $5\0Ass.end u1, $2, $3, $4\0Aso.v.dp.d u4, $0, p0\0A.SLOOP_3:  \0Aso.a.mul.fp  u1, u3, u4, p0\0Aso.b.nc u1, .SLOOP_3\0A", "r,r,r,r,r,r"(double %4, ptr %2, i64 0, i64 %3, i64 1, ptr %1) #2
  call void asm sideeffect "ss.sta.ld.d.v u3, $0\0Ass.end u3, $1, $2, $3\0Ass.sta.ld.d.v u4, $4\0Ass.end u4, $1, $2, $3\0Ass.sta.st.d.v u1, $5\0Ass.end u1, $1, $2, $3\0A.SLOOP_2:  \0Aso.a.add.fp  u1, u3, u4, p0\0Aso.b.nc u1, .SLOOP_2\0A", "r,r,r,r,r,r"(ptr %0, i64 0, i64 %3, i64 1, ptr %1, ptr %2) #2
  call void asm sideeffect "ss.sta.ld.d.v u3, $1\0Ass.end u3, $2, $3, $4\0Ass.sta.ld.d.v u5, $5\0Ass.end u5, $2, $3, $4\0Ass.sta.st.d.v u1, $6\0Ass.end u1, $2, $3, $4\0Aso.v.dp.d u6, $0, p0\0A.SLOOP_1:  \0Aso.a.mul.fp  u4, u5, u6, p0\0Aso.a.add.fp  u1, u3, u4, p0\0Aso.b.nc u1, .SLOOP_1\0A", "r,r,r,r,r,r,r"(double %4, ptr %1, i64 0, i64 %3, i64 1, ptr %2, ptr %0) #2
  br label %8

8:                                                ; preds = %.preheader3, %5
  %9 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !9
  store i64 %9, ptr @end, align 8, !tbaa !5
  %10 = load i64, ptr @start, align 8, !tbaa !5
  %11 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %10, i64 noundef %9)
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
!4 = !{i64 158}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 460}
