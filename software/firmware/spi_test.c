// SPI Integration Test for ATmega328P RISC-V replica
// Tests SPI functionality via firmware running on the SoC

// GPIO Port B register addresses
#define PINB  (*((volatile unsigned char*)0x20000023))
#define DDRB  (*((volatile unsigned char*)0x20000024))
#define PORTB (*((volatile unsigned char*)0x20000025))

// SPI register addresses (ATmega328P compatible)
#define SPCR  (*((volatile unsigned char*)0x2000004C))
#define SPSR  (*((volatile unsigned char*)0x2000004D))
#define SPDR  (*((volatile unsigned char*)0x2000004E))

// SPCR bit positions
#define SPIE  (1 << 7)
#define SPE   (1 << 6)
#define DORD  (1 << 5)
#define MSTR  (1 << 4)
#define CPOL  (1 << 3)
#define CPHA  (1 << 2)
#define SPR1  (1 << 1)
#define SPR0  (1 << 0)

// SPSR bit positions
#define SPIF  (1 << 7)
#define WCOL  (1 << 6)
#define SPI2X (1 << 0)

// Test result indicators on PORTB
// PB0 = Test 0 pass, PB1 = Test 1 pass, etc.
// All LEDs on = all tests passed

// Test status
static unsigned char test_results = 0;

// Signal test pass on specific bit
void signal_pass(unsigned char test_bit) {
    test_results |= (1 << test_bit);
    PORTB = test_results;
}

// Wait for SPIF flag with timeout
int wait_spif(unsigned int timeout) {
    while (timeout > 0) {
        if (SPSR & SPIF) {
            return 1;  // Success
        }
        timeout--;
    }
    return 0;  // Timeout
}

// Transfer a byte via SPI and return received byte
unsigned char spi_transfer(unsigned char data) {
    SPDR = data;
    
    // Wait for transfer complete
    while (!(SPSR & SPIF)) {
        // Spin wait
    }
    
    return SPDR;
}

// Reset SPI to known state
void reset_spi(void) {
    // Clear any pending SPIF by reading SPSR then SPDR
    volatile unsigned char dummy;
    dummy = SPSR;
    dummy = SPDR;
    (void)dummy;  // Suppress unused variable warning
    
    SPCR = 0;
    SPSR = 0;
}

int main(void) {
    // Configure PORTB as output for test status LEDs
    DDRB = 0xFF;
    PORTB = 0x00;
    
    // Small delay for stability
    volatile int i;
    for (i = 0; i < 10; i++) {
        __asm__ __volatile__("nop");
    }
    
    // =========================================================================
    // Test 0: SPI Register Access
    // Verify we can write and read back SPI registers
    // =========================================================================
    reset_spi();
    
    SPCR = 0xAA;
    unsigned char spcr_val = SPCR;
    
    SPSR = 0x01;  // Only SPI2X is writable
    unsigned char spsr_val = SPSR;
    
    // SPCR should read back 0xAA
    // SPSR should have SPI2X set (bit 0)
    if (spcr_val == 0xAA && (spsr_val & SPI2X)) {
        signal_pass(0);  // PB0 = Test 0 passed
    }
    
    // =========================================================================
    // Test 1: Master Mode Enable
    // Enable SPI in Master mode
    // =========================================================================
    reset_spi();
    
    SPCR = SPE | MSTR;  // Enable SPI, Master mode
    
    if ((SPCR & (SPE | MSTR)) == (SPE | MSTR)) {
        signal_pass(1);  // PB1 = Test 1 passed
    }
    
    // =========================================================================
    // Test 2: Single Byte Transfer
    // Perform a single byte transfer (with loopback MOSI->MISO)
    // =========================================================================
    reset_spi();
    
    SPCR = SPE | MSTR;  // Enable SPI, Master mode
    
    SPDR = 0xA5;  // Start transfer
    
    if (wait_spif(1000)) {
        signal_pass(2);  // PB2 = Test 2 passed
    }
    
    // =========================================================================
    // Test 3: SPIF Flag Detection
    // Verify SPIF flag is set after transfer
    // =========================================================================
    reset_spi();
    
    SPCR = SPE | MSTR;
    
    // SPIF should be clear before transfer
    if (!(SPSR & SPIF)) {
        SPDR = 0x5A;
        wait_spif(1000);
        
        // SPIF should be set after transfer
        if (SPSR & SPIF) {
            signal_pass(3);  // PB3 = Test 3 passed
        }
    }
    
    // =========================================================================
    // Test 4: Loopback Verification
    // Verify data sent is received (with external loopback)
    // =========================================================================
    reset_spi();
    
    SPCR = SPE | MSTR;
    
    unsigned char tx_data = 0xDE;
    unsigned char rx_data = spi_transfer(tx_data);
    
    // With loopback, received data should match sent data
    if (rx_data == tx_data) {
        signal_pass(4);  // PB4 = Test 4 passed
    }
    
    // =========================================================================
    // Test 5: Multi-Byte Transfer
    // Transfer multiple bytes sequentially
    // =========================================================================
    reset_spi();
    
    SPCR = SPE | MSTR;
    
    unsigned char data1 = spi_transfer(0x11);
    unsigned char data2 = spi_transfer(0x22);
    unsigned char data3 = spi_transfer(0x33);
    
    // All bytes should match with loopback
    if (data1 == 0x11 && data2 == 0x22 && data3 == 0x33) {
        signal_pass(5);  // PB5 = Test 5 passed
    }
    
    // =========================================================================
    // Test 6: Clock Rate Change
    // Verify different clock rates work
    // =========================================================================
    reset_spi();
    
    // Test with /16 prescaler (SPR=01)
    SPCR = SPE | MSTR | SPR0;
    
    unsigned char slow_data = spi_transfer(0xAB);
    
    // Then with /4 prescaler (SPR=00)
    SPCR = SPE | MSTR;
    
    unsigned char fast_data = spi_transfer(0xCD);
    
    if (slow_data == 0xAB && fast_data == 0xCD) {
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
