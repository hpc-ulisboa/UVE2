; ModuleID = 'benchmarks/3mm/kernel.c'
source_filename = "benchmarks/3mm/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown-elf"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite)
define dso_local void @core_kernel(ptr nocapture noundef readonly %0, ptr nocapture noundef readonly %1, ptr nocapture noundef %2, i64 noundef %3, i64 noundef %4, i64 noundef %5) local_unnamed_addr #0 {
  %7 = icmp eq i64 %3, 0
  br i1 %7, label %40, label %8

8:                                                ; preds = %6
  %9 = icmp eq i64 %4, 0
  %10 = icmp eq i64 %5, 0
  br label %11

11:                                               ; preds = %8, %37
  %12 = phi i64 [ 0, %8 ], [ %38, %37 ]
  br i1 %9, label %37, label %13

13:                                               ; preds = %11
  %14 = mul i64 %12, %4
  %15 = mul i64 %12, %5
  br label %16

16:                                               ; preds = %13, %34
  %17 = phi i64 [ 0, %13 ], [ %35, %34 ]
  %18 = add i64 %17, %14
  %19 = getelementptr inbounds double, ptr %2, i64 %18
  store double 0.000000e+00, ptr %19, align 8, !tbaa !4
  br i1 %10, label %34, label %20

20:                                               ; preds = %16, %20
  %21 = phi i64 [ %32, %20 ], [ 0, %16 ]
  %22 = phi double [ %31, %20 ], [ 0.000000e+00, %16 ]
  %23 = add i64 %21, %15
  %24 = getelementptr inbounds double, ptr %0, i64 %23
  %25 = load double, ptr %24, align 8, !tbaa !4
  %26 = mul i64 %21, %4
  %27 = add i64 %26, %17
  %28 = getelementptr inbounds double, ptr %1, i64 %27
  %29 = load double, ptr %28, align 8, !tbaa !4
  %30 = fmul double %25, %29
  %31 = fadd double %22, %30
  store double %31, ptr %19, align 8, !tbaa !4
  %32 = add nuw nsw i64 %21, 1
  %33 = icmp eq i64 %32, %5
  br i1 %33, label %34, label %20, !llvm.loop !8

34:                                               ; preds = %20, %16
  %35 = add nuw nsw i64 %17, 1
  %36 = icmp eq i64 %35, %4
  br i1 %36, label %37, label %16, !llvm.loop !11

37:                                               ; preds = %34, %11
  %38 = add nuw nsw i64 %12, 1
  %39 = icmp eq i64 %38, %3
  br i1 %39, label %40, label %11, !llvm.loop !12

40:                                               ; preds = %37, %6
  ret void
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite)
define dso_local void @core(ptr nocapture noundef readonly %0, ptr nocapture noundef readonly %1, ptr nocapture noundef readonly %2, ptr nocapture noundef readonly %3, ptr nocapture noundef %4, ptr nocapture noundef %5, ptr nocapture noundef writeonly %6, i64 noundef %7, i64 noundef %8, i64 noundef %9, i64 noundef %10, i64 noundef %11) local_unnamed_addr #0 {
  %13 = icmp eq i64 %7, 0
  br i1 %13, label %46, label %14

14:                                               ; preds = %12
  %15 = icmp eq i64 %8, 0
  %16 = icmp eq i64 %9, 0
  br label %17

17:                                               ; preds = %43, %14
  %18 = phi i64 [ 0, %14 ], [ %44, %43 ]
  br i1 %15, label %43, label %19

19:                                               ; preds = %17
  %20 = mul i64 %18, %8
  %21 = mul i64 %18, %9
  br label %22

22:                                               ; preds = %40, %19
  %23 = phi i64 [ 0, %19 ], [ %41, %40 ]
  %24 = add i64 %23, %20
  %25 = getelementptr inbounds double, ptr %4, i64 %24
  store double 0.000000e+00, ptr %25, align 8, !tbaa !4
  br i1 %16, label %40, label %26

26:                                               ; preds = %22, %26
  %27 = phi i64 [ %38, %26 ], [ 0, %22 ]
  %28 = phi double [ %37, %26 ], [ 0.000000e+00, %22 ]
  %29 = add i64 %27, %21
  %30 = getelementptr inbounds double, ptr %0, i64 %29
  %31 = load double, ptr %30, align 8, !tbaa !4
  %32 = mul i64 %27, %8
  %33 = add i64 %32, %23
  %34 = getelementptr inbounds double, ptr %1, i64 %33
  %35 = load double, ptr %34, align 8, !tbaa !4
  %36 = fmul double %31, %35
  %37 = fadd double %28, %36
  store double %37, ptr %25, align 8, !tbaa !4
  %38 = add nuw nsw i64 %27, 1
  %39 = icmp eq i64 %38, %9
  br i1 %39, label %40, label %26, !llvm.loop !8

