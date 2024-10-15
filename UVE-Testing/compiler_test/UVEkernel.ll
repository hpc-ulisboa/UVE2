; ModuleID = 'kernel.ll'
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
  br i1 %4, label %.loopexit3, label %8

.loopexit3:                                       ; preds = %2
  %5 = add nuw i32 %1, 1
  %6 = zext i32 %1 to i64
  %7 = sext i32 %5 to i64
  call void asm sideeffect "ss.sta.ld.d.v u4, $0\0Ass.app u4, $1, $2, $3\0Ass.app.mod.siz.inc.2 u4, $4\0Ass.app u4, $1, $1, $1\0Ass.app.mod.siz.inc.3 u4, $4\0Ass.end u4, $1, $1, $4\0Ass.sta.ld.d.v u5, $0\0Ass.app u5, $1, $2, $1\0Ass.app.mod.siz.inc.2 u5, $4\0Ass.app u5, $1, $1, $4\0Ass.app.mod.siz.inc.3 u5, $4\0Ass.end u5, $1, $1, $3\0Ass.sta.ld.d u16, $0\0Ass.app u16, $1, $2, $3\0Ass.app.mod.siz.inc.2 u16, $4\0Ass.end u16, $1, $1, $4\0Ass.sta.st.d u1, $0\0Ass.app u1, $1, $2, $3\0Ass.app.mod.siz.inc.2 u1, $4\0Ass.end u1, $1, $1, $4\0Ass.sta.ld.d u8, $0\0Ass.app u8, $1, $2, $3\0Ass.app.mod.siz.inc.2 u8, $4\0Ass.end u8, $1, $1, $4\0Ass.sta.ld.d u9, $0\0Ass.app u9, $1, $2, $1\0Ass.app.mod.siz.inc.2 u9, $4\0Ass.end u9, $1, $1, $5\0Ass.sta.st.d u6, $0\0Ass.app u6, $1, $2, $3\0Ass.app.mod.siz.inc.2 u6, $4\0Ass.end u6, $1, $1, $4\0Ass.sta.ld.d.v u13, $0\0Ass.app u13, $1, $2, $3\0Ass.app.mod.ofs.inc.2 u13, $4\0Ass.app.mod.siz.dec.2 u13, $4\0Ass.app.mod.siz.inc.3 u13, $4\0Ass.app u13, $1, $2, $1\0Ass.end u13, $1, $1, $4\0Ass.sta.ld.d.v u14, $0\0Ass.app u14, $1, $2, $1\0Ass.app.mod.ofs.inc.2 u14, $4\0Ass.app.mod.siz.dec.2 u14, $4\0Ass.app.mod.siz.inc.3 u14, $4\0Ass.app u14, $1, $2, $4\0Ass.end u14, $1, $1, $3\0Ass.sta.st.d.v u10, $0\0Ass.app u10, $1, $2, $3\0Ass.app.mod.ofs.inc.2 u10, $4\0Ass.app.mod.siz.dec.2 u10, $4\0Ass.app.mod.siz.inc.3 u10, $4\0Ass.app u10, $1, $2, $4\0Ass.end u10, $1, $1, $1\0A.SLOOP_1:  \0A.SLOOP_1_0:  \0Aso.v.dp.d u2, zero, p0\0A.SLOOP_1_0_0:  \0Aso.a.mul.fp  u3, u4, u5, p0\0Aso.a.sub.fp  u2, u2, u3, p0\0Aso.b.ndc.3 u4, .SLOOP_1_0_0\0Aso.a.adde.fp  u3, u2, p0\0Aso.a.add.fp  u1, u16, u3, p0\0Aso.a.div.fp  u6, u8, u9, p0\0Aso.b.ndc.2 u1, .SLOOP_1_0\0A.SLOOP_1_1:  \0Aso.a.mul.fp  u12, u13, u14, p0\0Aso.a.sub.fp  u10, u11, u12, p0\0Aso.b.ndc.2 u10, .SLOOP_1_1\0Aso.b.nc u1, .SLOOP_1\0A", "r,r,r,r,r,r"(ptr %0, i64 0, i64 %6, i64 %6, i64 1, i64 %7) #2
  br label %8

8:                                                ; preds = %.loopexit3, %2
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
!4 = !{i64 110}
!5 = !{!6, !6, i64 0}
!6 = !{!"long", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 521}
