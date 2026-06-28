	.file	"strategy_spec.cpp"
	.text
	.section	.text._ZN4csot8Strategy7on_initEv,"axG",@progbits,_ZN4csot8Strategy7on_initEv,comdat
	.align 2
	.p2align 4
	.weak	_ZN4csot8Strategy7on_initEv
	.type	_ZN4csot8Strategy7on_initEv, @function
_ZN4csot8Strategy7on_initEv:
.LFB2821:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE2821:
	.size	_ZN4csot8Strategy7on_initEv, .-_ZN4csot8Strategy7on_initEv
	.section	.text._ZN13strategy_spec7on_fillERKN4csot5OrderEdj,"axG",@progbits,_ZN13strategy_spec7on_fillERKN4csot5OrderEdj,comdat
	.align 2
	.p2align 4
	.weak	_ZN13strategy_spec7on_fillERKN4csot5OrderEdj
	.type	_ZN13strategy_spec7on_fillERKN4csot5OrderEdj, @function
_ZN13strategy_spec7on_fillERKN4csot5OrderEdj:
.LFB2846:
	.cfi_startproc
	movq	16(%rsi), %rax
	movsbl	3(%rax), %eax
	subl	$48, %eax
	cltq
	imulq	$544, %rax, %rax
	addq	%rax, %rdi
	movl	72(%rdi), %eax
	leal	(%rdx,%rax), %ecx
	subl	%edx, %eax
	cmpb	$0, (%rsi)
	cmove	%ecx, %eax
	movl	%eax, 72(%rdi)
	ret
	.cfi_endproc
.LFE2846:
	.size	_ZN13strategy_spec7on_fillERKN4csot5OrderEdj, .-_ZN13strategy_spec7on_fillERKN4csot5OrderEdj
	.section	.text._ZN13strategy_specD2Ev,"axG",@progbits,_ZN13strategy_specD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN13strategy_specD2Ev
	.type	_ZN13strategy_specD2Ev, @function
_ZN13strategy_specD2Ev:
.LFB3162:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE3162:
	.size	_ZN13strategy_specD2Ev, .-_ZN13strategy_specD2Ev
	.weak	_ZN13strategy_specD1Ev
	.set	_ZN13strategy_specD1Ev,_ZN13strategy_specD2Ev
	.section	.text._ZN13strategy_specD0Ev,"axG",@progbits,_ZN13strategy_specD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN13strategy_specD0Ev
	.type	_ZN13strategy_specD0Ev, @function
_ZN13strategy_specD0Ev:
.LFB3164:
	.cfi_startproc
	movl	$2224, %esi
	jmp	_ZdlPvm
	.cfi_endproc
.LFE3164:
	.size	_ZN13strategy_specD0Ev, .-_ZN13strategy_specD0Ev
	.section	.text._ZN13strategy_spec7on_tickERKN4csot4TickE,"axG",@progbits,_ZN13strategy_spec7on_tickERKN4csot4TickE,comdat
	.align 2
	.p2align 4
	.weak	_ZN13strategy_spec7on_tickERKN4csot4TickE
	.type	_ZN13strategy_spec7on_tickERKN4csot4TickE, @function
_ZN13strategy_spec7on_tickERKN4csot4TickE:
.LFB2823:
	.cfi_startproc
	movq	%rdi, %r8
	movq	%rdx, %rdi
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	16(%rdx), %rax
	movsd	24(%rdi), %xmm5
	movsd	32(%rdi), %xmm4
	movsbl	3(%rax), %edx
	movapd	%xmm5, %xmm0
	addsd	%xmm4, %xmm0
	mulsd	.LC0(%rip), %xmm0
	subl	$48, %edx
	movslq	%edx, %rdx
	imulq	$544, %rdx, %rax
	movapd	%xmm0, %xmm3
	leaq	(%rsi,%rax), %r9
	leaq	48(%rsi,%rax), %r10
	mulsd	%xmm0, %xmm3
	movl	64(%r9), %ecx
	movapd	%xmm0, %xmm1
	movl	68(%r9), %eax
	movupd	(%r10), %xmm2
	unpcklpd	%xmm3, %xmm1
	cmpl	$63, %ecx
	ja	.L9
	imulq	$68, %rdx, %r11
	addpd	%xmm2, %xmm1
	movq	%rbx, 64(%rsp)
	.cfi_offset 3, -16
	movl	%eax, %ebx
	addl	$1, %eax
	addl	$1, %ecx
	andl	$63, %eax
	leaq	10(%rbx,%r11), %r11
	movsd	%xmm0, (%rsi,%r11,8)
	movl	%ecx, 64(%r9)
	movups	%xmm1, (%r10)
	movl	%eax, 68(%r9)
	cmpl	$64, %ecx
	je	.L32
	movq	64(%rsp), %rbx
	.cfi_restore 3
