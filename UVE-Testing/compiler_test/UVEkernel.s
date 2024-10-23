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
	blez	a0, .LBB0_2
# %bb.1:                                # %.loopexit
	fmv.x.d	a7, fa0
	fmv.x.d	t0, fa1
	li	t1, 1
	#APP
	ss.sta.st.d	u1, a3
	ss.end	u1, zero, a0, t1
	ss.sta.st.d	u3, a5
	ss.end	u3, zero, a0, t1
	ss.sta.ld.d.v	u7, a1
	ss.app	u7, zero, a0, a0
	ss.end	u7, zero, a0, t1
	ss.sta.ld.d.v	u8, a4
	ss.app	u8, zero, a0, zero
	ss.end	u8, zero, a0, t1
	ss.sta.ld.d	u24, a3
	ss.end	u24, zero, a0, t1
	ss.sta.st.d	u4, a3
	ss.end	u4, zero, a0, t1
	ss.sta.ld.d.v	u13, a2
	ss.app	u13, zero, a0, a0
	ss.end	u13, zero, a0, t1
	ss.sta.ld.d.v	u14, a4
	ss.app	u14, zero, a0, zero
	ss.end	u14, zero, a0, t1
	ss.sta.ld.d	u26, a5
	ss.end	u26, zero, a0, t1
	ss.sta.ld.d	u19, a3
	ss.end	u19, zero, a0, t1
	ss.sta.st.d	u16, a5
	ss.end	u16, zero, a0, t1
	so.v.mvsv.d	u22, t0
	so.v.mvsv.d	u20, a7
.SLOOP_1:
	so.v.mvsv.d	u1, zero
	so.v.mvsv.d	u3, zero
	so.v.dp.d	u5, zero, p0
	so.v.dp.d	u11, zero, p0
.SLOOP_1_0:
	so.a.mul.fp	u6, u7, u8, p0
	so.a.add.fp	u5, u6, u5, p0
	so.a.mul.fp	u12, u13, u14, p0
	so.a.add.fp	u11, u12, u11, p0
	so.b.ndc.2	u7, .SLOOP_1_0
	so.a.adde.fp	u12, u11, p0
	so.a.add.fp	u25, u12, u26, p0
	so.a.adde.fp	u6, u5, p0
	so.a.add.fp	u4, u6, u24, p0
	so.a.mul.fp	u18, u19, u20, p0
	so.a.mul.fp	u21, u25, u22, p0
	so.a.add.fp	u16, u18, u21, p0
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
