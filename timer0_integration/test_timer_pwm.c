#include <stdint.h>

#define TCCR0A (*((volatile uint8_t *)0x20000044))
#define TCCR0B (*((volatile uint8_t *)0x20000045))
#define OCR0A  (*((volatile uint8_t *)0x20000047))

// Calibrated delay for 100 MHz PicoRV32
// Assuming ~4 clock cycles per loop iteration:
// 1 millisecond = 100,000 clock cycles = 47 loop iterations
void delay_ms(uint32_t ms) {
    volatile uint32_t i;
    uint32_t iterations = ms * 47;
    for (i = 0; i < iterations; i++) { 
        __asm__ __volatile__("nop"); 
    }
}

int main() {
    // Configure Fast PWM Mode (WGM = 011)
    // Clear OC0A on Compare Match, set OC0A at BOTTOM (COM0A = 10)
    TCCR0A = 0x83; 
    
    // Set Prescaler to clk/64 (CS = 011)
    // PWM Frequency = 100MHz / (64 * 256) = ~6.1 kHz
    TCCR0B = 0x03; 
    
    uint8_t brightness = 0;
    int8_t fade_amount = 1;

    while (1) {
        OCR0A = brightness; // Update hardware duty cycle

        brightness += fade_amount;
        
        // Reverse fading direction at the limits
        if (brightness == 0 || brightness == 255) {
            fade_amount = -fade_amount; 
        }

        // Delay 6 milliseconds per step.
        // 255 steps * 6 ms = ~1.53 seconds for a full fade in or fade out.
        delay_ms(6); 
    }
    return 0;
}
