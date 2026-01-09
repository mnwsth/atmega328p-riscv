// GPIO Input Test for ATmega328P RISC-V replica
// Reads from Port C (PC0) and mirrors state to Port B (PB0)

// GPIO Register Definitions
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

#define PINC  (*((volatile unsigned char*)0x20000026))
#define DDRC  (*((volatile unsigned char*)0x20000027))
#define PORTC (*((volatile unsigned char*)0x20000028))

int main(void) {
    // Configure PB0 as Output
    DDRB = 0x01;
    PORTB = 0x00;

    // Configure PC0 as Input (default, but being explicit)
    DDRC = 0x00;
    // Enable pull-up on PC0 (optional, but good practice if supported)
    // PORTC = 0x01; 

    // Configure PB7 as Output for heartbeat
    DDRB |= 0x80;

    while (1) {
        // Heartbeat: Toggle PB7
        PORTB ^= 0x80;
        
        // Read PC0
        unsigned char input_val = PINC & 0x01;
        
        // Write to PB0
        if (input_val) {
            PORTB |= 0x01;  // Set PB0 High
        } else {
            PORTB &= ~0x01; // Set PB0 Low
        }
        
        // Small delay to make heartbeat visible in simulation
        for (volatile int i = 0; i < 10; i++);
    }

    return 0;
}
