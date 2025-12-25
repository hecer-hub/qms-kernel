ENTRY(_start)
SECTIONS
{
    . = 0x00100000;
    
    .text : {
        KEEP(*(.multiboot))
        *(.text)
    }
    
    .rodata ALIGN(4096) : {
        *(.rodata)
    }
    
    .data ALIGN(4096) : {
        *(.data)
    }
    
    .bss ALIGN(4096) : {
        *(.bss)
        *(COMMON)
    }
    
    .modules ALIGN(4096) : {
        __modules_start = .;
        *(.module)
        . = ALIGN(4096);
        __modules_end = .;
    }

    _end = .; 
    end = .;
}
