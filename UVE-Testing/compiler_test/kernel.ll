; ModuleID = 'benchmarks/3mm/kernel.c'
source_filename = "benchmarks/3mm/kernel.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown-elf"

; Function Attrs: nounwind
define dso_local void @uve_config(ptr noundef %0, ptr noundef %1, ptr noundef %2, i64 noundef %3, i64 noundef %4, i64 noundef %5) local_unnamed_addr #0 {
  tail call void asm sideeffect "ss.sta.ld.d           u1, $0, $3, $5 \09\0Ass.app                u1, zero, $4, zero \09\0Ass.end                u1, zero, $5, $6 \09\0Ass.cfg.vec            u1                      \09\0Ass.sta.ld.d           u2, $1, $3, zero \09\0Ass.app                u2, zero, $4, $6 \09\0Ass.end                u2, zero, $5, $4 \09\0Ass.cfg.vec            u2                     \09\0Ass.sta.st.d           u4, $2, $3, $4 \09\0Ass.end                u4, zero, $4, $6 \09\0A", "r,r,r,r,r,r,r"(ptr %0, ptr %1, ptr %2, i64 %3, i64 %4, i64 %5, i32 1) #1, !srcloc !4
  ret void
}

; Function Attrs: nounwind
define dso_local void @uve_kernel() local_unnamed_addr #0 {
  tail call void asm sideeffect ".iLoop1${:uid}: \09\0Aso.v.dp.d u21, zero, p0 \09\0A.kloop1${:uid}: \09\0Aso.a.mul.fp u22, u1, u2, p0\0A\09so.a.add.fp u21, u21, u22, p0\0A\09so.b.ndc.1 u2, .kloop1${:uid} \0A\09so.a.adde.fp  u4, u21, p0 \0A\09so.b.nc\09u2, .iLoop1${:uid} \0A\09", ""() #1, !srcloc !5
  ret void
}

; Function Attrs: nounwind
define dso_local void @core(ptr noundef %0, ptr noundef %1, ptr noundef %2, ptr noundef %3, ptr noundef %4, ptr noundef %5, ptr noundef %6, i64 noundef %7, i64 noundef %8, i64 noundef %9, i64 noundef %10, i64 noundef %11) local_unnamed_addr #0 {
  tail call void asm sideeffect "ss.sta.ld.d           u1, $0, $3, $5 \09\0Ass.app                u1, zero, $4, zero \09\0Ass.end                u1, zero, $5, $6 \09\0Ass.cfg.vec            u1                      \09\0Ass.sta.ld.d           u2, $1, $3, zero \09\0Ass.app                u2, zero, $4, $6 \09\0Ass.end                u2, zero, $5, $4 \09\0Ass.cfg.vec            u2                     \09\0Ass.sta.st.d           u4, $2, $3, $4 \09\0Ass.end                u4, zero, $4, $6 \09\0A", "r,r,r,r,r,r,r"(ptr %0, ptr %1, ptr %4, i64 %7, i64 %8, i64 %9, i32 1) #1, !srcloc !4
  tail call void asm sideeffect ".iLoop1${:uid}: \09\0Aso.v.dp.d u21, zero, p0 \09\0A.kloop1${:uid}: \09\0Aso.a.mul.fp u22, u1, u2, p0\0A\09so.a.add.fp u21, u21, u22, p0\0A\09so.b.ndc.1 u2, .kloop1${:uid} \0A\09so.a.adde.fp  u4, u21, p0 \0A\09so.b.nc\09u2, .iLoop1${:uid} \0A\09", ""() #1, !srcloc !5
  tail call void asm sideeffect "ss.sta.ld.d           u1, $0, $3, $5 \09\0Ass.app                u1, zero, $4, zero \09\0Ass.end                u1, zero, $5, $6 \09\0Ass.cfg.vec            u1                      \09\0Ass.sta.ld.d           u2, $1, $3, zero \09\0Ass.app                u2, zero, $4, $6 \09\0Ass.end                u2, zero, $5, $4 \09\0Ass.cfg.vec            u2                     \09\0Ass.sta.st.d           u4, $2, $3, $4 \09\0Ass.end                u4, zero, $4, $6 \09\0A", "r,r,r,r,r,r,r"(ptr %2, ptr %3, ptr %5, i64 %8, i64 %10, i64 %11, i32 1) #1, !srcloc !4
  tail call void asm sideeffect ".iLoop1${:uid}: \09\0Aso.v.dp.d u21, zero, p0 \09\0A.kloop1${:uid}: \09\0Aso.a.mul.fp u22, u1, u2, p0\0A\09so.a.add.fp u21, u21, u22, p0\0A\09so.b.ndc.1 u2, .kloop1${:uid} \0A\09so.a.adde.fp  u4, u21, p0 \0A\09so.b.nc\09u2, .iLoop1${:uid} \0A\09", ""() #1, !srcloc !5
  tail call void asm sideeffect "ss.sta.ld.d           u1, $0, $3, $5 \09\0Ass.app                u1, zero, $4, zero \09\0Ass.end                u1, zero, $5, $6 \09\0Ass.cfg.vec            u1                      \09\0Ass.sta.ld.d           u2, $1, $3, zero \09\0Ass.app                u2, zero, $4, $6 \09\0Ass.end                u2, zero, $5, $4 \09\0Ass.cfg.vec            u2                     \09\0Ass.sta.st.d           u4, $2, $3, $4 \09\0Ass.end                u4, zero, $4, $6 \09\0A", "r,r,r,r,r,r,r"(ptr %4, ptr %5, ptr %6, i64 %7, i64 %10, i64 %8, i32 1) #1, !srcloc !4
  tail call void asm sideeffect ".iLoop1${:uid}: \09\0Aso.v.dp.d u21, zero, p0 \09\0A.kloop1${:uid}: \09\0Aso.a.mul.fp u22, u1, u2, p0\0A\09so.a.add.fp u21, u21, u22, p0\0A\09so.b.ndc.1 u2, .kloop1${:uid} \0A\09so.a.adde.fp  u4, u21, p0 \0A\09so.b.nc\09u2, .iLoop1${:uid} \0A\09", ""() #1, !srcloc !5
  ret void
}

attributes #0 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+m,+relax,-d,-e,-experimental-xuve,-experimental-zawrs,-experimental-zca,-experimental-zcd,-experimental-zcf,-experimental-zihintntl,-experimental-ztso,-experimental-zvfh,-f,-h,-save-restore,-svinval,-svnapot,-svpbmt,-v,-xtheadvdot,-xventanacondops,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zdinx,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zicbom,-zicbop,-zicboz,-zihintpause,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64"}
!2 = !{i32 1, !"SmallDataLimit", i32 8}
!3 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project/ 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!4 = !{i64 190, i64 303, i64 385, i64 492, i64 571, i64 680, i64 765, i64 875, i64 959, i64 1073}
!5 = !{i64 1370, i64 1406, i64 1450, i64 1533, i64 1562, i64 1622, i64 1682, i64 1728, i64 1785}
