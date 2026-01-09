// GPIO Port C LED blinky program for ATmega328P RISC-V replica
// Toggles GPIO Port C, Pin 0 (PC0) to blink an LED

// GPIO Port C register addresses (matching ATmega328P memory map)
#define PINC  (*((volatile unsigned char*)0x20000026))
#define DDRC  (*((volatile unsigned char*)0x20000027))
#define PORTC (*((volatile unsigned char*)0x20000028))

// Simple delay function (approximate)
// This creates a delay loop - adjust count based on clock frequency
void delay(unsigned int count) {
    volatile unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 100; j++) {
            __asm__ __volatile__("nop");
        }
    }
}

int main(void) {
    // Configure PC0 as output
    DDRC = 0x01;  // Set bit 0 of DDRC to 1 (output)
    PORTC = 0x00; // Initialize PORTC to 0

    // Main loop - toggle LED on PC0
    while (1) {
        PORTC ^= 0x01;  // Toggle bit 0 (XOR with 0x01)
        delay(1000000); // Delay
    }

    return 0;
}
