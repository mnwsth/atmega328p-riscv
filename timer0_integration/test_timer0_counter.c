#include <stdint.h>

#define DDRB   (*((volatile uint8_t *)0x20000024))
#define PORTB  (*((volatile uint8_t *)0x20000025))
#define TIFR0  (*((volatile uint8_t *)0x20000035))
#define TCCR0A (*((volatile uint8_t *)0x20000044))
#define TCCR0B (*((volatile uint8_t *)0x20000045))

int main() {
    // 1. Configure GPIO: Set Port B Pin 0 as output
    DDRB |= 0x01;   
    PORTB &= ~0x01;

    // 2. Configure Timer0
    TCCR0A = 0x00;  // Normal mode
    TCCR0B = 0x05;  // Prescaler = 1024 (CS02=1, CS01=0, CS00=1)

    uint16_t overflow_count = 0;

    while (1) {
        // Poll TOV0 flag (Bit 0 of TIFR0)
        if (TIFR0 & 0x01) {
            // Clear flag by writing '1'
            TIFR0 = 0x01;
            overflow_count++;

            // Toggle LED after ~190 overflows (approx 0.5 seconds at 100MHz)
            if (overflow_count >= 190) {
                PORTB ^= 0x01;
                overflow_count = 0;
            }
        }
    }
    return 0;
}
