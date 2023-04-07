.set IRQ_BASE, 0x20

.section .text
.extern _ZN16InterruptManager15handleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN16InterruptManager16HandleExceptionEv
_ZN16InterruptManager16HandleExceptionEv:
	movb $\num, (interruptnumber)
	jmp set_esp_and_invoke_handleInterrupt
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp set_esp_and_invoke_handleInterrupt
.endm


HandleInterruptRequest 0x00
HandleInterruptRequest 0x01


set_esp_and_invoke_handleInterrupt:
pusha
pushl %ds
pushl %es
pushl %fs
pushl %gs

pushl %esp
push (interruptnumber)
call _ZN16InterruptManager15handleInterruptEhj
movl %eax, %esp

popl %gs
popl %fs
popl %es
popl %ds
popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:

iret

.data
	interruptnumber: .byte 0
