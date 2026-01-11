// Watchdog Timer Integration Test for ATmega328P RISC-V replica
// Tests WDT functionality via firmware running on the SoC
//
// Test Signals on PORTB:
//   Bit 0: WDT register access OK
//   Bit 1: WDR command works
//   Bit 2: WDT interrupt fires
//   Bit 3: WDIF flag set correctly
//   Bit 4: Timed sequence works
//   Bit 5: WDCE auto-clears
//   Bit 6: Reserved
//   Bit 7: All tests passed
//
// Final Results:
//   0xAA: All tests passed
//   0x55: Test failed

// GPIO Port B register addresses
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// Watchdog Timer register addresses (ATmega328P compatible)
#define MCUSR  (*((volatile unsigned char*)0x20000054))
#define WDTCSR (*((volatile unsigned char*)0x20000060))
#define WDR    (*((volatile unsigned char*)0x20000061))

// MCUSR bits
#define WDRF  (1 << 3)  // Watchdog Reset Flag

// WDTCSR bits
#define WDIF  (1 << 7)  // Watchdog Interrupt Flag
#define WDIE  (1 << 6)  // Watchdog Interrupt Enable
#define WDP3  (1 << 5)  // Prescaler bit 3
#define WDCE  (1 << 4)  // Watchdog Change Enable
#define WDE   (1 << 3)  // Watchdog System Reset Enable
#define WDP2  (1 << 2)  // Prescaler bit 2
#define WDP1  (1 << 1)  // Prescaler bit 1
#define WDP0  (1 << 0)  // Prescaler bit 0

// WDR magic value
#define WDR_MAGIC 0xA5

// Prescaler settings (WDP3:0)
#define WDP_16MS   0x00  // 2K cycles
#define WDP_32MS   0x01  // 4K cycles
#define WDP_64MS   0x02  // 8K cycles
#define WDP_125MS  0x03  // 16K cycles
#define WDP_250MS  0x04  // 32K cycles
#define WDP_500MS  0x05  // 64K cycles
#define WDP_1S     0x06  // 128K cycles
#define WDP_2S     0x07  // 256K cycles
#define WDP_4S     0x28  // 512K cycles (WDP3=1, WDP2:0=000)
#define WDP_8S     0x29  // 1024K cycles (WDP3=1, WDP2:0=001)

// Test status
static unsigned char test_results = 0;

// Signal test pass on specific bit
void signal_pass(unsigned char test_bit) {
    test_results |= (1 << test_bit);
    PORTB = test_results;
}

// Signal test failure
void signal_fail(void) {
    PORTB = 0x55;  // Failure pattern
    while (1) {
        __asm__ __volatile__("nop");
    }
}

// Short delay for timing between operations
void delay(unsigned int count) {
    volatile unsigned int i;
    for (i = 0; i < count; i++) {
        __asm__ __volatile__("nop");
    }
}

// Poll WDIF for timeout with iteration limit
int wait_for_wdif(unsigned int max_iterations) {
    unsigned int i;
    for (i = 0; i < max_iterations; i++) {
        if (WDTCSR & WDIF) {
            return 1;  // Success
        }
    }
    return 0;  // Timeout waiting
}

// Issue WDR (Watchdog Reset) command
void wdt_reset(void) {
    WDR = WDR_MAGIC;
}

// Disable WDT using timed sequence
void wdt_disable(void) {
    // Start timed sequence
    WDTCSR = WDCE | WDE;
    // Within 4 cycles, clear WDE
    WDTCSR = 0x00;
}

// Enable WDT in interrupt mode
void wdt_enable_interrupt(unsigned char prescaler) {
    // Start timed sequence
    WDTCSR = WDCE | WDE;
    // Set new config: WDIE=1, WDE=0, prescaler bits
    WDTCSR = WDIE | (prescaler & 0x27);  // Mask valid prescaler bits
}

// Enable WDT in reset mode
void wdt_enable_reset(unsigned char prescaler) {
    // Start timed sequence
    WDTCSR = WDCE | WDE;
    // Set new config: WDIE=0, WDE=1, prescaler bits
    WDTCSR = WDE | (prescaler & 0x27);
}

// Check if WDIF is set
int wdt_interrupt_pending(void) {
    return (WDTCSR & WDIF) != 0;
}

// Clear WDIF by writing 1
void wdt_clear_interrupt(void) {
    WDTCSR |= WDIF;
}

int main(void) {
    unsigned char val;
    
    // Configure PORTB as output for test status
    DDRB = 0xFF;
    PORTB = 0x00;
    
    // =========================================================================
    // Test 0: WDT Register Access
    // Verify we can read/write WDTCSR and MCUSR
    // =========================================================================
    
    // Read initial values (should be 0 after reset)
    val = WDTCSR;
    if (val != 0x00) {
        signal_fail();
    }
    
    val = MCUSR;
    // MCUSR might have WDRF set from previous reset, clear it
    MCUSR = 0x00;
    
    // Write WDIE (should work without timed sequence)
    WDTCSR = WDIE;
    val = WDTCSR;
    if ((val & WDIE) == 0) {
        signal_fail();
    }
    
    // Disable for next tests
    wdt_disable();
    
    signal_pass(0);  // Test 0 passed
    
    // =========================================================================
    // Test 1: WDR Command
    // Verify WDR command resets the counter (doesn't cause immediate timeout)
    // =========================================================================
    
    // Enable WDT in interrupt mode with shortest timeout
    wdt_enable_interrupt(WDP_16MS);
    
    // Issue WDR repeatedly - should NOT timeout
    for (int i = 0; i < 1000; i++) {
        wdt_reset();
        delay(10);
    }
    
    // If we're still here, WDR is working
    // Check that WDIF is not set (no timeout)
    if (wdt_interrupt_pending()) {
        // Timeout occurred despite WDR - fail
        wdt_disable();
        signal_fail();
    }
    
    wdt_disable();
    signal_pass(1);  // Test 1 passed
    
    // =========================================================================
    // Test 2: WDT Interrupt Mode
    // Verify WDT generates interrupt on timeout
    // =========================================================================
    
    // Enable WDT in interrupt mode
    wdt_enable_interrupt(WDP_16MS);
    
    // Wait for WDIF to be set (polling)
    // WDT timeout: 2048 * 128 = 262144 system cycles
    // Each poll: ~10 cycles, so 50000 iterations is plenty
    if (!wait_for_wdif(50000)) {
        wdt_disable();
        signal_fail();
    }
    
    // Check WDIF is set (double-check)
    if (!wdt_interrupt_pending()) {
        wdt_disable();
        signal_fail();
    }
    
    signal_pass(2);  // Test 2 passed
    
    // =========================================================================
    // Test 3: WDIF Flag Handling
    // Verify WDIF can be cleared by writing 1
    // =========================================================================
    
    // WDIF should still be set from Test 2
    if (!wdt_interrupt_pending()) {
        signal_fail();
    }
    
    // Clear WDIF
    wdt_clear_interrupt();
    
    // Check WDIF is cleared
    if (wdt_interrupt_pending()) {
        wdt_disable();
        signal_fail();
    }
    
    wdt_disable();
    signal_pass(3);  // Test 3 passed
    
    // =========================================================================
    // Test 4: Timed Sequence
    // Verify WDE can only be cleared during timed sequence
    // =========================================================================
    
    // Enable WDE
    WDTCSR = WDE;
    
    // Try to clear WDE without timed sequence (should fail)
    WDTCSR = 0x00;
    
    val = WDTCSR;
    if ((val & WDE) == 0) {
        // WDE was cleared without timed sequence - fail
        signal_fail();
    }
    
    // Now use timed sequence to clear WDE
    WDTCSR = WDCE | WDE;
    WDTCSR = 0x00;
    
    val = WDTCSR;
    if ((val & WDE) != 0) {
        // WDE was not cleared with timed sequence - fail
        signal_fail();
    }
    
    signal_pass(4);  // Test 4 passed
    
    // =========================================================================
    // Test 5: WDCE Auto-clear
    // Verify WDCE clears automatically after 4 cycles
    // =========================================================================
    
    // Start timed sequence
    WDTCSR = WDCE | WDE;
    
    // Wait more than 4 cycles
    delay(100);
    
    // Check WDCE is cleared
    val = WDTCSR;
    if ((val & WDCE) != 0) {
        // WDCE should be auto-cleared - fail
        signal_fail();
    }
    
    wdt_disable();
    signal_pass(5);  // Test 5 passed
    
    // =========================================================================
    // All Tests Complete
    // =========================================================================
    
    // Check all tests passed (bits 0-5)
    if ((test_results & 0x3F) == 0x3F) {
        signal_pass(7);  // Set bit 7 for all passed
        PORTB = 0xAA;    // Signal complete success
    }
    
    // Spin forever
    while (1) {
        __asm__ __volatile__("nop");
    }
    
    return 0;
}
