global stack
section .multiboot_header
header_start:
	; magic number
	dd 0xe85250d6 ; multiboot2
	; architecture
	dd 0 ; protected mode i386
	; header length
	dd header_end - header_start
	; checksum
	dd -(0xe85250d6 + 0 + (header_end - header_start))

    ;; information request
.info_request_start:
    dw 1
    dw 0
    dd header_start.info_request_end - header_start.info_request_start
    dd 4
    dd 6
    ;;dd 8

.info_request_end:
	dw 0
	dw 0
	dd 8
header_end:
section .rodata
stack:
	resb 0x4000