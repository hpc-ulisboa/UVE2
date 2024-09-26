; ModuleID = 'kernel.ll'
source_filename = "benchmarks/convolution/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1, ptr nocapture noundef readonly %2, i32 noundef signext %3, i32 noundef signext %4) local_unnamed_addr #0 {
  %6 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !4
  store i64 %6, ptr @start, align 8, !tbaa !5
  %7 = icmp sgt i32 %4, 2
  br i1 %7, label %.loopexit1, label %17

.loopexit1:                                       ; preds = %5
  %8 = add nsw i32 %4, -1
  %9 = add i32 %3, -1
  %10 = icmp sgt i32 %3, 2
  %11 = sext i32 %3 to i64
  %12 = zext i32 %8 to i64
  %13 = zext i32 %9 to i64
  %14 = add nsw i64 %12, -1
  %15 = add nsw i64 %13, -1
  %16 = sub nsw i64 0, %11
  call void asm sideeffect "ss.sta.ld.w.v.1 u4, $1\0Ass.app u4, $2, $3, $4\0Ass.app u4, $5, $6, $5\0Ass.app u4, $7, $8, $7\0Ass.end u4, $9, $8, $10\0Ass.sta.ld.w.v.1 u5, $11\0Ass.app u5, $12, $3, $12\0Ass.app u5, $12, $6, $12\0Ass.app u5, $12, $8, $5\0Ass.end u5, $12, $8, $8\0Ass.sta.st.w.v.1 u1, $13\0Ass.app u1, $14, $3, $4\0Ass.end u1, $5, $6, $5\0A.SLOOP_1:  \0A.SLOOP_1_0_0:  \0Aso.b.nc u ERROR, .SLOOP_1_0_0\0Aso.a.mul.sg  u3, u4, u5, p0\0Aso.a.add.sg  u1, u3, u2, p0\0Aso.b.nc u1, .SLOOP_1\0A", "r,r,r,r,r,r,r,r,r,r,r,r,r,r,r"(i32 0, ptr %0, i64 %11, i64 %14, i64 %11, i64 1, i64 %15, i64 -1, i64 3, i64 %16, i64 %16, ptr %2, i64 0, ptr %1, i64 %11) #2
  br label %17

17:                                               ; preds = %.loopexit1, %5
  %18 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !9
  store i64 %18, ptr @end, align 8, !tbaa !5
  %19 = load i64, ptr @start, align 8, !tbaa !5
  %20 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %19, i64 noundef %18)
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
!9 = !{i64 608}
