#include <stdint.h>

#define DDRB   (*((volatile uint8_t *)0x20000024))
#define PORTB  (*((volatile uint8_t *)0x20000025))
#define TCCR0A (*((volatile uint8_t *)0x20000044))
#define TCCR0B (*((volatile uint8_t *)0x20000045))
#define TCNT0  (*((volatile uint8_t *)0x20000046))

int main() {
    DDRB = 0xFF;  // Set Port B as all outputs
    PORTB = 0x00; // Turn off LEDs

    // Normal mode
    TCCR0A = 0x00; 
    
    // External clock source on T0 pin, rising edge (CS = 111)
    TCCR0B = 0x07; 

    while (1) {
        // Continuously copy the hardware timer count to the LEDs
        PORTB = TCNT0;
    }
    return 0;
}
