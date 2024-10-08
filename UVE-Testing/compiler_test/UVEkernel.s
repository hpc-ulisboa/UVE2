	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_f2p0_d2p0_xuve0p1"
	.file	"kernel.c"
	.globl	core                            # -- Begin function core
	.p2align	1
	.type	core,@function
core:                                   # @core
# %bb.0:
	#APP
	rdinstret	a7

	#NO_APP
	lui	a6, %hi(start)
	sd	a7, %lo(start)(a6)
	blez	a4, .LBB0_2
# %bb.1:                                # %.loopexit1
	slli	a7, a5, 32
	srli	a7, a7, 32
	li	t0, 1
	#APP
	ss.sta.ld.d.v	u4, a0
	ss.app	u4, zero, a4, a5
	ss.end	u4, zero, a7, t0
	ss.sta.ld.w.inds	u6, a1
	ss.app	u6, zero, a4, a5
	ss.end	u6, zero, a7, t0
	ss.sta.ld.d.v	u5, a2
	ss.app	u5, zero, a4, zero
	ss.app	u5, zero, a7, zero
	ss.end.ind.ofs.add.1	u5, u6
	ss.sta.ld.d	u8, a3
	ss.end	u8, zero, a4, t0
	ss.sta.st.d	u1, a3
	ss.end	u1, zero, a4, t0
.SLOOP_1:
	so.v.dp.d	u2, zero, p0
.SLOOP_1_0:
	so.a.mul.fp	u3, u4, u5, p0
	so.a.add.fp	u2, u2, u3, p0
	so.b.ndc.1	u4, .SLOOP_1_0
	so.a.adde.fp	u3, u2, p0
	so.a.add.fp	u1, u8, u3, p0
	so.b.nc	u1, .SLOOP_1

	#NO_APP
.LBB0_2:
	#APP
	rdinstret	a2

	#NO_APP
	ld	a1, %lo(start)(a6)
	lui	a3, %hi(end)
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	sd	a2, %lo(end)(a3)
	tail	printf
.Lfunc_end0:
	.size	core, .Lfunc_end0-core
                                        # -- End function
	.type	start,@object                   # @start
	.section	.sbss,"aw",@nobits
	.globl	start
	.p2align	3
start:
	.quad	0                               # 0x0
	.size	start, 8

	.type	end,@object                     # @end
	.globl	end
	.p2align	3
end:
	.quad	0                               # 0x0
	.size	end, 8

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%ld\n%ld\n"
	.size	.L.str, 9

	.ident	"clang version 16.0.6 (https://github.com/llvm/llvm-project/ 7cbf1a2591520c2491aa35339f227775f4d3adf6)"
	.section	".note.GNU-stack","",@progbits
