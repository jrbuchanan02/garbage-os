global init_ram
extern error        ;; allow erroring out here
section .text
bits 32

init_ram:
    ;; pointer to mbi should still be in edi
    mov ebx, edi                ;; put pointer back into ebx
    ;; first step, parse to find memory structure information
    mov ecx, [ebx]              ;; size of memory structure
    cmp ecx, 8                  ;; ensure that we have a size > 8 bytes
    jb .insane_mbi


    ;; ecx now stores the size of the mbi structure.
    ;; now what we have to do is parse to find the total size of 
    ;; usable RAM and an appropriate place to store the page table.

    ;; point ecx, ebx to the first tag

    add ebx, 8
    sub ecx, 8

.search_loop:
    cmp dword [ebx], 6                ;; check if type is 6 (memory information structure)
    jne .skip                   ;; if not, skip this entry

    mov esi, ebx                ;; point esi to size and entry size of memory information
    add esi, 4                  ;; to reset after loops, we can mov ebx, esi - 4

    add ebx, 16                 ;; 16 bytes from type to first entry
    sub ecx, 16

    cmp ecx, 0
    je .insane_mbi              ;; no entries!

    jmp init_memory

.skip:
    sub ecx, [ebx + 4]          ;; move ebx, ecx along
    add ebx, [ebx + 4]

    mov eax, ebx
    and eax, 7                  ;; find how much more to move for 8-byte alignment

    sub ecx, eax
    add ebx, eax                ;; move them along, if already 8-byte aligned, then no movement occurs

    cmp ecx, 0                  ;; check if no more space left

    je .insane_mbi              ;; if no more tags left, insane mbi
    jne .search_loop            ;; if more tags left, keep searching



.insane_mbi:
    mov al, 'I'                 ;; I for insane
    jmp error
.no_room:
    mov al, 'R'                 ;; R for Room
    jmp error
.partly_initialized:
    mov al, 'P'                 ;; P for partly
    jmp error

verify_page_table_location:
    ;; if upper 32-bits of address are all 0's
    ;; and memory type is 1
    ;; and the entry can fit an address aligned to 4 KiB
    ;; of a size of 4 KiB, then leave 1 in eax
    ;;
    ;; if any of these are not true, then leave 0 in eax
    ;;
    ;; ebx is presumed to point to the beginning of the entry
    ;; 
    ;; eax and edx are clobbered.

    mov eax, dword [ebx]

    ;; entry beyond 32-bit addressing
    cmp eax, 0x00000000
    jne .return_false


    ;; entry smaller than 4KiB
    mov eax, dword [ebx + 12]
    cmp eax, 0x1000
    jl .return_false


    ;; entry is not RAM
    mov eax, dword [ebx + 16]
    cmp eax, 1
    jne .return_false

    ;; entry cannot have a 4 KiB aligned address with a page table
    mov eax, dword [ebx + 4]
    and eax, 0x0fff
    mov edx, 0x1000
    sub edx, eax            ;; find out what we need to add to eax to get the next 4KiB aligned address

    mov eax, dword [ebx + 12]   ;; check that our size beyond the next 4KiB aligned address
    sub eax, edx                ;; is greater than 4kiB
    cmp eax, 0x1000
    jl .return_false            ;; if less than, return false

    ;; edx still holds amount to add to eax
    mov eax, dword [ebx + 4 ]   ;; move lower 32-bits
    add eax, edx                ;; if this operation carries, then, part of 
    jc .return_false            ;; the page table will be outsize of RAM that
                                ;; we can address

    jmp .return_true


.return_false:
    mov eax, 0x00000000
    ret
.return_true:
    mov eax, 0x00000001
    ret

init_memory:
    ;; when entering:
    ;; 
    ;;  ebx points to first entry
    ;;  ecx holds remaining size in all tags
    ;;  edi points to start of tags structure
    ;;  esi points to the size entry of the memory map structure
    ;;  ebp is still undefined
    ;;  esp holds stack
    ;;
    ;;  registers we can safely edit:
    ;;      eax
    ;;      edx
    ;; 

    ;; each of these entries is a 64-bit address - a bit interesting since we are in 32-bits!
    ;; thus, we need to enter in only 32-bit addresses for our page tables!

    ;; since we are on an x64 processor, we can safely assume a 4-level page table
    ;; our first task is to find 4KiB of type 1 ram that is 4KiB aligned for the page table


    ;; iterate through all entries until finding one where verify_page_table_location returns true