.L10:
	pxor	%xmm0, %xmm0
	movq	$0, 16(%r8)
	movq	%r8, %rax
	movups	%xmm0, (%r8)
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	.cfi_restore_state
	imulq	$68, %rdx, %rcx
	movl	%eax, %r11d
	addl	$1, %eax
	andl	$63, %eax
	leaq	10(%r11,%rcx), %rcx
	movsd	(%rsi,%rcx,8), %xmm3
	movsd	%xmm0, (%rsi,%rcx,8)
	movapd	%xmm3, %xmm6
	mulsd	%xmm3, %xmm6
	unpcklpd	%xmm6, %xmm3
	subpd	%xmm3, %xmm2
	addpd	%xmm1, %xmm2
	movapd	%xmm2, %xmm7
	movups	%xmm2, (%r10)
	movapd	%xmm2, %xmm1
	movl	%eax, 68(%r9)
	unpckhpd	%xmm7, %xmm7
	movapd	%xmm7, %xmm3
.L12:
	movsd	.LC1(%rip), %xmm2
	mulsd	%xmm2, %xmm1
	mulsd	%xmm3, %xmm2
	movapd	%xmm1, %xmm3
	mulsd	%xmm1, %xmm3
	subsd	%xmm3, %xmm2
	movsd	.LC2(%rip), %xmm3
	comisd	%xmm2, %xmm3
	ja	.L10
	imulq	$544, %rdx, %rdx
	subsd	%xmm1, %xmm0
	movl	72(%rsi,%rdx), %eax
	movapd	%xmm0, %xmm1
	mulsd	%xmm0, %xmm1
	testl	%eax, %eax
	jne	.L14
	mulsd	.LC3(%rip), %xmm2
	comisd	%xmm2, %xmm1
	jb	.L10
	pxor	%xmm1, %xmm1
	xorl	%eax, %eax
	comisd	%xmm1, %xmm0
	jb	.L16
	movapd	%xmm5, %xmm4
	movl	$1, %eax
.L16:
	movdqu	8(%rdi), %xmm0
	movq	%r8, 8(%rsp)
	movb	%al, 16(%rsp)
	movl	$1, 48(%rsp)
	movups	%xmm0, 24(%rsp)
	movsd	%xmm4, 40(%rsp)
.L30:
	movl	$40, %edi
	call	_Znwm
	movq	8(%rsp), %r8
	movdqa	16(%rsp), %xmm0
	leaq	40(%rax), %rdx
	movq	%rax, (%r8)
	movq	%rdx, 16(%r8)
	movups	%xmm0, (%rax)
	movdqa	32(%rsp), %xmm0
	movups	%xmm0, 16(%rax)
	movq	48(%rsp), %rcx
	movq	%rcx, 32(%rax)
	movq	%r8, %rax
	movq	%rdx, 8(%r8)
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L14:
	.cfi_restore_state
	mulsd	.LC5(%rip), %xmm2
	jle	.L18
	comisd	%xmm1, %xmm2
	jb	.L10
	movdqu	8(%rdi), %xmm0
	movq	%r8, 8(%rsp)
	movb	$1, 16(%rsp)
	movups	%xmm0, 24(%rsp)
	movsd	%xmm5, 40(%rsp)
.L31:
	movl	%eax, 48(%rsp)
	jmp	.L30
	.p2align 4,,10
	.p2align 3
.L18:
	comisd	%xmm1, %xmm2
	jb	.L10
	movdqu	8(%rdi), %xmm0
	movq	%r8, 8(%rsp)
	negl	%eax
	movb	$0, 16(%rsp)
	movups	%xmm0, 24(%rsp)
	movsd	%xmm4, 40(%rsp)
	jmp	.L31
