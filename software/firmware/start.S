// Startup code for RISC-V
// Sets up stack pointer and calls main()

.section .text.start
.global _start
_start:
    // Set stack pointer to end of RAM
    // RAM is at 0x10000000, size is 4KB (0x1000)
    // Stack grows downward, so start at 0x10001000
    lui sp, %hi(0x10001000)
    addi sp, sp, %lo(0x10001000)
    
    // Call main
    jal ra, main
    
    // If main returns, loop forever
    _halt:
        j _halt

