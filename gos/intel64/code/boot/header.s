section .multiboot_header
header_start:
	; magic number
	dd 0xe85250d6 ; multiboot2
	; architecture
	dd 0 ; protected mode i386
	; header length
	dd header_end - header_start
	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ;; information request
.info_request_start:
    dw 1
    dw 0
    dd 20
    dd 4        ;; request memory information (useful later)
    dd 6        ;; request memory map
    dd 8        ;; request framebuffer info
.info_request_end:

	; end tag
	dw 0
	dw 0
	dd 8
header_end: