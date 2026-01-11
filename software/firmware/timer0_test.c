// Timer0 Integration Test for ATmega328P RISC-V replica
// Tests Timer0 functionality via firmware running on the SoC

// GPIO Port B register addresses
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// Timer0 register addresses (ATmega328P compatible)
#define TIFR0  (*((volatile unsigned char*)0x20000035))
#define TCCR0A (*((volatile unsigned char*)0x20000044))
#define TCCR0B (*((volatile unsigned char*)0x20000045))
#define TCNT0  (*((volatile unsigned char*)0x20000046))
#define OCR0A  (*((volatile unsigned char*)0x20000047))
#define OCR0B  (*((volatile unsigned char*)0x20000048))
#define TIMSK0 (*((volatile unsigned char*)0x2000006E))

// Clock select values
#define CS_STOP    0
#define CS_DIV1    1
#define CS_DIV8    2
#define CS_DIV64   3
#define CS_DIV256  4
#define CS_DIV1024 5

// TIFR0 bits
#define TOV0  (1 << 0)
#define OCF0A (1 << 1)
#define OCF0B (1 << 2)

// TIMSK0 bits
#define TOIE0  (1 << 0)
#define OCIE0A (1 << 1)
#define OCIE0B (1 << 2)

// Test result indicators on PORTB
// PB0 = Test 1 pass, PB1 = Test 2 pass, etc.
// All LEDs on = all tests passed

// Test status
static unsigned char test_results = 0;

// Signal test pass on specific bit
void signal_pass(unsigned char test_bit) {
    test_results |= (1 << test_bit);
    PORTB = test_results;
}

// Wait for overflow flag with timeout
int wait_overflow(unsigned int timeout) {
    while (timeout > 0) {
        if (TIFR0 & TOV0) {
            TIFR0 = TOV0;  // Clear flag by writing 1
            return 1;  // Success
        }
        timeout--;
    }
    return 0;  // Timeout
}

// Wait for compare match A flag with timeout
int wait_compare_a(unsigned int timeout) {
    while (timeout > 0) {
        if (TIFR0 & OCF0A) {
            TIFR0 = OCF0A;  // Clear flag by writing 1
            return 1;  // Success
        }
        timeout--;
    }
    return 0;  // Timeout
}

// Wait for compare match B flag with timeout
int wait_compare_b(unsigned int timeout) {
    while (timeout > 0) {
        if (TIFR0 & OCF0B) {
            TIFR0 = OCF0B;
            return 1;
        }
        timeout--;
    }
    return 0;
}

// Reset timer to known state
void reset_timer(void) {
    TCCR0B = 0;  // Stop timer first
    TCCR0A = 0;
    TCNT0 = 0;
    OCR0A = 0;
    OCR0B = 0;
    TIMSK0 = 0;
    TIFR0 = 0x07;  // Clear all flags
}

