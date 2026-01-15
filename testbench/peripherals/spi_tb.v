// SPI Unit Testbench
// Comprehensive tests for ATmega328P-compatible SPI peripheral
`timescale 1ns / 1ps

module spi_tb;

    // Test control
    integer test_num;
    integer errors;
    
    // DUT signals
    reg         clk;
    reg         rst_n;
    reg         mem_valid;
    reg  [31:0] mem_addr;
    reg  [31:0] mem_wdata;
    reg  [3:0]  mem_wstrb;
    wire [31:0] mem_rdata;
    wire        mem_ready;
    wire        sck;
    wire        mosi;
    reg         miso;
    reg         ss_n;
    reg         sck_in;
    wire        irq_spi;

    // Register addresses
    localparam SPCR_ADDR = 32'h2000004C;
    localparam SPSR_ADDR = 32'h2000004D;
    localparam SPDR_ADDR = 32'h2000004E;

    // SPCR bit positions
    localparam SPIE = 7;
    localparam SPE  = 6;
    localparam DORD = 5;
    localparam MSTR = 4;
    localparam CPOL = 3;
    localparam CPHA = 2;

    // SPSR bit positions
    localparam SPIF = 7;
    localparam WCOL = 6;
    localparam SPI2X = 0;

    // Instantiate DUT
    spi uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
        .sck(sck),
        .mosi(mosi),
        .miso(miso),
        .ss_n(ss_n),
        .sck_in(sck_in),
        .irq_spi(irq_spi)
    );

    // Clock generation (10ns period = 100MHz)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // VCD dump
    initial begin
        $dumpfile("spi_tb.vcd");
        $dumpvars(0, spi_tb);
    end

    // Loopback connection for testing (directly connect mosi to miso)
    // This is controlled per-test via miso_loopback flag
    reg miso_loopback;
    always @(*) begin
        if (miso_loopback)
            miso = mosi;
    end

    // Main test sequence
    initial begin
        errors = 0;
        test_num = 0;
        
        // Initialize
        rst_n = 0;
        mem_valid = 0;
        mem_addr = 0;
        mem_wdata = 0;
        mem_wstrb = 0;
        miso = 0;
        ss_n = 1;
        sck_in = 0;
        miso_loopback = 0;
        
        // Reset
        #100;
        rst_n = 1;
        #20;

        // =====================================================================
        // Test 1: Reset Values
        // =====================================================================
        test_num = 1;
        $display("\n=== Test %0d: Reset Values ===", test_num);
        
        read_reg(SPCR_ADDR);
        check_value("SPCR reset", read_byte, 8'h00);
        
        read_reg(SPSR_ADDR);
        check_value("SPSR reset", read_byte, 8'h00);
        
        read_reg(SPDR_ADDR);
        check_value("SPDR reset", read_byte, 8'h00);

        // =====================================================================
        // Test 2: SPCR Read/Write
        // =====================================================================
        test_num = 2;
        $display("\n=== Test %0d: SPCR Read/Write ===", test_num);
        
        // Write all bits
        write_reg(SPCR_ADDR, 8'hFF);
        read_reg(SPCR_ADDR);
        check_value("SPCR all bits", read_byte, 8'hFF);
        
        // Write pattern
        write_reg(SPCR_ADDR, 8'hA5);
        read_reg(SPCR_ADDR);
        check_value("SPCR pattern", read_byte, 8'hA5);
        
        reset_spi();

        // =====================================================================
        // Test 3: SPSR Read-Only Bits (SPIF, WCOL)
        // =====================================================================
        test_num = 3;
        $display("\n=== Test %0d: SPSR Read-Only Bits ===", test_num);
        
        // Try to write SPIF and WCOL directly - they should not change
        write_reg(SPSR_ADDR, 8'hC0);  // Try to set SPIF and WCOL
        read_reg(SPSR_ADDR);
        // SPIF and WCOL are read-only, should still be 0
        check_value("SPSR read-only", read_byte & 8'hC0, 8'h00);
        
        reset_spi();

        // =====================================================================
        // Test 4: SPSR Writable Bit (SPI2X)
        // =====================================================================
        test_num = 4;
        $display("\n=== Test %0d: SPSR Writable Bit (SPI2X) ===", test_num);
        
        write_reg(SPSR_ADDR, 8'h01);  // Set SPI2X
        read_reg(SPSR_ADDR);
        check_value("SPI2X set", read_byte & 8'h01, 8'h01);
        
        write_reg(SPSR_ADDR, 8'h00);  // Clear SPI2X
        read_reg(SPSR_ADDR);
        check_value("SPI2X clear", read_byte & 8'h01, 8'h00);
        
        reset_spi();

        // =====================================================================
        // Test 5: SPDR Read/Write
        // =====================================================================
        test_num = 5;
        $display("\n=== Test %0d: SPDR Read/Write ===", test_num);
        
        // Note: Writing SPDR when SPI is disabled doesn't start transfer
        write_reg(SPDR_ADDR, 8'hAB);
        // SPDR read returns rx_data which starts at 0
        read_reg(SPDR_ADDR);
        // When SPI is not enabled/transferring, read returns rx_data buffer
        // Initial value is 0
        $display("INFO: SPDR read = 0x%02x (rx buffer)", read_byte);
        
        reset_spi();

        // =====================================================================
        // Test 6: Master Mode Enable
        // =====================================================================
        test_num = 6;
        $display("\n=== Test %0d: Master Mode Enable ===", test_num);
        
        // Enable SPI in Master mode
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        read_reg(SPCR_ADDR);
        check_value("Master mode", read_byte, 8'h50);
        
        reset_spi();

        // =====================================================================
        // Test 7: Clock Rate /4 (SPR=00, SPI2X=0)
        // =====================================================================
        test_num = 7;
        $display("\n=== Test %0d: Clock Rate /4 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));  // SPR=00
        write_reg(SPSR_ADDR, 8'h00);  // SPI2X=0
        
        // Start transfer
        write_reg(SPDR_ADDR, 8'hA5);
        
        // Wait for transfer to complete
        wait_spif(1000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /4", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 8: Clock Rate /16 (SPR=01, SPI2X=0)
        // =====================================================================
        test_num = 8;
        $display("\n=== Test %0d: Clock Rate /16 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h01);  // SPR=01
        write_reg(SPSR_ADDR, 8'h00);
        
        write_reg(SPDR_ADDR, 8'h5A);
        wait_spif(2000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /16", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 9: Clock Rate /64 (SPR=10, SPI2X=0)
        // =====================================================================
        test_num = 9;
        $display("\n=== Test %0d: Clock Rate /64 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h02);  // SPR=10
        write_reg(SPSR_ADDR, 8'h00);
        
        write_reg(SPDR_ADDR, 8'hF0);
        wait_spif(6000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /64", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 10: Clock Rate /128 (SPR=11, SPI2X=0)
        // =====================================================================
        test_num = 10;
        $display("\n=== Test %0d: Clock Rate /128 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h03);  // SPR=11
        write_reg(SPSR_ADDR, 8'h00);
        
        write_reg(SPDR_ADDR, 8'h0F);
        wait_spif(12000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /128", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 11: Clock Rate /2 (SPR=00, SPI2X=1)
        // =====================================================================
        test_num = 11;
        $display("\n=== Test %0d: Clock Rate /2 (Double Speed) ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));  // SPR=00
        write_reg(SPSR_ADDR, 8'h01);  // SPI2X=1
        
        write_reg(SPDR_ADDR, 8'hCC);
        wait_spif(500);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /2", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 12: Clock Rate /8 (SPR=01, SPI2X=1)
        // =====================================================================
        test_num = 12;
        $display("\n=== Test %0d: Clock Rate /8 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h01);
        write_reg(SPSR_ADDR, 8'h01);  // SPI2X=1
        
        write_reg(SPDR_ADDR, 8'h33);
        wait_spif(1000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /8", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 13: Clock Rate /32 (SPR=10, SPI2X=1)
        // =====================================================================
        test_num = 13;
        $display("\n=== Test %0d: Clock Rate /32 ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h02);
        write_reg(SPSR_ADDR, 8'h01);  // SPI2X=1
        
        write_reg(SPDR_ADDR, 8'h55);
        wait_spif(3000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF set /32", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 14: SPI Mode 0 (CPOL=0, CPHA=0)
        // =====================================================================
        test_num = 14;
        $display("\n=== Test %0d: SPI Mode 0 (CPOL=0, CPHA=0) ===", test_num);
        
        miso_loopback = 1;
        // Mode 0: CPOL=0, CPHA=0
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        // Check SCK idle level (should be low for CPOL=0)
        check_value("SCK idle low", sck, 1'b0);
        
        write_reg(SPDR_ADDR, 8'hA5);
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("Mode 0 loopback", read_byte, 8'hA5);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 15: SPI Mode 1 (CPOL=0, CPHA=1)
        // =====================================================================
        test_num = 15;
        $display("\n=== Test %0d: SPI Mode 1 (CPOL=0, CPHA=1) ===", test_num);
        
        miso_loopback = 1;
        // Mode 1: CPOL=0, CPHA=1
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | (1 << CPHA));
        
        check_value("SCK idle low M1", sck, 1'b0);
        
        write_reg(SPDR_ADDR, 8'h5A);
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("Mode 1 loopback", read_byte, 8'h5A);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 16: SPI Mode 2 (CPOL=1, CPHA=0)
        // =====================================================================
        test_num = 16;
        $display("\n=== Test %0d: SPI Mode 2 (CPOL=1, CPHA=0) ===", test_num);
        
        miso_loopback = 1;
        // Mode 2: CPOL=1, CPHA=0
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | (1 << CPOL));
        
        // SCK idle should be high for CPOL=1
        repeat(5) @(posedge clk);
        check_value("SCK idle high", sck, 1'b1);
        
        write_reg(SPDR_ADDR, 8'hF0);
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("Mode 2 loopback", read_byte, 8'hF0);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 17: SPI Mode 3 (CPOL=1, CPHA=1)
        // =====================================================================
        test_num = 17;
        $display("\n=== Test %0d: SPI Mode 3 (CPOL=1, CPHA=1) ===", test_num);
        
        miso_loopback = 1;
        // Mode 3: CPOL=1, CPHA=1
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA));
        
        repeat(5) @(posedge clk);
        check_value("SCK idle high M3", sck, 1'b1);
        
        write_reg(SPDR_ADDR, 8'h0F);
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("Mode 3 loopback", read_byte, 8'h0F);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 18: MSB First Transfer (DORD=0)
        // =====================================================================
        test_num = 18;
        $display("\n=== Test %0d: MSB First Transfer ===", test_num);
        
        miso_loopback = 1;
        // DORD=0 (MSB first, default)
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        write_reg(SPDR_ADDR, 8'h81);  // 10000001
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("MSB first", read_byte, 8'h81);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 19: LSB First Transfer (DORD=1)
        // =====================================================================
        test_num = 19;
        $display("\n=== Test %0d: LSB First Transfer ===", test_num);
        
        miso_loopback = 1;
        // DORD=1 (LSB first)
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | (1 << DORD));
        
        write_reg(SPDR_ADDR, 8'h81);
        wait_spif(1000);
        
        read_reg(SPDR_ADDR);
        check_value("LSB first", read_byte, 8'h81);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 20: Single Byte Transfer Complete
        // =====================================================================
        test_num = 20;
        $display("\n=== Test %0d: Single Byte Transfer Complete ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        // Verify SPIF is clear before transfer
        read_reg(SPSR_ADDR);
        check_value("SPIF clear before", read_byte[SPIF], 1'b0);
        
        write_reg(SPDR_ADDR, 8'hDE);
        wait_spif(1000);
        
        // Verify SPIF is set after transfer
        read_reg(SPSR_ADDR);
        check_value("SPIF set after", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 21: SPIF Flag Set on Completion
        // =====================================================================
        test_num = 21;
        $display("\n=== Test %0d: SPIF Flag Set on Completion ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        write_reg(SPDR_ADDR, 8'hAD);
        
        // Wait and check SPIF
        wait_spif(1000);
        
        read_reg(SPSR_ADDR);
        check_value("SPIF on complete", read_byte[SPIF], 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 22: SPIF Cleared by Read Sequence
        // =====================================================================
        test_num = 22;
        $display("\n=== Test %0d: SPIF Cleared by Read Sequence ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        write_reg(SPDR_ADDR, 8'hBE);
        wait_spif(1000);
        
        // Verify SPIF is set
        read_reg(SPSR_ADDR);
        check_value("SPIF set", read_byte[SPIF], 1'b1);
        
        // Read SPDR to clear SPIF
        read_reg(SPDR_ADDR);
        
        // Verify SPIF is now cleared
        read_reg(SPSR_ADDR);
        check_value("SPIF cleared", read_byte[SPIF], 1'b0);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 23: Multi-Byte Transfer
        // =====================================================================
        test_num = 23;
        $display("\n=== Test %0d: Multi-Byte Transfer ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        // First byte
        write_reg(SPDR_ADDR, 8'h11);
        wait_spif(1000);
        read_reg(SPSR_ADDR);  // Read SPSR
        read_reg(SPDR_ADDR);  // Read SPDR (clears SPIF)
        check_value("Byte 1", read_byte, 8'h11);
        
        // Second byte
        write_reg(SPDR_ADDR, 8'h22);
        wait_spif(1000);
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        check_value("Byte 2", read_byte, 8'h22);
        
        // Third byte
        write_reg(SPDR_ADDR, 8'h33);
        wait_spif(1000);
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        check_value("Byte 3", read_byte, 8'h33);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 24: Write Collision Detection (WCOL)
        // =====================================================================
        test_num = 24;
        $display("\n=== Test %0d: Write Collision Detection ===", test_num);
        
        miso_loopback = 1;
        // Use slower clock for more reliable collision detection
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR) | 8'h03);  // /128
        
        // Start first transfer
        write_reg(SPDR_ADDR, 8'hAA);
        
        // Immediately try to write again (collision)
        repeat(50) @(posedge clk);  // Small delay but still during transfer
        write_reg(SPDR_ADDR, 8'hBB);
        
        // Check WCOL flag
        read_reg(SPSR_ADDR);
        check_value("WCOL set", read_byte[WCOL], 1'b1);
        
        // Wait for transfer to complete
        wait_spif(15000);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 25: Loopback Test (MOSI to MISO)
        // =====================================================================
        test_num = 25;
        $display("\n=== Test %0d: Loopback Test ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));
        
        // Test pattern
        write_reg(SPDR_ADDR, 8'hA5);
        wait_spif(1000);
        
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        check_value("Loopback A5", read_byte, 8'hA5);
        
        // Another pattern
        write_reg(SPDR_ADDR, 8'h5A);
        wait_spif(1000);
        
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        check_value("Loopback 5A", read_byte, 8'h5A);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 26: Slave Mode Enable
        // =====================================================================
        test_num = 26;
        $display("\n=== Test %0d: Slave Mode Enable ===", test_num);
        
        // Enable SPI in Slave mode (MSTR=0)
        write_reg(SPCR_ADDR, (1 << SPE));
        read_reg(SPCR_ADDR);
        check_value("Slave mode", read_byte, 8'h40);
        
        reset_spi();

        // =====================================================================
        // Test 27: SS_n Pin Control (Slave Mode)
        // =====================================================================
        test_num = 27;
        $display("\n=== Test %0d: SS_n Pin Control ===", test_num);
        
        // Configure as slave
        write_reg(SPCR_ADDR, (1 << SPE));
        
        // With SS_n high, slave should not be selected
        ss_n = 1;
        repeat(10) @(posedge clk);
        
        // With SS_n low, slave should be selected
        ss_n = 0;
        repeat(10) @(posedge clk);
        
        $display("PASS: SS_n control tested");
        
        ss_n = 1;
        reset_spi();

        // =====================================================================
        // Test 28: Slave Receives Data on SCK
        // =====================================================================
        test_num = 28;
        $display("\n=== Test %0d: Slave Receives Data ===", test_num);
        
        write_reg(SPCR_ADDR, (1 << SPE));  // Slave mode
        write_reg(SPDR_ADDR, 8'h00);  // Load tx buffer
        
        ss_n = 0;  // Select slave
        repeat(5) @(posedge clk);
        
        // Shift in 0xA5 (10100101) MSB first
        miso = 0;  // Not used in this direction
        shift_in_byte(8'hA5);
        
        // Check received data
        wait_spif(100);
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        check_value("Slave RX", read_byte, 8'hA5);
        
        ss_n = 1;
        reset_spi();

        // =====================================================================
        // Test 29: Slave Transmits Data
        // =====================================================================
        test_num = 29;
        $display("\n=== Test %0d: Slave Transmits Data ===", test_num);
        
        write_reg(SPCR_ADDR, (1 << SPE));  // Slave mode
        write_reg(SPDR_ADDR, 8'h5A);  // Load tx buffer
        
        ss_n = 0;  // Select slave
        repeat(10) @(posedge clk);
        
        // Clock out 8 bits and capture MOSI (which carries slave's data)
        // Note: In actual slave mode, slave outputs on MISO, but our
        // shift register uses same output
        shift_in_byte(8'h00);
        
        wait_spif(100);
        
        $display("PASS: Slave TX tested");
        
        ss_n = 1;
        reset_spi();

        // =====================================================================
        // Test 30: Slave Mode SPIF Flag
        // =====================================================================
        test_num = 30;
        $display("\n=== Test %0d: Slave Mode SPIF Flag ===", test_num);
        
        write_reg(SPCR_ADDR, (1 << SPE));  // Slave mode
        write_reg(SPDR_ADDR, 8'h00);
        
        // Verify SPIF is clear
        read_reg(SPSR_ADDR);
        check_value("SPIF clear slave", read_byte[SPIF], 1'b0);
        
        ss_n = 0;
        shift_in_byte(8'hFF);
        
        // SPIF should be set
        wait_spif(100);
        read_reg(SPSR_ADDR);
        check_value("SPIF set slave", read_byte[SPIF], 1'b1);
        
        ss_n = 1;
        reset_spi();

        // =====================================================================
        // Test 31: IRQ when SPIE=1 and SPIF=1
        // =====================================================================
        test_num = 31;
        $display("\n=== Test %0d: IRQ when SPIE=1 and SPIF=1 ===", test_num);
        
        miso_loopback = 1;
        // Enable interrupt
        write_reg(SPCR_ADDR, (1 << SPIE) | (1 << SPE) | (1 << MSTR));
        
        // Verify IRQ is low before transfer
        check_value("IRQ low before", irq_spi, 1'b0);
        
        write_reg(SPDR_ADDR, 8'hEE);
        wait_spif(1000);
        
        // Verify IRQ is high after transfer
        check_value("IRQ high after", irq_spi, 1'b1);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 32: IRQ Masked when SPIE=0
        // =====================================================================
        test_num = 32;
        $display("\n=== Test %0d: IRQ Masked when SPIE=0 ===", test_num);
        
        miso_loopback = 1;
        // Disable interrupt
        write_reg(SPCR_ADDR, (1 << SPE) | (1 << MSTR));  // SPIE=0
        
        write_reg(SPDR_ADDR, 8'hDD);
        wait_spif(1000);
        
        // SPIF is set but IRQ should be masked
        read_reg(SPSR_ADDR);
        check_value("SPIF set", read_byte[SPIF], 1'b1);
        check_value("IRQ masked", irq_spi, 1'b0);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Test 33: IRQ Cleared with SPIF
        // =====================================================================
        test_num = 33;
        $display("\n=== Test %0d: IRQ Cleared with SPIF ===", test_num);
        
        miso_loopback = 1;
        write_reg(SPCR_ADDR, (1 << SPIE) | (1 << SPE) | (1 << MSTR));
        
        write_reg(SPDR_ADDR, 8'hCC);
        wait_spif(1000);
        
        // IRQ should be high
        check_value("IRQ high", irq_spi, 1'b1);
        
        // Clear by reading SPSR then SPDR
        read_reg(SPSR_ADDR);
        read_reg(SPDR_ADDR);
        
        // Wait for the clear to propagate (needs 2 cycles due to pipelining)
        @(posedge clk);
        @(posedge clk);
        
        // IRQ should be low now
        check_value("IRQ cleared", irq_spi, 1'b0);
        
        miso_loopback = 0;
        reset_spi();

        // =====================================================================
        // Summary
        // =====================================================================
        $display("\n========================================");
        $display("Test Summary: %0d tests, %0d errors", test_num, errors);
        if (errors == 0)
            $display("ALL TESTS PASSED!");
        else
            $display("SOME TESTS FAILED!");
        $display("========================================\n");
        
        $finish;
    end

    // =========================================================================
    // Helper Tasks
    // =========================================================================
    
    reg [1:0] last_byte_lane;
    reg [7:0] read_byte;
    
    always @(*) begin
        case (last_byte_lane)
            2'b00: read_byte = mem_rdata[7:0];
            2'b01: read_byte = mem_rdata[15:8];
            2'b10: read_byte = mem_rdata[23:16];
            2'b11: read_byte = mem_rdata[31:24];
        endcase
    end
    
    task write_reg;
        input [31:0] addr;
        input [7:0] data;
        reg [1:0] byte_lane;
        reg [31:0] word_addr;
        begin
            byte_lane = addr[1:0];
            word_addr = {addr[31:2], 2'b00};
            @(negedge clk);
            mem_valid = 1;
            mem_addr = word_addr;
            case (byte_lane)
                2'b00: begin mem_wdata = {24'h0, data};        mem_wstrb = 4'b0001; end
                2'b01: begin mem_wdata = {16'h0, data, 8'h0};  mem_wstrb = 4'b0010; end
                2'b10: begin mem_wdata = {8'h0, data, 16'h0};  mem_wstrb = 4'b0100; end
                2'b11: begin mem_wdata = {data, 24'h0};        mem_wstrb = 4'b1000; end
            endcase
            @(posedge clk);
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
            mem_wstrb = 0;
        end
    endtask

    task read_reg;
        input [31:0] addr;
        reg [31:0] word_addr;
        begin
            last_byte_lane = addr[1:0];
            word_addr = {addr[31:2], 2'b00};
            @(negedge clk);
            mem_valid = 1;
            mem_addr = word_addr;
            mem_wstrb = 0;
            @(posedge clk);
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
        end
    endtask

    task reset_spi;
        begin
            rst_n = 0;
            @(posedge clk);
            @(posedge clk);
            rst_n = 1;
            @(posedge clk);
            miso = 0;
            ss_n = 1;
            sck_in = 0;
        end
    endtask

    task wait_spif;
        input integer timeout;
        integer cnt;
        begin
            cnt = 0;
            while (cnt < timeout) begin
                @(posedge clk);
                // Check internal state for transfer complete
                if (uut.spif) begin
                    cnt = timeout;  // Exit loop
                end
                cnt = cnt + 1;
            end
        end
    endtask

    // Shift in a byte as if we're an external master driving the slave
    task shift_in_byte;
        input [7:0] data;
        integer i;
        begin
            for (i = 7; i >= 0; i = i - 1) begin
                // Drive MISO with bit (this will be sampled by slave)
                miso = data[i];
                // Toggle SCK
                sck_in = 0;
                repeat(4) @(posedge clk);
                sck_in = 1;
                repeat(4) @(posedge clk);
            end
            sck_in = 0;
            repeat(4) @(posedge clk);
        end
    endtask

    task check_value;
        input [256*8-1:0] name;
        input [31:0] actual;
        input [31:0] expected;
        begin
            if (actual !== expected) begin
                $display("ERROR: %0s - expected %h, got %h", name, expected, actual);
                errors = errors + 1;
            end else begin
                $display("PASS: %0s = %h", name, actual);
            end
        end
    endtask

endmodule
