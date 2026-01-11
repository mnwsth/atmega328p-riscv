// Memory Map for ATmega328P RISC-V Replica
// Based on ATmega328P datasheet memory organization

// Program Memory (Flash equivalent)
`define MEM_ROM_BASE    32'h00000000
`define MEM_ROM_SIZE    32'h00010000  // 64KB
`define MEM_ROM_MASK    32'hFFFF0000

// Data Memory (SRAM equivalent)
`define MEM_RAM_BASE    32'h10000000
`define MEM_RAM_SIZE    32'h00001000  // 4KB
`define MEM_RAM_MASK    32'hFFFFF000

// I/O Space (Peripherals)
`define MEM_IO_BASE     32'h20000000
`define MEM_IO_SIZE     32'h00001000  // 4KB
`define MEM_IO_MASK     32'hFFFFF000

// GPIO Port B Registers (matching ATmega328P addresses)
`define GPIOB_PINB      32'h20000023  // PINB - Port B Input Pins
`define GPIOB_DDRB      32'h20000024  // DDRB - Port B Data Direction Register
`define GPIOB_PORTB     32'h20000025  // PORTB - Port B Data Register

// GPIO Port D Registers (matching ATmega328P addresses)
`define GPIOD_PIND      32'h20000029  // PIND - Port D Input Pins
`define GPIOD_DDRD      32'h2000002A  // DDRD - Port D Data Direction Register
`define GPIOD_PORTD     32'h2000002B  // PORTD - Port D Data Register

// GPIO Port C Registers (matching ATmega328P addresses)
`define GPIOC_PINC      32'h20000026  // PINC - Port C Input Pins
`define GPIOC_DDRC      32'h20000027  // DDRC - Port C Data Direction Register
`define GPIOC_PORTC     32'h20000028  // PORTC - Port C Data Register

// Analog Comparator Registers
`define ACSR            32'h20000050  // ACSR - Analog Comparator Control and Status Register

// Timer/Counter 0 Registers (matching ATmega328P addresses)
`define TIMER0_TIFR0    32'h20000035  // Timer Interrupt Flag Register
`define TIMER0_TCCR0A   32'h20000044  // Timer Control Register A
`define TIMER0_TCCR0B   32'h20000045  // Timer Control Register B
`define TIMER0_TCNT0    32'h20000046  // Timer Counter Register
`define TIMER0_OCR0A    32'h20000047  // Output Compare Register A
`define TIMER0_OCR0B    32'h20000048  // Output Compare Register B
`define TIMER0_TIMSK0   32'h2000006E  // Timer Interrupt Mask Register

