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
    cmp [ebx], 6                ;; check if type is 6 (memory information structure)

    jne .skip                   ;; if not, skip initializing memory


    mov esi, ebx                ;; point esi to size and entry size of memory information
    add esi, 4                  ;; to reset after loops, we can mov ebx, esi - 4

    add ebx, 16                 ;; 16 bytes from type to first entry
    sub ecx, 16

    cmp ecx, 0
    je .insane_mbi              ;; no entries!

    ;; each of these entries is a 64-bit address - a bit interesting since we are in 32-bits!
    ;; thus, we need to enter these into a page table as we get them.

    ;; since we are on an x64 processor, we can safely assume a 4-level page table
    ;; our first task is to find 4KiB of type 1 ram that is 4KiB aligned for the page table

.find_l4:
    ;; check if entry type is 1 (RAM)

    cmp [ebx + 16], 1
    jne .continue_l4_search

    ;; check if entry length >= 0x1000

    mov eax, [ebx + 12]  ;; check lower 32 bits
    cmp eax, 0x1000

    jg .check_potential_alignment

    mov eax, [ebx + 8]  ;; check upper 32 bits
    cmp eax, 0x0000
    jl .continue_l4_search  ;; if not enough room, then don't initialize

.check_potential_alignment:
    ;; if base address is aligned to 4KiB, then definitely

    mov eax, [ebx + 4]  ;; lower 32 bits of address

    and eax, 0x0fff     ;; mask out all other bits

    jz .init_l4_page    ;; initialize l4 page table if our beginning address is 4 KiB aligned

    ;; if not, we need to load the length and check to see if
    ;; the next 4 KiB aligned address is, in fact, available for 
    ;; a page table

    mov edx, 0x1000     ;; move the 4KiB value into edx
    sub edx, eax        ;; find the amount to add to the start address to give
                        ;; a 4 KiB aligned address

    mov eax, [ebx + 12]  ;; move the lower 32 bits of the length into eax
    sub eax, edx
    cmp eax, 0          ;; check if value < 0
    jnb .init_l4_page ;; if not below 0, initialize l4 page

    ;; if below 0, check that subtracting 1 from the upper bits of the length gives 0
    mov eax, [ebx + 8]  ;; move the upper 32 bits of the length into eax
    dec eax             ;; decrement eax
    jz .continue_l4_search  ;; no 4 KiB aligned address
.init_l4_page:
    ;; get the 4KiB aligned address again
    mov eax, [ebx + 4]  ;; ensure lower 32 bits in eax
    mov edx, 0x1000     ;; smallest 4KiB aligned address
    and eax, 0x0fff     ;; mask out all of the lower bits

    sub edx, eax        ;; get the amount that we have to add
    
    mov eax, [ebx + 4]  ;; get the lower 32-bits back
    add eax, edx        ;; align our address

    mov dword [perm_page_table_addresses.l4 + 4], eax       ;; move eax's address into our l4 page table's permanent address

    mov eax, [ebx + 0]  ;; get the upper 32-bits of address
    jnc .straight_init_l4_page  ;; if not carry from previous add, just put it in
    inc eax             ;; add one to the address
.straight_init_l4_page:
    mov dword [perm_page_table_addresses.l4 + 0], eax       ;; move the upper 32 bits
    jmp .initialize_l3_tables                               ;; initialize the l3 page tables

.continue_l4_search:
    add ebx, [esi + 4]  ;; add entry size to ebx
    sub ecx, [esi + 4]  ;; subtract entry size from ecx
    mov eax, esi        ;; move the original address from esi to eax
    sub eax, 4          ;; adjust
    mov ebx, edx        ;; copy ebx into edx so that we do not corrupt it
    sub edx, eax        ;; compare the difference in start and now
    cmp edx, [esi]      ;; check if it is equal to the total size
    je .no_room         ;; if they are equal, and no room for l4 page table - end here
    jne .find_l4        ;; if not, we already point to the next entry

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

.initialize_l3_tables:
    mov dword [l3_page_table_count], 0  ;; our current amount of l3 page tables is 0
    ;; I want to initialize 4 l3 page tables on all systems:
    ;; - RAM
    ;; - ACPI
    ;; - Preserved
    ;; - Reserved / Broken

    ;; At the l3 stage, all 4 * k page tables are present
    ;; however, at the l1 level, Reserved / Broken memory is not present ;)
    ;; preserved memory will be read-only (for now)
    ;; ACPI will be read / write, but with systems permissions
    ;; RAM will be read, write, and global

    ;; first step in finding the l3 page tables, is to find a 4KiB aligned address that is not the l4
    ;; page table's address
    mov ebx, esi
    sub ebx, 4  ;; reset ebx
    
    ;; the caveat here is that there may now be multiple page tables where an entry says there is space!
    ;; we easily know where the l4 page table is (we remember that address)
    ;; 
    ;; To find the l3 tables, we need to search the l4 page table

.l3_search_loop:
    


.insane_mbi:
    mov al, 'I'                 ;; I for insane
    jmp error
.no_room:
    mov al, 'R'                 ;; R for Room
    jmp error

section .bss
l3_page_table_count: resw 1     ;; reserve 2 bytes for l3 page table index
perm_page_table_addresses:
.l4: resb 8