.L32:
	.cfi_offset 3, -16
	movapd	%xmm1, %xmm7
	movq	64(%rsp), %rbx
	.cfi_restore 3
	unpckhpd	%xmm7, %xmm7
	movapd	%xmm7, %xmm3
	jmp	.L12
	.cfi_endproc
.LFE2823:
	.size	_ZN13strategy_spec7on_tickERKN4csot4TickE, .-_ZN13strategy_spec7on_tickERKN4csot4TickE
	.text
	.p2align 4
	.globl	create_strategy
	.type	create_strategy, @function
create_strategy:
.LFB2847:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$2224, %edi
	call	_Znwm
	movl	$2224, %edx
	xorl	%esi, %esi
	movq	%rax, %rdi
	call	memset
	movapd	.LC7(%rip), %xmm0
	movq	$_ZTV13strategy_spec+16, (%rax)
	movq	%rax, %rcx
	movb	$64, 8(%rax)
	movups	%xmm0, 16(%rax)
	movl	$257, %eax
	movw	%ax, 32(%rcx)
	movq	.LC9(%rip), %rax
	movq	%rax, 40(%rcx)
	movq	%rcx, %rax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2847:
	.size	create_strategy, .-create_strategy
	.weak	_ZTSN4csot8StrategyE
	.section	.rodata._ZTSN4csot8StrategyE,"aG",@progbits,_ZTSN4csot8StrategyE,comdat
	.align 16
	.type	_ZTSN4csot8StrategyE, @object
	.size	_ZTSN4csot8StrategyE, 17
_ZTSN4csot8StrategyE:
	.string	"N4csot8StrategyE"
	.weak	_ZTIN4csot8StrategyE
	.section	.rodata._ZTIN4csot8StrategyE,"aG",@progbits,_ZTIN4csot8StrategyE,comdat
	.align 8
	.type	_ZTIN4csot8StrategyE, @object
	.size	_ZTIN4csot8StrategyE, 16
_ZTIN4csot8StrategyE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSN4csot8StrategyE
	.weak	_ZTS13strategy_spec
	.section	.rodata._ZTS13strategy_spec,"aG",@progbits,_ZTS13strategy_spec,comdat
	.align 16
	.type	_ZTS13strategy_spec, @object
	.size	_ZTS13strategy_spec, 16
_ZTS13strategy_spec:
	.string	"13strategy_spec"
	.weak	_ZTI13strategy_spec
	.section	.rodata._ZTI13strategy_spec,"aG",@progbits,_ZTI13strategy_spec,comdat
	.align 8
	.type	_ZTI13strategy_spec, @object
	.size	_ZTI13strategy_spec, 24
_ZTI13strategy_spec:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTS13strategy_spec
	.quad	_ZTIN4csot8StrategyE
	.weak	_ZTV13strategy_spec
	.section	.rodata._ZTV13strategy_spec,"aG",@progbits,_ZTV13strategy_spec,comdat
	.align 8
	.type	_ZTV13strategy_spec, @object
	.size	_ZTV13strategy_spec, 56
_ZTV13strategy_spec:
	.quad	0
	.quad	_ZTI13strategy_spec
	.quad	_ZN13strategy_specD1Ev
	.quad	_ZN13strategy_specD0Ev
	.quad	_ZN4csot8Strategy7on_initEv
	.quad	_ZN13strategy_spec7on_tickERKN4csot4TickE
	.quad	_ZN13strategy_spec7on_fillERKN4csot5OrderEdj
	.set	.LC0,.LC7+8
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1066401792
	.align 8
.LC2:
	.long	-774749268
	.long	1009939037
	.align 8
.LC3:
	.long	0
	.long	1074790400
	.align 8
.LC5:
	.long	0
	.long	1070596096
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC7:
	.long	0
	.long	1073741824
	.long	0
	.long	1071644672
	.section	.rodata.cst8
	.align 8
.LC9:
	.long	-400107883
	.long	1041313291
	.ident	"GCC: (GNU) 16.1.0"
	.section	.note.GNU-stack,"",@progbits
