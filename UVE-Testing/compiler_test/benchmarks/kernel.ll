; ModuleID = 'kernel.c'
source_filename = "kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown-elf"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite)
define dso_local void @core_kernel(ptr nocapture noundef readonly %0, ptr nocapture noundef readonly %1, ptr nocapture noundef %2, i32 noundef signext %3, i32 noundef signext %4, i32 noundef signext %5) local_unnamed_addr #0 {
  %7 = icmp sgt i32 %3, 0
  br i1 %7, label %8, label %45

8:                                                ; preds = %6
  %9 = icmp sgt i32 %4, 0
  %10 = icmp sgt i32 %5, 0
  %11 = sext i32 %4 to i64
  %12 = sext i32 %5 to i64
  %13 = zext i32 %3 to i64
  %14 = zext i32 %4 to i64
  %15 = zext i32 %5 to i64
  br label %16

16:                                               ; preds = %8, %42
  %17 = phi i64 [ 0, %8 ], [ %43, %42 ]
  br i1 %9, label %18, label %42

18:                                               ; preds = %16
  %19 = mul nsw i64 %17, %11
  %20 = mul nsw i64 %17, %12
  br label %21

21:                                               ; preds = %18, %39
  %22 = phi i64 [ 0, %18 ], [ %40, %39 ]
  %23 = add nsw i64 %22, %19
  %24 = getelementptr inbounds double, ptr %2, i64 %23
  store double 0.000000e+00, ptr %24, align 8, !tbaa !4
  br i1 %10, label %25, label %39

25:                                               ; preds = %21, %25
  %26 = phi i64 [ %37, %25 ], [ 0, %21 ]
  %27 = phi double [ %36, %25 ], [ 0.000000e+00, %21 ]
  %28 = add nsw i64 %26, %20
  %29 = getelementptr inbounds double, ptr %0, i64 %28
  %30 = load double, ptr %29, align 8, !tbaa !4
  %31 = mul nsw i64 %26, %11
  %32 = add nsw i64 %31, %22
  %33 = getelementptr inbounds double, ptr %1, i64 %32
  %34 = load double, ptr %33, align 8, !tbaa !4
  %35 = fmul double %30, %34
  %36 = fadd double %27, %35
  store double %36, ptr %24, align 8, !tbaa !4
  %37 = add nuw nsw i64 %26, 1
  %38 = icmp eq i64 %37, %15
  br i1 %38, label %39, label %25, !llvm.loop !8

39:                                               ; preds = %25, %21
  %40 = add nuw nsw i64 %22, 1
  %41 = icmp eq i64 %40, %14
  br i1 %41, label %42, label %21, !llvm.loop !11

42:                                               ; preds = %39, %16
  %43 = add nuw nsw i64 %17, 1
  %44 = icmp eq i64 %43, %13
  br i1 %44, label %45, label %16, !llvm.loop !12

45:                                               ; preds = %42, %6
  ret void
}

attributes #0 = { nofree norecurse nosync nounwind memory(argmem: readwrite) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+m,+relax,-d,-e,-experimental-xuve,-experimental-zawrs,-experimental-zca,-experimental-zcd,-experimental-zcf,-experimental-zihintntl,-experimental-ztso,-experimental-zvfh,-f,-h,-save-restore,-svinval,-svnapot,-svpbmt,-v,-xtheadvdot,-xventanacondops,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zdinx,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zicbom,-zicbop,-zicboz,-zihintpause,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64"}
!2 = !{i32 1, !"SmallDataLimit", i32 8}
!3 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project/ 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"double", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = distinct !{!8, !9, !10}
!9 = !{!"llvm.loop.mustprogress"}
!10 = !{!"llvm.loop.unroll.disable"}
!11 = distinct !{!11, !9, !10}
!12 = distinct !{!12, !9, !10}
