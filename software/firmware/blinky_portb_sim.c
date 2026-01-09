// GPIO Port B LED blinky program for ATmega328P RISC-V replica (SIMULATION VERSION)
// Uses a much shorter delay for simulation purposes

// GPIO Port B register addresses (matching ATmega328P memory map)
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// Simple delay function - SHORT DELAY FOR SIMULATION
void delay(unsigned int count) {
    volatile unsigned int i;
    for (i = 0; i < count; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    // Configure PB0 as output
    DDRB = 0x01;  // Set bit 0 of DDRB to 1 (output)
    PORTB = 0x00; // Initialize PORTB to 0

    // Main loop - toggle LED on PB0
    while (1) {
        PORTB ^= 0x01;  // Toggle bit 0 (XOR with 0x01)
        delay(100);     // VERY SHORT delay for simulation (100 cycles)
    }

    return 0;
}
