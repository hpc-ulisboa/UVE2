	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_f2p0_xuve0p1"
	.file	"kernel.c"
	.globl	core_kernel                     # -- Begin function core_kernel
	.p2align	1
	.type	core_kernel,@function
core_kernel:                            # @core_kernel
# %bb.0:
	addi	sp, sp, -144
	sd	ra, 136(sp)                     # 8-byte Folded Spill
	sd	s0, 128(sp)                     # 8-byte Folded Spill
	sd	s1, 120(sp)                     # 8-byte Folded Spill
	sd	s2, 112(sp)                     # 8-byte Folded Spill
	sd	s3, 104(sp)                     # 8-byte Folded Spill
	sd	s4, 96(sp)                      # 8-byte Folded Spill
	sd	s5, 88(sp)                      # 8-byte Folded Spill
	sd	s6, 80(sp)                      # 8-byte Folded Spill
	sd	s7, 72(sp)                      # 8-byte Folded Spill
	sd	s8, 64(sp)                      # 8-byte Folded Spill
	sd	s9, 56(sp)                      # 8-byte Folded Spill
	sd	s10, 48(sp)                     # 8-byte Folded Spill
	sd	s11, 40(sp)                     # 8-byte Folded Spill
	sd	a1, 0(sp)                       # 8-byte Folded Spill
	sd	a0, 32(sp)                      # 8-byte Folded Spill
	sd	a3, 16(sp)                      # 8-byte Folded Spill
	beqz	a3, .LBB0_9
# %bb.1:
	mv	s9, a5
	mv	s6, a4
	mv	s2, a2
	sd	zero, 24(sp)                    # 8-byte Folded Spill
	slli	s0, a4, 3
	slli	a0, a5, 3
	sd	a0, 8(sp)                       # 8-byte Folded Spill
	j	.LBB0_3
.LBB0_2:                                #   in Loop: Header=BB0_3 Depth=1
	ld	a1, 24(sp)                      # 8-byte Folded Reload
	addi	a1, a1, 1
	ld	a0, 32(sp)                      # 8-byte Folded Reload
	ld	a2, 8(sp)                       # 8-byte Folded Reload
	add	a0, a0, a2
	sd	a0, 32(sp)                      # 8-byte Folded Spill
	ld	a0, 16(sp)                      # 8-byte Folded Reload
	sd	a1, 24(sp)                      # 8-byte Folded Spill
	beq	a1, a0, .LBB0_9
.LBB0_3:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_6 Depth 2
                                        #       Child Loop BB0_8 Depth 3
	beqz	s6, .LBB0_2
# %bb.4:                                #   in Loop: Header=BB0_3 Depth=1
	li	s10, 0
	ld	a0, 24(sp)                      # 8-byte Folded Reload
	mul	s11, a0, s6
	ld	s7, 0(sp)                       # 8-byte Folded Reload
	j	.LBB0_6
.LBB0_5:                                #   in Loop: Header=BB0_6 Depth=2
	addi	s10, s10, 1
	addi	s7, s7, 8
	beq	s10, s6, .LBB0_2
.LBB0_6:                                #   Parent Loop BB0_3 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_8 Depth 3
	add	a0, s10, s11
	slli	a0, a0, 3
	add	s3, s2, a0
	sd	zero, 0(s3)
	beqz	s9, .LBB0_5
# %bb.7:                                # %.preheader
                                        #   in Loop: Header=BB0_6 Depth=2
	li	s1, 0
	ld	s5, 32(sp)                      # 8-byte Folded Reload
	mv	s4, s7
	mv	s8, s9
.LBB0_8:                                #   Parent Loop BB0_3 Depth=1
                                        #     Parent Loop BB0_6 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	ld	a0, 0(s5)
	ld	a1, 0(s4)
	call	__muldf3@plt
	mv	a1, a0
	mv	a0, s1
	call	__adddf3@plt
	mv	s1, a0
	sd	a0, 0(s3)
	addi	s8, s8, -1
	add	s4, s4, s0
	addi	s5, s5, 8
	bnez	s8, .LBB0_8
	j	.LBB0_5
