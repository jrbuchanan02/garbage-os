/*
 * File: pe32p.ld
 * Author: Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * Brief: Implements a tailored version of the PE32+ format for use in EFI 
 * Firmware environments.
 * Version: 1
 * Date: 2023-04-16
 * Copyright: This file is part of Garbage OS. Garbage OS is Copyright (C) 2023
 * Joshua Buchanan and published under the LGPL license version 2.1. You are 
 * free to use this source code in your project even if your code is not open 
 * source. This file also implements a tailored version of the PE32+ format, 
 * which is intellectual property of Microsoft.
 * This file contains a linker-script implementation of the PE32+ executible 
 * format used by the Windows Operating system and (important to Garbage OS)
 * the EFI standard. This file is designed exclusively for use with an operating
 * system loader and cannot be guaranteed easily adapted to anything else.
 * 
 */

/*
 * I'm not sure this has any effect since we're telling the linker to output a 
 * binary file, but the entry point for all programs that run in an efi enviornment
 * is efi_main
 */
ENTRY(efi_main)
/*
 * the linker thinks it's outputting a binary file, so it doesn't put in any 
 * headers that would interfere with the ones we're manually placing in the file
 */
OUTPUT_FORMAT(binary)
/*
 * Linker scripts can't encode character literals, so this file defines some for
 * us. Makes the script (more) human readable without the use of an ASCII / UTF-8
 * table.
 */
INCLUDE source/build/link/ascii.ld
/*
 * Amount of sections in the script, changing this number means more code since
 * we would manually have to edit the section header.
 *
 * DO NOT TOUCH! ¡NO TOQUE!
 */
sections = 3;
/*
 * We teach the linker to lie and say it made the file 1970-1-1, or, in my local
 * time, 1969-12-31 (at 6:00 PM). Alternatively, this could also be some time in
 * the 22nd century (when the 32-bit time_t rolls back over to zero), whichever's closer.
 */
time = 0;
/*
 * special variable for deprecated fields that have to be zero. If a field is 
 * actually zero, it is not a zero_field.
 */
zero_field = 0;
/* bitflags that mean the following:
 * 0x0002 -> this file was linked without erro (LD will not output if it fails)
 * 0x0020 -> can handle addresses beyond 2 GiB
 * 0x0200 -> "no debug info"
 * 0x0400 -> if on removable media, bring into ram / swap
 * 0x0800 -> if from network, bring into ram / swap
 * 0x1000 -> not a user application (important since code assumes ring 0)
 */
characteristics = 0x0002 | 0x0020 | 0x0200 | 0x0400 | 0x0800 | 0x1000;
/*
 * bitflags that say this section is a code segment.
 * 0x00000020 -> this section contains code
 * 0x20000000 -> this section is executible (not kidding, they seem to mean the same thing)
 * 0x40000000 -> this section is readable.
 */
code_segment_characteristics = 0x00000020 | 0x20000000 | 0x40000000;

/*
 * bitflags that say this section is a data segment.
 * 0x00000040 -> this section contains initialized data
 * 0x40000000 -> this section can be read
 * 0x80000000 -> this section can be written to
 */
data_segment_characteristics = 0x00000040 | 0x40000000 | 0x80000000;

/*
 * bitflags that say this section is a bss segment.
 * 0x00000080 -> this section contains uninitialized data
 * 0x40000000 -> this section can be read.
 * 0x80000000 -> this section can be written to.
 */
bss_segment_characteristics = 0x00000080 | 0x400000000 | 0x80000000;

