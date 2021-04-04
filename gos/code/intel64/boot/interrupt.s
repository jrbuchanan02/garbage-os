global interrupt_table, init_idt

section .text64
bits 64

init_idt:

    mov al, 0x1b ;; ← for "GO BACK!"

    ;;lidt interrupt_table

    ret



section .bss:
interrupt_table: resb 8 * 255