// Simple LED blinky program for ATmega328P RISC-V replica
// This program toggles GPIO Port B, Pin 0 (PB0) to blink an LED

// GPIO register addresses (matching ATmega328P memory map)
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// Simple delay function (approximate)
// This creates a delay loop - adjust count based on clock frequency
void delay(unsigned int count) {
    volatile unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 100; j++) {
            // Nested loop for better delay resolution
            __asm__ __volatile__("nop");
        }
    }
}

// Main program
int main(void) {
    // Configure PB0 as output
    DDRB = 0x01;  // Set bit 0 of DDRB to 1 (output)
    PORTB = 0x00; // Initialize PORTB to 0
    
    // Main loop - toggle LED
    while (1) {
        PORTB ^= 0x01;  // Toggle bit 0 (XOR with 0x01)
        delay(1000000); // Delay
    }
    
    return 0;
}