.LBB0_9:
	ld	ra, 136(sp)                     # 8-byte Folded Reload
	ld	s0, 128(sp)                     # 8-byte Folded Reload
	ld	s1, 120(sp)                     # 8-byte Folded Reload
	ld	s2, 112(sp)                     # 8-byte Folded Reload
	ld	s3, 104(sp)                     # 8-byte Folded Reload
	ld	s4, 96(sp)                      # 8-byte Folded Reload
	ld	s5, 88(sp)                      # 8-byte Folded Reload
	ld	s6, 80(sp)                      # 8-byte Folded Reload
	ld	s7, 72(sp)                      # 8-byte Folded Reload
	ld	s8, 64(sp)                      # 8-byte Folded Reload
	ld	s9, 56(sp)                      # 8-byte Folded Reload
	ld	s10, 48(sp)                     # 8-byte Folded Reload
	ld	s11, 40(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 144
	ret
.Lfunc_end0:
	.size	core_kernel, .Lfunc_end0-core_kernel
                                        # -- End function
	.globl	core                            # -- Begin function core
	.p2align	1
	.type	core,@function
core:                                   # @core
# %bb.0:
	addi	sp, sp, -192
	sd	ra, 184(sp)                     # 8-byte Folded Spill
	sd	s0, 176(sp)                     # 8-byte Folded Spill
	sd	s1, 168(sp)                     # 8-byte Folded Spill
	sd	s2, 160(sp)                     # 8-byte Folded Spill
	sd	s3, 152(sp)                     # 8-byte Folded Spill
	sd	s4, 144(sp)                     # 8-byte Folded Spill
	sd	s5, 136(sp)                     # 8-byte Folded Spill
	sd	s6, 128(sp)                     # 8-byte Folded Spill
	sd	s7, 120(sp)                     # 8-byte Folded Spill
	sd	s8, 112(sp)                     # 8-byte Folded Spill
	sd	s9, 104(sp)                     # 8-byte Folded Spill
	sd	s10, 96(sp)                     # 8-byte Folded Spill
	sd	s11, 88(sp)                     # 8-byte Folded Spill
	ld	s9, 192(sp)
	ld	s1, 208(sp)
	sd	s1, 80(sp)                      # 8-byte Folded Spill
	sd	a6, 56(sp)                      # 8-byte Folded Spill
	sd	a5, 64(sp)                      # 8-byte Folded Spill
	sd	a4, 72(sp)                      # 8-byte Folded Spill
	sd	a3, 16(sp)                      # 8-byte Folded Spill
	sd	a2, 48(sp)                      # 8-byte Folded Spill
	sd	a1, 8(sp)                       # 8-byte Folded Spill
	slli	s11, s9, 3
	sd	a7, 32(sp)                      # 8-byte Folded Spill
	beqz	a7, .LBB1_9
# %bb.1:
	mv	s6, a0
	ld	s10, 200(sp)
	sd	zero, 40(sp)                    # 8-byte Folded Spill
	slli	a0, s10, 3
	sd	a0, 24(sp)                      # 8-byte Folded Spill
	j	.LBB1_3
.LBB1_2:                                #   in Loop: Header=BB1_3 Depth=1
	ld	a1, 40(sp)                      # 8-byte Folded Reload
	addi	a1, a1, 1
	ld	a0, 24(sp)                      # 8-byte Folded Reload
	add	s6, s6, a0
	ld	a0, 32(sp)                      # 8-byte Folded Reload
	sd	a1, 40(sp)                      # 8-byte Folded Spill
	beq	a1, a0, .LBB1_9
.LBB1_3:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_6 Depth 2
                                        #       Child Loop BB1_8 Depth 3
	beqz	s9, .LBB1_2
# %bb.4:                                #   in Loop: Header=BB1_3 Depth=1
	li	s2, 0
	ld	a0, 40(sp)                      # 8-byte Folded Reload
	mul	s3, a0, s9
	ld	s7, 8(sp)                       # 8-byte Folded Reload
	j	.LBB1_6
.LBB1_5:                                #   in Loop: Header=BB1_6 Depth=2
	addi	s2, s2, 1
	addi	s7, s7, 8
	beq	s2, s9, .LBB1_2
.LBB1_6:                                #   Parent Loop BB1_3 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_8 Depth 3
	add	a0, s2, s3
	slli	a0, a0, 3
	ld	s8, 72(sp)                      # 8-byte Folded Reload
	add	s8, s8, a0
	sd	zero, 0(s8)
	beqz	s10, .LBB1_5
# %bb.7:                                # %.preheader7
                                        #   in Loop: Header=BB1_6 Depth=2
	li	s1, 0
	mv	s0, s6
	mv	s5, s7
	mv	s4, s10
.LBB1_8:                                #   Parent Loop BB1_3 Depth=1
                                        #     Parent Loop BB1_6 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	ld	a0, 0(s0)
	ld	a1, 0(s5)
	call	__muldf3@plt
	mv	a1, a0
	mv	a0, s1
	call	__adddf3@plt
	mv	s1, a0
	sd	a0, 0(s8)
	addi	s4, s4, -1
	add	s5, s5, s11
	addi	s0, s0, 8
	bnez	s4, .LBB1_8
	j	.LBB1_5
.LBB1_9:
	ld	s8, 80(sp)                      # 8-byte Folded Reload
	slli	s8, s8, 3
	beqz	s9, .LBB1_18
# %bb.10:
	ld	s6, 216(sp)
	sd	zero, 40(sp)                    # 8-byte Folded Spill
	slli	a0, s6, 3
	sd	a0, 24(sp)                      # 8-byte Folded Spill
	j	.LBB1_12
.LBB1_11:                               #   in Loop: Header=BB1_12 Depth=1
	ld	a1, 40(sp)                      # 8-byte Folded Reload
	addi	a1, a1, 1
	ld	a0, 48(sp)                      # 8-byte Folded Reload
	ld	a2, 24(sp)                      # 8-byte Folded Reload
	add	a0, a0, a2
	sd	a0, 48(sp)                      # 8-byte Folded Spill
	sd	a1, 40(sp)                      # 8-byte Folded Spill
	beq	a1, s9, .LBB1_18
.LBB1_12:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_15 Depth 2
                                        #       Child Loop BB1_17 Depth 3
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	beqz	a0, .LBB1_11
# %bb.13:                               #   in Loop: Header=BB1_12 Depth=1
	li	s2, 0
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	ld	a1, 40(sp)                      # 8-byte Folded Reload
	mul	s3, a1, a0
	ld	s5, 16(sp)                      # 8-byte Folded Reload
	j	.LBB1_15
.LBB1_14:                               #   in Loop: Header=BB1_15 Depth=2
	addi	s2, s2, 1
	addi	s5, s5, 8
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	beq	s2, a0, .LBB1_11
.LBB1_15:                               #   Parent Loop BB1_12 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_17 Depth 3
	add	a0, s2, s3
	slli	a0, a0, 3
	ld	s7, 64(sp)                      # 8-byte Folded Reload
	add	s7, s7, a0
	sd	zero, 0(s7)
	beqz	s6, .LBB1_14
# %bb.16:                               # %.preheader3
                                        #   in Loop: Header=BB1_15 Depth=2
	li	s10, 0
	ld	s4, 48(sp)                      # 8-byte Folded Reload
	mv	s1, s5
	mv	s0, s6
.LBB1_17:                               #   Parent Loop BB1_12 Depth=1
                                        #     Parent Loop BB1_15 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	ld	a0, 0(s4)
	ld	a1, 0(s1)
	call	__muldf3@plt
	mv	a1, a0
	mv	a0, s10
	call	__adddf3@plt
	mv	s10, a0
	sd	a0, 0(s7)
	addi	s0, s0, -1
	add	s1, s1, s8
	addi	s4, s4, 8
	bnez	s0, .LBB1_17
	j	.LBB1_14
.LBB1_18:
	ld	a0, 32(sp)                      # 8-byte Folded Reload
	beqz	a0, .LBB1_27
# %bb.19:
	li	s4, 0
	j	.LBB1_21
.LBB1_20:                               #   in Loop: Header=BB1_21 Depth=1
	addi	s4, s4, 1
	ld	a0, 72(sp)                      # 8-byte Folded Reload
	add	a0, a0, s11
	sd	a0, 72(sp)                      # 8-byte Folded Spill
	ld	a0, 32(sp)                      # 8-byte Folded Reload
	beq	s4, a0, .LBB1_27
.LBB1_21:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_24 Depth 2
                                        #       Child Loop BB1_26 Depth 3
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	beqz	a0, .LBB1_20
# %bb.22:                               #   in Loop: Header=BB1_21 Depth=1
	li	s2, 0
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	mul	s3, s4, a0
	ld	s5, 64(sp)                      # 8-byte Folded Reload
	j	.LBB1_24
.LBB1_23:                               #   in Loop: Header=BB1_24 Depth=2
	addi	s2, s2, 1
	addi	s5, s5, 8
	ld	a0, 80(sp)                      # 8-byte Folded Reload
	beq	s2, a0, .LBB1_20
.LBB1_24:                               #   Parent Loop BB1_21 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_26 Depth 3
	add	a0, s2, s3
	slli	a0, a0, 3
	ld	s6, 56(sp)                      # 8-byte Folded Reload
	add	s6, s6, a0
	sd	zero, 0(s6)
	beqz	s9, .LBB1_23
# %bb.25:                               # %.preheader
                                        #   in Loop: Header=BB1_24 Depth=2
	li	s7, 0
	ld	s10, 72(sp)                     # 8-byte Folded Reload
	mv	s0, s5
	mv	s1, s9
.LBB1_26:                               #   Parent Loop BB1_21 Depth=1
                                        #     Parent Loop BB1_24 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	ld	a0, 0(s10)
	ld	a1, 0(s0)
	call	__muldf3@plt
	mv	a1, a0
	mv	a0, s7
	call	__adddf3@plt
	mv	s7, a0
	sd	a0, 0(s6)
	addi	s1, s1, -1
	add	s0, s0, s8
	addi	s10, s10, 8
	bnez	s1, .LBB1_26
	j	.LBB1_23
.LBB1_27:
	ld	ra, 184(sp)                     # 8-byte Folded Reload
	ld	s0, 176(sp)                     # 8-byte Folded Reload
	ld	s1, 168(sp)                     # 8-byte Folded Reload
	ld	s2, 160(sp)                     # 8-byte Folded Reload
	ld	s3, 152(sp)                     # 8-byte Folded Reload
	ld	s4, 144(sp)                     # 8-byte Folded Reload
	ld	s5, 136(sp)                     # 8-byte Folded Reload
	ld	s6, 128(sp)                     # 8-byte Folded Reload
	ld	s7, 120(sp)                     # 8-byte Folded Reload
	ld	s8, 112(sp)                     # 8-byte Folded Reload
	ld	s9, 104(sp)                     # 8-byte Folded Reload
	ld	s10, 96(sp)                     # 8-byte Folded Reload
	ld	s11, 88(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 192
	ret
.Lfunc_end1:
	.size	core, .Lfunc_end1-core
                                        # -- End function
	.ident	"clang version 16.0.6 (https://github.com/llvm/llvm-project/ 7cbf1a2591520c2491aa35339f227775f4d3adf6)"
	.section	".note.GNU-stack","",@progbits
