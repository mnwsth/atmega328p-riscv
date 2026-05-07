// Port B Registers (Outputs - Chip Selects)
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// Port C Registers (Inputs - Data Switches)
#define PINC  (*((volatile unsigned char*)0x20000026))
#define DDRC  (*((volatile unsigned char*)0x20000027))
#define PORTC (*((volatile unsigned char*)0x20000028))

// Port D Registers (Inputs - Target Selector)
#define PIND  (*((volatile unsigned char*)0x20000029))
#define DDRD  (*((volatile unsigned char*)0x2000002A))
#define PORTD (*((volatile unsigned char*)0x2000002B))

// SPI Registers
#define SPCR  (*((volatile unsigned char*)0x2000004C))
#define SPSR  (*((volatile unsigned char*)0x2000004D))
#define SPDR  (*((volatile unsigned char*)0x2000004E))

#define SPIF  (1 << 7)
#define SPE   (1 << 6)
#define DORD  (1 << 5) 
#define MSTR  (1 << 4)
#define SPR1  (1 << 1) // Clock Rate Select Bit 1
#define SPR0  (1 << 0) // Clock Rate Select Bit 0

// Pin Definitions
#define PIN_CS_RP2040 7 // Port B, Bit 2
#define PIN_CS_ESP32  3 // Port B, Bit 3
#define PIN_TARGET_SW 7 // Port D, Bit 0

void delay() {
    for (volatile int i = 0; i < 50000; i++) {
        __asm__ __volatile__("nop");
    }
}

int main(void) {
    // 1. Configure Chip Selects as outputs
    DDRB |= (1 << PIN_CS_RP2040) | (1 << PIN_CS_ESP32);
    
    // 2. Initialize both CS lines HIGH (Inactive)
    PORTB |= (1 << PIN_CS_RP2040) | (1 << PIN_CS_ESP32); 
    
    // 3. Configure Data and Target switches as inputs
    DDRC = 0x00; 
    DDRD &= ~(1 << PIN_TARGET_SW);
    
    // 4. Enable SPI Master, LSB-first, and set clock to fosc / 128
    SPCR = SPE | MSTR |  SPR1 | SPR0;
    
    unsigned char portc_input;
    unsigned char target_switch;
    unsigned char transmitted_data;
    unsigned char dummy;
    
    while (1) {
        portc_input = PINC;
        target_switch = PIND;
        
        transmitted_data = portc_input % 8;
        
        // 5. Assert the correct Chip Select based on Port D switch
        if (target_switch & (1 << PIN_TARGET_SW)) {
            // Switch is HIGH: Target ESP32
            PORTB &= ~(1 << PIN_CS_ESP32);
        } else {
            // Switch is LOW: Target RP2040
            PORTB &= ~(1 << PIN_CS_RP2040);
        }
        
        // 6. Transmit Data
        SPDR = transmitted_data;
        while (!(SPSR & SPIF));
        
        // Clear flags
        dummy = SPSR;
        dummy = SPDR;
        (void)dummy; 
        
        // Hold time delay for Verilog trailing edge sync
        for(volatile int i = 0; i < 20; i++) {
            __asm__ __volatile__("nop");
        }
        
        // 7. Deassert ALL devices by pulling both CS lines HIGH
        PORTB |= (1 << PIN_CS_RP2040) | (1 << PIN_CS_ESP32);
        
        delay();
    }
    
    return 0;
}
