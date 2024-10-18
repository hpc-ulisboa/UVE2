; ModuleID = 'benchmarks/heat-3d/kernel.c'
source_filename = "benchmarks/heat-3d/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

@start = dso_local local_unnamed_addr global i64 0, align 8
@end = dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"%ld\0A%ld\0A\00", align 1

; Function Attrs: nounwind
define dso_local void @core(ptr nocapture noundef %0, ptr nocapture noundef %1, i32 noundef signext %2, i32 noundef signext %3) local_unnamed_addr #0 {
  %5 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !4
  store i64 %5, ptr @start, align 8, !tbaa !5
  %6 = icmp slt i32 %3, 1
  br i1 %6, label %168, label %7

7:                                                ; preds = %4
  %8 = add i32 %2, -1
  %9 = icmp sgt i32 %2, 2
  %10 = zext i32 %8 to i64
  %11 = zext i32 %8 to i64
  br label %12

12:                                               ; preds = %7, %165
  %13 = phi i32 [ 1, %7 ], [ %166, %165 ]
  br i1 %9, label %15, label %165

14:                                               ; preds = %88
  br i1 %9, label %90, label %165

15:                                               ; preds = %12, %88
  %16 = phi i32 [ %17, %88 ], [ 1, %12 ]
  %17 = add nuw nsw i32 %16, 1
  %18 = mul nsw i32 %17, %2
  %19 = mul nsw i32 %16, %2
  %20 = add nsw i32 %16, -1
  %21 = mul nsw i32 %20, %2
  br label %22

22:                                               ; preds = %85, %15
  %23 = phi i64 [ 1, %15 ], [ %86, %85 ]
  %24 = trunc i64 %23 to i32
  %25 = add i32 %18, %24
  %26 = mul i32 %25, %2
  %27 = add i32 %19, %24
  %28 = mul i32 %27, %2
  %29 = add i32 %21, %24
  %30 = mul i32 %29, %2
  %31 = add i32 %27, 1
  %32 = mul i32 %31, %2
  %33 = add i32 %27, -1
  %34 = mul i32 %33, %2
  %35 = sext i32 %26 to i64
  %36 = sext i32 %28 to i64
  %37 = sext i32 %30 to i64
  %38 = sext i32 %32 to i64
  %39 = sext i32 %34 to i64
  br label %40

40:                                               ; preds = %22, %40
  %41 = phi i64 [ 1, %22 ], [ %67, %40 ]
  %42 = phi i32 [ 1, %22 ], [ %68, %40 ]
  %43 = add nsw i64 %41, %35
  %44 = getelementptr inbounds double, ptr %0, i64 %43
  %45 = load double, ptr %44, align 8, !tbaa !9
  %46 = fadd double %45, -2.000000e+00
  %47 = fmul double %46, 1.250000e-01
  %48 = add nsw i64 %41, %36
  %49 = getelementptr inbounds double, ptr %0, i64 %48
  %50 = load double, ptr %49, align 8, !tbaa !9
  %51 = fmul double %50, %47
  %52 = add nsw i64 %41, %37
  %53 = getelementptr inbounds double, ptr %0, i64 %52
  %54 = load double, ptr %53, align 8, !tbaa !9
  %55 = fadd double %54, %51
  %56 = add nsw i64 %41, %38
  %57 = getelementptr inbounds double, ptr %0, i64 %56
  %58 = load double, ptr %57, align 8, !tbaa !9
  %59 = fadd double %58, -2.000000e+00
  %60 = fmul double %59, 1.250000e-01
  %61 = fmul double %50, %60
  %62 = fadd double %55, %61
  %63 = add nsw i64 %41, %39
  %64 = getelementptr inbounds double, ptr %0, i64 %63
  %65 = load double, ptr %64, align 8, !tbaa !9
  %66 = fadd double %65, %62
  %67 = add nuw nsw i64 %41, 1
  %68 = add nuw nsw i32 %42, 1
  %69 = add nsw i64 %67, %36
  %70 = getelementptr inbounds double, ptr %0, i64 %69
  %71 = load double, ptr %70, align 8, !tbaa !9
  %72 = fadd double %71, -2.000000e+00
  %73 = fmul double %72, 1.250000e-01
  %74 = fmul double %50, %73
  %75 = fadd double %66, %74
  %76 = add i32 %42, %28
  %77 = add i32 %76, -1
  %78 = sext i32 %77 to i64
  %79 = getelementptr inbounds double, ptr %0, i64 %78
  %80 = load double, ptr %79, align 8, !tbaa !9
  %81 = fadd double %80, %75
  %82 = fadd double %50, %81
  %83 = getelementptr inbounds double, ptr %1, i64 %48
  store double %82, ptr %83, align 8, !tbaa !9
  %84 = icmp eq i64 %67, %10
  br i1 %84, label %85, label %40, !llvm.loop !11

85:                                               ; preds = %40
  %86 = add nuw nsw i64 %23, 1
  %87 = icmp eq i64 %86, %10
  br i1 %87, label %88, label %22, !llvm.loop !14