40:                                               ; preds = %26, %22
  %41 = add nuw nsw i64 %23, 1
  %42 = icmp eq i64 %41, %8
  br i1 %42, label %43, label %22, !llvm.loop !11

43:                                               ; preds = %40, %17
  %44 = add nuw nsw i64 %18, 1
  %45 = icmp eq i64 %44, %7
  br i1 %45, label %46, label %17, !llvm.loop !12

46:                                               ; preds = %43, %12
  %47 = icmp eq i64 %8, 0
  br i1 %47, label %80, label %48

48:                                               ; preds = %46
  %49 = icmp eq i64 %10, 0
  %50 = icmp eq i64 %11, 0
  br label %51

51:                                               ; preds = %77, %48
  %52 = phi i64 [ 0, %48 ], [ %78, %77 ]
  br i1 %49, label %77, label %53

53:                                               ; preds = %51
  %54 = mul i64 %52, %10
  %55 = mul i64 %52, %11
  br label %56

56:                                               ; preds = %74, %53
  %57 = phi i64 [ 0, %53 ], [ %75, %74 ]
  %58 = add i64 %57, %54
  %59 = getelementptr inbounds double, ptr %5, i64 %58
  store double 0.000000e+00, ptr %59, align 8, !tbaa !4
  br i1 %50, label %74, label %60

60:                                               ; preds = %56, %60
  %61 = phi i64 [ %72, %60 ], [ 0, %56 ]
  %62 = phi double [ %71, %60 ], [ 0.000000e+00, %56 ]
  %63 = add i64 %61, %55
  %64 = getelementptr inbounds double, ptr %2, i64 %63
  %65 = load double, ptr %64, align 8, !tbaa !4
  %66 = mul i64 %61, %10
  %67 = add i64 %66, %57
  %68 = getelementptr inbounds double, ptr %3, i64 %67
  %69 = load double, ptr %68, align 8, !tbaa !4
  %70 = fmul double %65, %69
  %71 = fadd double %62, %70
  store double %71, ptr %59, align 8, !tbaa !4
  %72 = add nuw nsw i64 %61, 1
  %73 = icmp eq i64 %72, %11
  br i1 %73, label %74, label %60, !llvm.loop !8

74:                                               ; preds = %60, %56
  %75 = add nuw nsw i64 %57, 1
  %76 = icmp eq i64 %75, %10
  br i1 %76, label %77, label %56, !llvm.loop !11

77:                                               ; preds = %74, %51
  %78 = add nuw nsw i64 %52, 1
  %79 = icmp eq i64 %78, %8
  br i1 %79, label %80, label %51, !llvm.loop !12

80:                                               ; preds = %77, %46
  br i1 %13, label %112, label %81

81:                                               ; preds = %80
  %82 = icmp eq i64 %10, 0
  br label %83

83:                                               ; preds = %109, %81
  %84 = phi i64 [ 0, %81 ], [ %110, %109 ]
  br i1 %82, label %109, label %85

85:                                               ; preds = %83
  %86 = mul i64 %84, %10
  %87 = mul i64 %84, %8
  br label %88

88:                                               ; preds = %106, %85
  %89 = phi i64 [ 0, %85 ], [ %107, %106 ]
  %90 = add i64 %89, %86
  %91 = getelementptr inbounds double, ptr %6, i64 %90
  store double 0.000000e+00, ptr %91, align 8, !tbaa !4
  br i1 %47, label %106, label %92

92:                                               ; preds = %88, %92
  %93 = phi i64 [ %104, %92 ], [ 0, %88 ]
  %94 = phi double [ %103, %92 ], [ 0.000000e+00, %88 ]
  %95 = add i64 %93, %87
  %96 = getelementptr inbounds double, ptr %4, i64 %95
  %97 = load double, ptr %96, align 8, !tbaa !4
  %98 = mul i64 %93, %10
  %99 = add i64 %98, %89
  %100 = getelementptr inbounds double, ptr %5, i64 %99
  %101 = load double, ptr %100, align 8, !tbaa !4
  %102 = fmul double %97, %101
  %103 = fadd double %94, %102
  store double %103, ptr %91, align 8, !tbaa !4
  %104 = add nuw nsw i64 %93, 1
  %105 = icmp eq i64 %104, %8
  br i1 %105, label %106, label %92, !llvm.loop !8

106:                                              ; preds = %92, %88
  %107 = add nuw nsw i64 %89, 1
  %108 = icmp eq i64 %107, %10
  br i1 %108, label %109, label %88, !llvm.loop !11

109:                                              ; preds = %106, %83
  %110 = add nuw nsw i64 %84, 1
  %111 = icmp eq i64 %110, %7
  br i1 %111, label %112, label %83, !llvm.loop !12

112:                                              ; preds = %109, %80
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
