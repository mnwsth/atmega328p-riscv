// GPIO Port B and C program for ATmega328P RISC-V replica
// Takes input from Port B, computes (input % 5)!, outputs to Port C

// GPIO Port register addresses
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define DDRC  (*((volatile unsigned char*)0x20000027))
#define PORTC (*((volatile unsigned char*)0x20000028))

void delay(unsigned int count) {
    volatile unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 100; j++) {
            __asm__ __volatile__("nop");
        }
    }
}


int fact(int num){
    int f = 1;
    for(int i=num;i>0;i--){
        f *= i;
    }
    return f;
}

int main(void) {
    // Configure Port B as input (0x00) and Port C as output (0xFF)
    DDRB = 0x00; 
    DDRC = 0xFF; 
    
    // Initialize PORTC to 0
    PORTC = 0x00; 

    // Main loop 
    while (1) {
        // Read input and compute modulo 5
        unsigned char mod_val = PINB % 6;
        
        // Output the precomputed factorial to PORTC
        PORTC = fact(mod_val); 
        delay(100000);
    }

    return 0;
}