SECTIONS {
    . = 0;
    file_start = .;
    reqh_section = .;
    opth_section = reqh_section + SIZEOF(.required_header);
    sech_section = opth_section + SIZEOF(.optional_header);
    data_section = sech_section + SIZEOF(.section_header);
    text_section = data_section + SIZEOF(.data);
    bss_section  = text_section + SIZEOF(.text);

    optional_header_size = SIZEOF(.optional_header);
    code_section_size = SIZEOF(.text);
    data_section_size = SIZEOF(.data);
    bss_section_size = SIZEOF(.bss);
    
    .required_header : 
    {
        BYTE(M) /* All EXE / PE / etc. files must start with the letters "MZ" */
        BYTE(Z) /* the mysterious letters that simply happen to be the creator's initials.*/
        /*
         * From the MZ header to offset 3C should be a DOS 2 program that simply
         * outputs "this program cannot run in DOS mode". However, since that 
         * requires extra effort, and possibly convincing GCC to link real mode
         * assembly with something that's not real-mode assembly, possibly quite
         * a bit of extra effort for code that's customizeable and not going to
         * be used. Feel free, however, to add in a program at a later date as
         * (if DOS could find the partition) you could try to run the EFI loader
         * on a DOS machine.
         */
        . = 0x3C;
        /*
         * Align the PE signature on a 16-byte boundary. This is not required, 
         * but will make the header always start at offset 0x40, which might make
         * debugging this script (which I have already done) easier.
         */
        offset = NEXT(0x10);
        BYTE(offset);
        . = offset;
        /*
         * The PE signature is "PE" (in ASCII) followed by two null characters.
         */
        BYTE(P)
        BYTE(E)
        BYTE(nul)
        BYTE(nul)
        /*
         * Machine type -- who can run this code, as defined by the file that 
         * includes this one.
         */
        SHORT(machine)
        /*
         * Section count: it's three. Data, Code, Read-only Data.
         */
        SHORT(sections)
        /*
         * The lower 32-bits of the amount of seconds since when time_t was
         * zero. Choosing the closer of the two closest dates to time_t == 0,
         * this file will be linked some time in the next ten/twenty years.
         */
        LONG(time)
        /* symbol table pointer -- deprecated */
        LONG(zero_field)
        /* number of symbols -- deprecated */
        LONG(zero_field)
        /* size of the optional header */
        SHORT(optional_header_size)
        /* characteristics -- important things */
        SHORT(characteristics)
    } = 0xF4 /* fill with halt instruction, a valid (if messy) DOS 2 application*/
    /* 
     * force the current offset to be where it should be. If anything sneaky 
     * tries to put itself after the required header, the linker will error out
     * since we forced the optional header to overwrite the sneaky bugger.
     */
    . = SIZEOF(.required_header);
    /*
     * Microsoft calls this the optional header. It's not optional. 
     */
    .optional_header : 
    {
        /* 
         * this optional header begins with the magic number. 0x020B, hard-coded
         * in little-endian (so the 0x0B appears first on purpose) 
         */
        BYTE(0x0B)
        BYTE(0x02)
        /*
         * Linker's major version. This field is for book-keeping purposes, this
         * script is set to always report version 0.0
         */
        BYTE(0)
        /*
         * Linker's minor version. This field is for book-keeping purposes, this
         * script is set to always report version 0.0
         */
        BYTE(0)
        /*
         * Report how much code is in the file.
         */
        LONG(code_section_size)
        /*
         * Report how much initialized data is in the file.
         */
        LONG(data_section_size)
        /*
         * Report how much uninitialized data is in the file.
         */
        LONG(bss_section_size)
        /*
         * "The address of the entry point relative to the image base when the 
         * executible file is loaded into memory." 
         *
         * I believe this is Microsoft-ese for "offset of the entry point in the
         * file". If I'm wrong, this field must be zero instead since we want 
         * efi_main to be at both the beginning of the code and the beginning of
         * the file when in memory.
         */
        LONG(efi_main)
        /*
         * "The address that is relative to the image base at the beginning-of-code
         * section when it is loaded into memory."
         *
         * I believe this is Microsoft-ese for "where in the file does the first
         * code section start."
         */
        LONG(text_section)
        /* 
         * Where in memory we want our image loaded. Like most systems, Garbage
         * OS wants to load at the top of the address space. Unlike most systems
         * Garbage OS looks at its memory map upside-down, so we actually request
         * to be placed at the bottom.
         */
        QUAD(0x0)
        LONG(0x1000)
        LONG(0x1000)
        SHORT(0)
        SHORT(0)
        SHORT(0)
        SHORT(1)
        SHORT(2)
        SHORT(10)
        LONG(0)
        image_size = file_end - file_start;
        LONG(image_size)
        header_sizes = SIZEOF(.required_header) + SIZEOF(.optional_header) + SIZEOF(.section_header);
        to_round_up = 0x1000 - header_sizes & 0xFFF;
        rounded_header_sizes = header_sizes + to_round_up;
        LONG(rounded_header_sizes);
        LONG(0) /* checksum, may have to be filled in later somehow? */
        SHORT(10) /* EFI application*/
        SHORT(0) /* no DLL characteristics, since this is not a DLL*/
        QUAD(128K) /* reserve this much stack (EFI requires at least this much) */
        QUAD(128K) /* commit this much stack (EFI requires at least this much) */
        QUAD(0) /* there is no heap, so no heap is reserved */
        QUAD(0) /* there is no heap, so no heap is committed */
        LONG(0) /* reserved */
        LONG(0) /* no RVA tables.*/
    }
    
    .section_header :
    {
        dot_text_name_start = .;
        /* an 8-byte long string that pads with zeros if it has less than eight
         * characters. For the text section, this string is ".text".
         */
        BYTE(period)
        BYTE(letter_t)
        BYTE(letter_e)
        BYTE(letter_x)
        BYTE(letter_t)
        FILL(0x00)
        /*
         * Fill 8 - dot_text_name_start zeros. Using this zero count instead of 
         * NEXT(8) to allow the section header to be aligned on any boundary.
         */
        . += 3;
        FILL(0xF4)
        text_section_size = SIZEOF(.text);
        LONG(text_section_size)
        LONG(text_section)
        /* size of the text section on disk. */
        to_round_up_text_section_size = 0x1000 - (text_section_size & 0xFFF);
        rounded_text_section_size = text_section_size + to_round_up_text_section_size;
        LONG(rounded_text_section_size)
        LONG(text_section)
        LONG(0) /* no relocations */
        LONG(zero_field) /* line numbers -- deprecated */
        LONG(0) /* no relocations */
        LONG(zero_field) /* line numbers count -- deprecated */
        LONG(code_segment_characteristics)
        dot_data_name_start = .;
        BYTE(period)
        BYTE(letter_d)
        BYTE(letter_a)
        BYTE(letter_t)
        BYTE(letter_a)
        FILL(0x00)
        . += 3;
        FILL(0xF4)
        data_section_size = SIZEOF(.data);
        LONG(data_section_size)
        LONG(data_section)
        to_round_up_data_section_size = 0x1000 - (data_section_size & 0xFFF);
        rounded_data_section_size = data_section_size + to_round_up_data_section_size;
        LONG(rounded_data_section_size)
        LONG(data_section)
        LONG(0) /* disallow relocating the data section */
        LONG(zero_field) /* line numbers -- deprecated */
        LONG(0) /* no relocations, since we disallowed them*/
        LONG(zero_field) /* line numbers count -- deprecated */
        LONG(data_segment_characteristics)
        dot_bss_name_start = .;
        BYTE(period)
        BYTE(letter_b)
        BYTE(letter_s)
        BYTE(letter_s)
        FILL(0x00)
        . += 4;
        FILL(0xF4)
        bss_section_size = SIZEOF(.bss);
        LONG(bss_section_size)
        LONG(bss_section)
        to_round_up_bss_section_size = 0x1000 - (bss_section_size & 0xFFF);
        rounded_bss_section_size = bss_section_size + to_round_up_bss_section_size;
        LONG(rounded_bss_section_size)
        LONG(bss_section)
        LONG(0) /* disallow relocating the bss section */
        LONG(zero_field) /* line numbers -- deprecated */
        LONG(0) /* no relocations, since we disallowed them*/
        LONG(zero_field) /* line numbers count -- deprecated*/
        LONG(bss_segment_characteristics)
    } 

    .data : 
    { 
        *(*.data);
        *(*.rodata);
    }

    .text : 
    {
        efi_main = .;
        *(.text.efi_main);
        *(*.text);
        *(*.text LMA);
        *(*.text.*);
        *(*.eh_frame);
    }

    .bss :
    {
        *(*.bss);
    }

    file_end = .;

    /DISCARD/ : 
    {
        *(.note.gnu.property);
    }
}