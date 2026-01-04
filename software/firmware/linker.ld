/* Linker script for ATmega328P RISC-V replica */

MEMORY
{
    ROM (rx) : ORIGIN = 0x00000000, LENGTH = 64K
    RAM (rw) : ORIGIN = 0x10000000, LENGTH = 4K
}

SECTIONS
{
    .text : {
        *(.text.start)
        *(.text*)
    } > ROM
    
    .rodata : {
        *(.rodata*)
    } > ROM
    
    .data : {
        _data_start = .;
        *(.data*)
        _data_end = .;
    } > RAM AT > ROM
    
    .bss : {
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } > RAM
    
    _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}

