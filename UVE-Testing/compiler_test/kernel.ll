; ModuleID = 'benchmarks/lu/kernel.c'
source_filename = "benchmarks/lu/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(ptr nocapture noundef %0, i32 noundef signext %1) local_unnamed_addr #0 {
  %3 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !4
  store i64 %3, ptr @start, align 8, !tbaa !5
  %4 = icmp sgt i32 %1, 0
  br i1 %4, label %5, label %74

5:                                                ; preds = %2
  %6 = add nuw i32 %1, 1
  %7 = zext i32 %1 to i64
  %8 = zext i32 %1 to i64
  br label %9

9:                                                ; preds = %5, %71
  %10 = phi i64 [ 0, %5 ], [ %72, %71 ]
  %11 = icmp eq i64 %10, 0
  br i1 %11, label %14, label %12

12:                                               ; preds = %9
  %13 = mul nsw i64 %10, %7
  br label %16

14:                                               ; preds = %37, %9
  %15 = mul nsw i64 %10, %7
  br label %48

16:                                               ; preds = %12, %37
  %17 = phi i64 [ 0, %12 ], [ %46, %37 ]
  %18 = icmp eq i64 %17, 0
  %19 = add nuw nsw i64 %17, %13
  br i1 %18, label %37, label %20

20:                                               ; preds = %16
  %21 = getelementptr inbounds double, ptr %0, i64 %19
  %22 = load double, ptr %21, align 8, !tbaa !9
  br label %23

23:                                               ; preds = %20, %23
  %24 = phi i64 [ 0, %20 ], [ %35, %23 ]
  %25 = phi double [ %22, %20 ], [ %34, %23 ]
  %26 = add nuw nsw i64 %24, %13
  %27 = getelementptr inbounds double, ptr %0, i64 %26
  %28 = load double, ptr %27, align 8, !tbaa !9
  %29 = mul nsw i64 %24, %7
  %30 = add nuw nsw i64 %29, %17
  %31 = getelementptr inbounds double, ptr %0, i64 %30
  %32 = load double, ptr %31, align 8, !tbaa !9
  %33 = fmul double %28, %32
  %34 = fsub double %25, %33
  store double %34, ptr %21, align 8, !tbaa !9
  %35 = add nuw nsw i64 %24, 1
  %36 = icmp eq i64 %35, %17
  br i1 %36, label %37, label %23, !llvm.loop !11

37:                                               ; preds = %23, %16
  %38 = trunc i64 %17 to i32
  %39 = mul i32 %6, %38
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds double, ptr %0, i64 %40
  %42 = load double, ptr %41, align 8, !tbaa !9
  %43 = getelementptr inbounds double, ptr %0, i64 %19
  %44 = load double, ptr %43, align 8, !tbaa !9
  %45 = fdiv double %44, %42
  store double %45, ptr %43, align 8, !tbaa !9
  %46 = add nuw nsw i64 %17, 1
  %47 = icmp eq i64 %46, %10
  br i1 %47, label %14, label %16, !llvm.loop !14

48:                                               ; preds = %14, %68
  %49 = phi i64 [ %10, %14 ], [ %69, %68 ]
  br i1 %11, label %68, label %50

50:                                               ; preds = %48
  %51 = add nuw nsw i64 %49, %15
  %52 = getelementptr inbounds double, ptr %0, i64 %51
  %53 = load double, ptr %52, align 8, !tbaa !9
  br label %54

54:                                               ; preds = %50, %54
  %55 = phi i64 [ 0, %50 ], [ %66, %54 ]
  %56 = phi double [ %53, %50 ], [ %65, %54 ]
  %57 = add nuw nsw i64 %55, %15
  %58 = getelementptr inbounds double, ptr %0, i64 %57
  %59 = load double, ptr %58, align 8, !tbaa !9
  %60 = mul nsw i64 %55, %7
  %61 = add nuw nsw i64 %60, %49
  %62 = getelementptr inbounds double, ptr %0, i64 %61
  %63 = load double, ptr %62, align 8, !tbaa !9
  %64 = fmul double %59, %63
  %65 = fsub double %56, %64
  store double %65, ptr %52, align 8, !tbaa !9
  %66 = add nuw nsw i64 %55, 1
  %67 = icmp eq i64 %66, %10
  br i1 %67, label %68, label %54, !llvm.loop !15

68:                                               ; preds = %54, %48
  %69 = add nuw nsw i64 %49, 1
  %70 = icmp eq i64 %69, %8
  br i1 %70, label %71, label %48, !llvm.loop !16

71:                                               ; preds = %68
  %72 = add nuw nsw i64 %10, 1
  %73 = icmp eq i64 %72, %8
  br i1 %73, label %74, label %9, !llvm.loop !17

74:                                               ; preds = %71, %2
  %75 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !18
  store i64 %75, ptr @end, align 8, !tbaa !5
  %76 = load i64, ptr @start, align 8, !tbaa !5
  %77 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %76, i64 noundef %75)
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
!4 = !{i64 110}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"double", !7, i64 0}
!11 = distinct !{!11, !12, !13}
!12 = !{!"llvm.loop.mustprogress"}
!13 = !{!"llvm.loop.unroll.disable"}
!14 = distinct !{!14, !12, !13}
!15 = distinct !{!15, !12, !13}
!16 = distinct !{!16, !12, !13}
!17 = distinct !{!17, !12, !13}
!18 = !{i64 521}
