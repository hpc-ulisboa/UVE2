; ModuleID = 'benchmarks/convolution/kernel.c'
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
  br i1 %7, label %8, label %52

8:                                                ; preds = %5
  %9 = add nsw i32 %4, -1
  %10 = add i32 %3, -1
  %11 = icmp sgt i32 %3, 2
  %12 = sext i32 %3 to i64
  %13 = zext i32 %9 to i64
  %14 = zext i32 %10 to i64
  br label %15

15:                                               ; preds = %8, %49
  %16 = phi i64 [ 1, %8 ], [ %50, %49 ]
  br i1 %11, label %17, label %49

17:                                               ; preds = %15
  %18 = mul nsw i64 %16, %12
  br label %19

19:                                               ; preds = %17, %44
  %20 = phi i64 [ 1, %17 ], [ %47, %44 ]
  br label %21

21:                                               ; preds = %19, %42
  %22 = phi i64 [ -1, %19 ], [ %24, %42 ]
  %23 = phi i32 [ 0, %19 ], [ %40, %42 ]
  %24 = add nsw i64 %22, 1
  %25 = sub nsw i64 %20, %22
  br label %26

26:                                               ; preds = %21, %26
  %27 = phi i64 [ -1, %21 ], [ %29, %26 ]
  %28 = phi i32 [ %23, %21 ], [ %40, %26 ]
  %29 = add nsw i64 %27, 1
  %30 = mul nuw nsw i64 %29, 3
  %31 = add nuw nsw i64 %24, %30
  %32 = getelementptr inbounds i32, ptr %2, i64 %31
  %33 = load i32, ptr %32, align 4, !tbaa !9
  %34 = sub nsw i64 %16, %27
  %35 = mul nsw i64 %34, %12
  %36 = add nsw i64 %25, %35
  %37 = getelementptr inbounds i32, ptr %0, i64 %36
  %38 = load i32, ptr %37, align 4, !tbaa !9
  %39 = mul nsw i32 %38, %33
  %40 = add nsw i32 %39, %28
  %41 = icmp eq i64 %29, 2
  br i1 %41, label %42, label %26, !llvm.loop !11

42:                                               ; preds = %26
  %43 = icmp eq i64 %24, 2
  br i1 %43, label %44, label %21, !llvm.loop !14

44:                                               ; preds = %42
  %45 = add nsw i64 %20, %18
  %46 = getelementptr inbounds i32, ptr %1, i64 %45
  store i32 %40, ptr %46, align 4, !tbaa !9
  %47 = add nuw nsw i64 %20, 1
  %48 = icmp eq i64 %47, %14
  br i1 %48, label %49, label %19, !llvm.loop !15

49:                                               ; preds = %44, %15
  %50 = add nuw nsw i64 %16, 1
  %51 = icmp eq i64 %50, %13
  br i1 %51, label %52, label %15, !llvm.loop !16

52:                                               ; preds = %49, %5
  %53 = tail call i64 asm sideeffect "rdinstret $0 \0A", "=&r"() #2, !srcloc !17
  store i64 %53, ptr @end, align 8, !tbaa !5
  %54 = load i64, ptr @start, align 8, !tbaa !5
  %55 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %54, i64 noundef %53)
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
!9 = !{!10, !10, i64 0}
!10 = !{!"int", !7, i64 0}
!11 = distinct !{!11, !12, !13}
!12 = !{!"llvm.loop.mustprogress"}
!13 = !{!"llvm.loop.unroll.disable"}
!14 = distinct !{!14, !12, !13}
!15 = distinct !{!15, !12, !13}
!16 = distinct !{!16, !12, !13}
!17 = !{i64 608}
