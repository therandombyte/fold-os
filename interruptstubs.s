.set IRQ_BASE, 0x20

.section .text
.extern _ZN16InterruptManager15handleInterruptEhj

.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

HandleException 0x00
HandleException 0x01
HandleInterruptRequest 0x00
HandleInterruptRequest 0x01


int_bottom:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	pushl %esp
	push (interruptnumber)
	call _ZN16InterruptManager15handleInterruptEhj
	mov %eax, %esp

	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa

.global _ZN16InterruptManager22IgnoreInterruptRequestEv
_ZN16InterruptManager22IgnoreInterruptRequestEv:

	iret

.data
	interruptnumber: .byte 0
