ENTRY(_start32)

SECTIONS
{
    .header ALIGN(8) : {
        KEEP(*(header))
    }
    .text ALIGN(8) : {
        KEEP(*(*.text *.text.*))
    }
    .data ALIGN(8) : {
        KEEP(*(*.data *.data.*))
    }

    .bss ALIGN(8) : {
        KEEP(*(*.bss *.bss.*))
    }

    .rodata ALIGN(8) : {
        KEEP(*(*.bss *.bss.*))
    }
}