int main(void) {
    // Configure PORTB as output for test status LEDs
    DDRB = 0xFF;
    PORTB = 0x00;
    
    // =========================================================================
    // Test 0: Timer Register Access
    // Verify we can write and read back timer registers
    // =========================================================================
    reset_timer();
    
    OCR0A = 0xAA;
    OCR0B = 0x55;
    TCNT0 = 0x12;
    
    if (OCR0A == 0xAA && OCR0B == 0x55 && TCNT0 == 0x12) {
        signal_pass(0);  // PB0 = Test 0 passed
    }
    
    // =========================================================================
    // Test 1: Normal Mode Counting
    // Timer should count when started
    // Use /1024 prescaler to avoid wrap-around issues at high clock rates
    // =========================================================================
    reset_timer();
    
    TCCR0A = 0x00;      // Normal mode
    TCCR0B = CS_DIV1024; // Start with clk/1024 (slower, won't wrap quickly)
    
    // Wait for timer to count a few ticks
    volatile int i;
    for (i = 0; i < 100; i++) {
        __asm__ __volatile__("nop");
    }
    
    unsigned char count1 = TCNT0;
    
    // Wait more and read again
    for (i = 0; i < 100; i++) {
        __asm__ __volatile__("nop");
    }
    
    unsigned char count2 = TCNT0;
    
    TCCR0B = 0;  // Stop timer
    
    // With /1024 prescaler, timer counts slowly enough that count2 > count1
    // Also verify timer actually started (count1 > 0 or count2 > count1)
    if (count2 > count1 || count1 > 0) {
        signal_pass(1);  // PB1 = Test 1 passed
    }
    
    // =========================================================================
    // Test 2: Overflow Detection
    // Start counter near max and wait for overflow flag
    // =========================================================================
    reset_timer();
    
    TCNT0 = 0xF8;  // Start at 248
    TCCR0A = 0x00;  // Normal mode
    TCCR0B = CS_DIV1;  // clk/1
    
    if (wait_overflow(1000)) {
        signal_pass(2);  // PB2 = Test 2 passed
    }
    
    // =========================================================================
    // Test 3: Compare Match A Detection
    // =========================================================================
    reset_timer();
    
    OCR0A = 0x10;  // Compare at 16
    TCCR0A = 0x00;  // Normal mode
    TCCR0B = CS_DIV1;
    
    if (wait_compare_a(1000)) {
        signal_pass(3);  // PB3 = Test 3 passed
    }
    
    // =========================================================================
    // Test 4: Compare Match B Detection
    // =========================================================================
    reset_timer();
    
    OCR0B = 0x20;  // Compare at 32
    TCCR0A = 0x00;
    TCCR0B = CS_DIV1;
    
    if (wait_compare_b(1000)) {
        signal_pass(4);  // PB4 = Test 4 passed
    }
    
    // =========================================================================
    // Test 5: CTC Mode (Clear Timer on Compare)
    // Timer should clear when it reaches OCR0A
    // =========================================================================
    reset_timer();
    
    OCR0A = 0x0F;  // TOP = 15
    TCCR0A = 0x02;  // CTC mode (WGM = 010)
    TCCR0B = CS_DIV1;
    
    // Wait for a compare match
    if (wait_compare_a(500)) {
        // Wait a bit and check counter stays below OCR0A
        for (i = 0; i < 50; i++) {
            __asm__ __volatile__("nop");
        }
        
        if (TCNT0 <= 0x0F) {
            signal_pass(5);  // PB5 = Test 5 passed
        }
    }
    
    // =========================================================================
    // Test 6: Prescaler Test (clk/8 vs clk/1024)
    // Verify prescaler affects counting rate by comparing two prescalers
    // =========================================================================
    
    // First, measure count with /1024 prescaler
    reset_timer();
    TCCR0A = 0x00;
    TCCR0B = CS_DIV1024;  // clk/1024
    
    // Wait a fixed number of cycles
    for (i = 0; i < 200; i++) {
        __asm__ __volatile__("nop");
    }
    
    unsigned char count_slow = TCNT0;
    TCCR0B = 0;  // Stop
    
    // Now measure count with /8 prescaler (128x faster than /1024)
    reset_timer();
    TCCR0A = 0x00;
    TCCR0B = CS_DIV8;  // clk/8
    
    // Wait the same number of cycles
    for (i = 0; i < 200; i++) {
        __asm__ __volatile__("nop");
    }
    
    unsigned char count_fast = TCNT0;
    TCCR0B = 0;  // Stop
    
    // With /8 prescaler, counter should be significantly higher than with /1024
    // /8 is 128x faster than /1024, so count_fast should be much larger
    if (count_fast > count_slow) {
        signal_pass(6);  // PB6 = Test 6 passed
    }
    
    // =========================================================================
    // Test 7: All Tests Complete
    // If we got here, set final success indicator
    // =========================================================================
    
    // Check if all previous tests passed (bits 0-6)
    if ((test_results & 0x7F) == 0x7F) {
        signal_pass(7);  // PB7 = All tests passed!
    }
    
    // Final state: PORTB should show 0xFF if all tests passed
    // Each bit represents a test: PB0=Test0, PB1=Test1, etc.
    
    // Halt - spin forever
    while (1) {
        __asm__ __volatile__("nop");
    }
    
    return 0;
}
