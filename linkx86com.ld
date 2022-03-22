ENTRY(_start32)
SECTIONS
{
    . = 1M;
    .header :
    {
        KEEP(*(.hdr))
    }

    .text :
    {
        KEEP(*(.text))
    }

    .bss :
    {
        KEEP(*(.bss))
    }

    .data :
    {
        KEEP(*(.data))
    }
}