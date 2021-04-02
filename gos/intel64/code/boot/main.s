global start

section .text
bits 32
start:
	; print `OK`
	mov dword [0xb8000], 0x2f4b2f4f
    mov eax, nmi_handler
.loop:
    mov ecx, 0
    mov dword [idt + ecx * 8], eax
    cmp ecx, 0x100
    jb .loop
    lidt [idt]
	hlt


nmi_handler:
    iret

section .bss

idt: resb 0x8 * 0x100