.l4_loop:
    ;; ecx already holds the first entry, so we should check that first
    call verify_page_table_location
    cmp eax, 1                      ;; if we returned true
    je .l4_mem_init                 ;; initialize as l4 table
    ;; otherwise
    add ebx, dword [esi + 4]        ;; add entry size to ebx
    sub ecx, dword [esi + 4]        ;; move along

    cmp ecx, 0
    je init_mem.no_room             ;; if no more tags left, no more room

    jmp .l4_loop
.l4_mem_init:
    ;; initialize the value we found into CR3

    mov eax, dword [ecx + 4]        ;; lower 32 bits of memory address -> eax
    and eax, 0x0fff                 ;; mask out for alignment
    mov edx, 0x1000
    sub edx, eax                    ;; get number to add to align eax

    mov eax, dword [ecx + 4]        ;; get address back into eax
    add eax, edx                    ;; address of our l4 page table

    mov cr3, eax                    ;; put that address as the page table
    mov word [l3_page_table_count], 0   ;; no l3 page tables (yet)
    
    mov ebx, dword [esi]            ;; almost reset value of ebx
    mov ecx, dword [edi]            ;; almost reset size of ecx

    add ebx, 12                     ;; move ebx to the first entry
    mov edx, ebx                    ;; copy ebx into edx
    sub edx, ecx                    ;; find out how much we have moved
    add ecx, edx                    ;; move ecx along

.l3_loop:
    ;; find the l3 page table.
    ;; the caveat here, is that other page tables exist in RAM
    ;; moreover, their address may not necessarily be the address
    ;; that is listed in cr3

    ;; at this point here are our important registers:
    ;;  
    ;;  eax -> address of l4 page table
    ;;  ebx -> first memory map entry
    ;;  ecx -> remaining size in all tags
    ;;  edx -> difference between tags size and ecx
    ;;  esi -> points to size of memory map tag
    ;;  edi -> points to beginning of information structure
    ;;  esp -> stack
    ;;  ebp -> undefined
    ;;  cr3 -> address of l4 page table
    ;;  
    ;;  l3_page_table_count -> 0
    
    call verify_page_table_location
    cmp eax, 1                      ;; check if we have a valid position

    je .l3_mem_collision            ;; if valid, ensure that tables do not
                                    ;; collide
.l3_otherwise:
    ;; otherwise
    add ebx, dword [esi + 4]        ;; add entry size to ebx
    sub ecx, dowrd [esi + 4]        ;; move along

    cmp ecx, 0
    je init_mem.partly_initialized  ;; different error, we were able to initialize
                                    ;; the l4 table.
    jmp .l3_loop
.l3_mem_collision:
    ;; check if the found memory entry is not yet taken
    ;; if it is taken, go to the otherwise part of the l3_loop

    mov eax, dword [ecx + 4]        ;; lower 32 bits of memory address -> eax
    and eax, 0x0fff                 ;; mask out for alignment
    mov edx, 0x1000
    sub edx, eax                    ;; get number to add to align eax

    mov eax, dword [ecx + 4]        ;; get address back into eax
    add eax, edx                    ;; address of our page table

    ;; regardless of the value of l3_page_table_count, we want to 
    ;; ensure that this is not the value of cr3
    xchg eax, edx
    mov eax, cr3        ;; address of l4 page table
    cmp eax, edx        ;; check if equal
    jne .l3_mem_collision_loop_enter
    ;; however, if they are equal, we want to see if we can fit both!
    ;; we can do this by looking at the size of the address when aligned
    ;; to 4096 bytes

    ;; this code is copy-pase from verify_page_table_location
    ;; however, we want to check if there is 8 KiB of space.
    mov eax, dword [ebx + 4]
    and eax, 0x0fff
    mov edx, 0x1000
    sub edx, eax            

    mov eax, dword [ebx + 12]   
    sub eax, edx                
    cmp eax, 0x2000     
    jge .l3_mem_collision_loop_enter  ;; want to make sure that we haven't
                                ;; already put a l3 page table after the
                                ;; l4 page table
.l3_mem_collision_loop_enter:
    ;; set our "i" to zero
    mov word [l3_page_table_loop_iterator], 0
.l3_mem_collision_loop:
    mov ax, [l3_page_table_count]
section .bss
l3_page_table_count: resw 1     ;; reserve 2 bytes for l3 page table index
l3_page_table_loop_iterator: resw 1 ;; reserve 2 bytes for our "i" in the loop