88:                                               ; preds = %85
  %89 = icmp eq i32 %17, %8
  br i1 %89, label %14, label %15, !llvm.loop !15

90:                                               ; preds = %14, %163
  %91 = phi i32 [ %92, %163 ], [ 1, %14 ]
  %92 = add nuw nsw i32 %91, 1
  %93 = mul nsw i32 %92, %2
  %94 = mul nsw i32 %91, %2
  %95 = add nsw i32 %91, -1
  %96 = mul nsw i32 %95, %2
  br label %97

97:                                               ; preds = %160, %90
  %98 = phi i64 [ 1, %90 ], [ %161, %160 ]
  %99 = trunc i64 %98 to i32
  %100 = add i32 %93, %99
  %101 = mul i32 %100, %2
  %102 = add i32 %94, %99
  %103 = mul i32 %102, %2
  %104 = add i32 %96, %99
  %105 = mul i32 %104, %2
  %106 = add i32 %102, 1
  %107 = mul i32 %106, %2
  %108 = add i32 %102, -1
  %109 = mul i32 %108, %2
  %110 = sext i32 %101 to i64
  %111 = sext i32 %103 to i64
  %112 = sext i32 %105 to i64
  %113 = sext i32 %107 to i64
  %114 = sext i32 %109 to i64
  br label %115

115:                                              ; preds = %97, %115
  %116 = phi i64 [ 1, %97 ], [ %142, %115 ]
  %117 = phi i32 [ 1, %97 ], [ %143, %115 ]
  %118 = add nsw i64 %116, %110
  %119 = getelementptr inbounds double, ptr %1, i64 %118
  %120 = load double, ptr %119, align 8, !tbaa !9
  %121 = fadd double %120, -2.000000e+00
  %122 = fmul double %121, 1.250000e-01
  %123 = add nsw i64 %116, %111
  %124 = getelementptr inbounds double, ptr %1, i64 %123
  %125 = load double, ptr %124, align 8, !tbaa !9
  %126 = fmul double %125, %122
  %127 = add nsw i64 %116, %112
  %128 = getelementptr inbounds double, ptr %1, i64 %127
  %129 = load double, ptr %128, align 8, !tbaa !9
  %130 = fadd double %129, %126
  %131 = add nsw i64 %116, %113
  %132 = getelementptr inbounds double, ptr %1, i64 %131
  %133 = load double, ptr %132, align 8, !tbaa !9
  %134 = fadd double %133, -2.000000e+00
  %135 = fmul double %134, 1.250000e-01
  %136 = fmul double %125, %135
  %137 = fadd double %130, %136
  %138 = add nsw i64 %116, %114
  %139 = getelementptr inbounds double, ptr %1, i64 %138
  %140 = load double, ptr %139, align 8, !tbaa !9
  %141 = fadd double %140, %137
  %142 = add nuw nsw i64 %116, 1
  %143 = add nuw nsw i32 %117, 1
  %144 = add nsw i64 %142, %111
  %145 = getelementptr inbounds double, ptr %1, i64 %144
  %146 = load double, ptr %145, align 8, !tbaa !9
  %147 = fadd double %146, -2.000000e+00
  %148 = fmul double %147, 1.250000e-01
  %149 = fmul double %125, %148
  %150 = fadd double %141, %149
  %151 = add i32 %117, %103
  %152 = add i32 %151, -1
  %153 = sext i32 %152 to i64
  %154 = getelementptr inbounds double, ptr %1, i64 %153
  %155 = load double, ptr %154, align 8, !tbaa !9
  %156 = fadd double %155, %150
  %157 = fadd double %125, %156
  %158 = getelementptr inbounds double, ptr %0, i64 %123
  store double %157, ptr %158, align 8, !tbaa !9
  %159 = icmp eq i64 %142, %11
  br i1 %159, label %160, label %115, !llvm.loop !16

160:                                              ; preds = %115
  %161 = add nuw nsw i64 %98, 1
  %162 = icmp eq i64 %161, %11
  br i1 %162, label %163, label %97, !llvm.loop !17

163:                                              ; preds = %160
  %164 = icmp eq i32 %92, %8
  br i1 %164, label %165, label %90, !llvm.loop !18

165:                                              ; preds = %163, %12, %14
  %166 = add nuw i32 %13, 1
  %167 = icmp eq i32 %13, %3
  br i1 %167, label %168, label %12, !llvm.loop !19

168:                                              ; preds = %165, %4
  %169 = tail call i64 asm sideeffect "rdinstret $0 \09\0A", "=&r"() #2, !srcloc !20
  store i64 %169, ptr @end, align 8, !tbaa !5
  %170 = load i64, ptr @start, align 8, !tbaa !5
  %171 = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %170, i64 noundef %169)
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
!4 = !{i64 197}
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
!18 = distinct !{!18, !12, !13}
!19 = distinct !{!19, !12, !13}
!20 = !{i64 1232}
