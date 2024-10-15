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
	rdinstret	a2

	#NO_APP
	lui	a3, %hi(start)
	sd	a2, %lo(start)(a3)
	blez	a1, .LBB0_2
# %bb.1:                                # %.loopexit3
	addiw	a2, a1, 1
	li	a4, 1
	#APP
	ss.sta.ld.d.v	u4, a0
	ss.app	u4, zero, a1, a1
	ss.app.mod.siz.inc.2	u4, a4
	ss.app	u4, zero, zero, zero
	ss.app.mod.siz.inc.3	u4, a4
	ss.end	u4, zero, zero, a4
	ss.sta.ld.d.v	u5, a0
	ss.app	u5, zero, a1, zero
	ss.app.mod.siz.inc.2	u5, a4
	ss.app	u5, zero, zero, a4
	ss.app.mod.siz.inc.3	u5, a4
	ss.end	u5, zero, zero, a1
	ss.sta.ld.d	u16, a0
	ss.app	u16, zero, a1, a1
	ss.app.mod.siz.inc.2	u16, a4
	ss.end	u16, zero, zero, a4
	ss.sta.st.d	u1, a0
	ss.app	u1, zero, a1, a1
	ss.app.mod.siz.inc.2	u1, a4
	ss.end	u1, zero, zero, a4
	ss.sta.ld.d	u8, a0
	ss.app	u8, zero, a1, a1
	ss.app.mod.siz.inc.2	u8, a4
	ss.end	u8, zero, zero, a4
	ss.sta.ld.d	u9, a0
	ss.app	u9, zero, a1, zero
	ss.app.mod.siz.inc.2	u9, a4
	ss.end	u9, zero, zero, a2
	ss.sta.st.d	u6, a0
	ss.app	u6, zero, a1, a1
	ss.app.mod.siz.inc.2	u6, a4
	ss.end	u6, zero, zero, a4
	ss.sta.ld.d.v	u13, a0
	ss.app	u13, zero, a1, a1
	ss.app.mod.ofs.inc.2	u13, a4
	ss.app.mod.siz.dec.2	u13, a4
	ss.app.mod.siz.inc.3	u13, a4
	ss.app	u13, zero, a1, zero
	ss.end	u13, zero, zero, a4
	ss.sta.ld.d.v	u14, a0
	ss.app	u14, zero, a1, zero
	ss.app.mod.ofs.inc.2	u14, a4
	ss.app.mod.siz.dec.2	u14, a4
	ss.app.mod.siz.inc.3	u14, a4
	ss.app	u14, zero, a1, a4
	ss.end	u14, zero, zero, a1
	ss.sta.st.d.v	u10, a0
	ss.app	u10, zero, a1, a1
	ss.app.mod.ofs.inc.2	u10, a4
	ss.app.mod.siz.dec.2	u10, a4
	ss.app.mod.siz.inc.3	u10, a4
	ss.app	u10, zero, a1, a4
	ss.end	u10, zero, zero, zero
.SLOOP_1:
.SLOOP_1_0:
	so.v.dp.d	u2, zero, p0
.SLOOP_1_0_0:
	so.a.mul.fp	u3, u4, u5, p0
	so.a.sub.fp	u2, u2, u3, p0
	so.b.ndc.3	u4, .SLOOP_1_0_0
	so.a.adde.fp	u3, u2, p0
	so.a.add.fp	u1, u16, u3, p0
	so.a.div.fp	u6, u8, u9, p0
	so.b.ndc.2	u1, .SLOOP_1_0
.SLOOP_1_1:
	so.a.mul.fp	u12, u13, u14, p0
	so.a.sub.fp	u10, u11, u12, p0
	so.b.ndc.2	u10, .SLOOP_1_1
	so.b.nc	u1, .SLOOP_1

	#NO_APP
.LBB0_2:
	#APP
	rdinstret	a2

	#NO_APP
	ld	a1, %lo(start)(a3)
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
