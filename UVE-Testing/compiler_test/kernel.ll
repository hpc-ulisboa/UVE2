; ModuleID = 'benchmarks/spmv_ellpack/kernel.c'
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
  br i1 %8, label %9, label %20

9:                                                ; preds = %6
  %10 = icmp sgt i32 %5, 0
  %11 = sext i32 %5 to i64
  %12 = zext i32 %4 to i64
  %13 = zext i32 %5 to i64
  br label %14

14:                                               ; preds = %9, %24
  %15 = phi i64 [ 0, %9 ], [ %25, %24 ]
  br i1 %10, label %16, label %24

16:                                               ; preds = %14
  %17 = mul nsw i64 %15, %11
  %18 = getelementptr inbounds double, ptr %3, i64 %15
  %19 = load double, ptr %18, align 8, !tbaa !9
  br label %27

20:                                               ; preds = %24, %6
  %21 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !11
  store i64 %21, ptr @end, align 8, !tbaa !5
  %22 = load i64, ptr @start, align 8, !tbaa !5
  %23 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %22, i64 noundef %21)
  ret void

24:                                               ; preds = %27, %14
  %25 = add nuw nsw i64 %15, 1
  %26 = icmp eq i64 %25, %12
  br i1 %26, label %20, label %14, !llvm.loop !12

27:                                               ; preds = %16, %27
  %28 = phi i64 [ 0, %16 ], [ %40, %27 ]
  %29 = phi double [ %19, %16 ], [ %39, %27 ]
  %30 = add nsw i64 %28, %17
  %31 = getelementptr inbounds double, ptr %0, i64 %30
  %32 = load double, ptr %31, align 8, !tbaa !9
  %33 = getelementptr inbounds i32, ptr %1, i64 %30
  %34 = load i32, ptr %33, align 4, !tbaa !15
  %35 = sext i32 %34 to i64
  %36 = getelementptr inbounds double, ptr %2, i64 %35
  %37 = load double, ptr %36, align 8, !tbaa !9
  %38 = fmul double %32, %37
  %39 = fadd double %29, %38
  store double %39, ptr %18, align 8, !tbaa !9
  %40 = add nuw nsw i64 %28, 1
  %41 = icmp eq i64 %40, %13
  br i1 %41, label %24, label %27, !llvm.loop !17
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
!9 = !{!10, !10, i64 0}
!10 = !{!"double", !7, i64 0}
!11 = !{i64 361}
!12 = distinct !{!12, !13, !14}
!13 = !{!"llvm.loop.mustprogress"}
!14 = !{!"llvm.loop.unroll.disable"}
!15 = !{!16, !16, i64 0}
!16 = !{!"int", !7, i64 0}
!17 = distinct !{!17, !13, !14}
