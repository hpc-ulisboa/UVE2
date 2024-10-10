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
	rdinstret	a5

	#NO_APP
	lui	a6, %hi(start)
	sd	a5, %lo(start)(a6)
	beqz	a3, .LBB0_2
# %bb.1:                                # %.preheader3
	li	a5, 1
	#APP
	ss.sta.ld.d.v	u2, a0
	ss.end	u2, zero, a3, a5
	ss.sta.st.d.v	u1, a2
	ss.end	u1, zero, a3, a5
.SLOOP_4:
	so.v.mv	u1, u2, p0
	so.b.nc	u1, .SLOOP_4

	#NO_APP
	fmv.x.d	a4, fa0
	#APP
	ss.sta.ld.d.v	u3, a2
	ss.end	u3, zero, a3, a5
	ss.sta.st.d.v	u1, a1
	ss.end	u1, zero, a3, a5
	so.v.dp.d	u4, a4, p0
.SLOOP_3:
	so.a.mul.fp	u1, u3, u4, p0
	so.b.nc	u1, .SLOOP_3

	#NO_APP
	#APP
	ss.sta.ld.d.v	u3, a0
	ss.end	u3, zero, a3, a5
	ss.sta.ld.d.v	u4, a1
	ss.end	u4, zero, a3, a5
	ss.sta.st.d.v	u1, a2
	ss.end	u1, zero, a3, a5
.SLOOP_2:
	so.a.add.fp	u1, u3, u4, p0
	so.b.nc	u1, .SLOOP_2

	#NO_APP
	#APP
	ss.sta.ld.d.v	u3, a1
	ss.end	u3, zero, a3, a5
	ss.sta.ld.d.v	u5, a2
	ss.end	u5, zero, a3, a5
	ss.sta.st.d.v	u1, a0
	ss.end	u1, zero, a3, a5
	so.v.dp.d	u6, a4, p0
.SLOOP_1:
	so.a.mul.fp	u4, u5, u6, p0
	so.a.add.fp	u1, u3, u4, p0
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
