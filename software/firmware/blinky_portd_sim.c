// GPIO Port D LED blinky program for ATmega328P RISC-V replica (SIMULATION VERSION)
// Uses a much shorter delay for simulation purposes

// GPIO Port D register addresses (matching ATmega328P memory map)
#define PIND  (*((volatile unsigned char*)0x20000029))
#define DDRD  (*((volatile unsigned char*)0x2000002A))
#define PORTD (*((volatile unsigned char*)0x2000002B))

// Simple delay function - SHORT DELAY FOR SIMULATION
void delay(unsigned int count) {
    volatile unsigned int i;
    for (i = 0; i < count; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    // Configure PD0 as output
    DDRD = 0x01;  // Set bit 0 of DDRD to 1 (output)
    PORTD = 0x00; // Initialize PORTD to 0

    // Main loop - toggle LED on PD0
    while (1) {
        PORTD ^= 0x01;  // Toggle bit 0 (XOR with 0x01)
        delay(100);     // VERY SHORT delay for simulation (100 cycles)
    }

    return 0;
}


