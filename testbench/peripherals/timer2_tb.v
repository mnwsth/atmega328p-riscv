// Timer/Counter 2 Unit Testbench
// Comprehensive tests for ATmega328P-compatible Timer2
// 
// Timer2 Differences from Timer0:
// - Different prescaler options: 1, 8, 32, 64, 128, 256, 1024
// - No external clock input
// - Different register addresses: TCCR2A=0xB0, TCCR2B=0xB1, etc.
`timescale 1ns / 1ps

module timer2_tb;

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
    wire        oc2a;
    wire        oc2b;
    wire        irq_ovf;
    wire        irq_compa;
    wire        irq_compb;

    // Register addresses (ATmega328P Timer2)
    localparam TIFR2_ADDR  = 32'h20000037;
    localparam TIMSK2_ADDR = 32'h20000070;
    localparam TCCR2A_ADDR = 32'h200000B0;
    localparam TCCR2B_ADDR = 32'h200000B1;
    localparam TCNT2_ADDR  = 32'h200000B2;
    localparam OCR2A_ADDR  = 32'h200000B3;
    localparam OCR2B_ADDR  = 32'h200000B4;

    // Clock Select values (Timer2 specific - different from Timer0!)
    localparam CS_STOP   = 3'b000;  // Stopped
    localparam CS_DIV1   = 3'b001;  // clk/1
    localparam CS_DIV8   = 3'b010;  // clk/8
    localparam CS_DIV32  = 3'b011;  // clk/32  (Timer2 only)
    localparam CS_DIV64  = 3'b100;  // clk/64
    localparam CS_DIV128 = 3'b101;  // clk/128 (Timer2 only)
    localparam CS_DIV256 = 3'b110;  // clk/256
    localparam CS_DIV1024= 3'b111;  // clk/1024

    // Waveform Generation Modes
    localparam WGM_NORMAL      = 3'b000;
    localparam WGM_PWM_PC_FF   = 3'b001;
    localparam WGM_CTC         = 3'b010;
    localparam WGM_FAST_PWM_FF = 3'b011;
    localparam WGM_PWM_PC_OCRA = 3'b101;
    localparam WGM_FAST_PWM_OCRA = 3'b111;

    // Instantiate DUT
    timer2 uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
        .oc2a(oc2a),
        .oc2b(oc2b),
        .irq_ovf(irq_ovf),
        .irq_compa(irq_compa),
        .irq_compb(irq_compb)
    );

    // Clock generation (10ns period = 100MHz)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // VCD dump
    initial begin
        $dumpfile("timer2_tb.vcd");
        $dumpvars(0, timer2_tb);
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
        
        // Reset
        #100;
        rst_n = 1;
        #20;

        // =====================================================================
        // Test 1: Reset Values
        // =====================================================================
        test_num = 1;
        $display("\n=== Test %0d: Reset Values ===", test_num);
        
        read_reg(TCCR2A_ADDR);
        check_value("TCCR2A reset", read_byte, 8'h00);
        
        read_reg(TCCR2B_ADDR);
        check_value("TCCR2B reset", read_byte, 8'h00);
        
        read_reg(TCNT2_ADDR);
        check_value("TCNT2 reset", read_byte, 8'h00);
        
        read_reg(OCR2A_ADDR);
        check_value("OCR2A reset", read_byte, 8'h00);
        
        read_reg(OCR2B_ADDR);
        check_value("OCR2B reset", read_byte, 8'h00);
        
        read_reg(TIMSK2_ADDR);
        check_value("TIMSK2 reset", read_byte, 8'h00);
        
        read_reg(TIFR2_ADDR);
        check_value("TIFR2 reset", read_byte, 8'h00);

        // =====================================================================
        // Test 2: Register Read/Write
        // =====================================================================
        test_num = 2;
        $display("\n=== Test %0d: Register Read/Write ===", test_num);
        
        // Write and readback all writable registers
        write_reg(TCCR2A_ADDR, 8'hC3); // COM2A=11, COM2B=00, WGM=11
        read_reg(TCCR2A_ADDR);
        check_value("TCCR2A write", read_byte, 8'hC3);
        
        write_reg(TCCR2B_ADDR, 8'h0D); // WGM22=1, CS=101
        read_reg(TCCR2B_ADDR);
        check_value("TCCR2B write", read_byte, 8'h0D);
        
        write_reg(TCNT2_ADDR, 8'hAA);
        read_reg(TCNT2_ADDR);
        check_value("TCNT2 write", read_byte, 8'hAA);
        
        write_reg(OCR2A_ADDR, 8'h55);
        read_reg(OCR2A_ADDR);
        check_value("OCR2A write", read_byte, 8'h55);
        
        write_reg(OCR2B_ADDR, 8'hBB);
        read_reg(OCR2B_ADDR);
        check_value("OCR2B write", read_byte, 8'hBB);
        
        write_reg(TIMSK2_ADDR, 8'h07); // All interrupt enables
        read_reg(TIMSK2_ADDR);
        check_value("TIMSK2 write", read_byte, 8'h07);
        
        // Reset for next tests
        reset_timer();

        // =====================================================================
        // Test 3: Timer Stopped (CS=000)
        // =====================================================================
        test_num = 3;
        $display("\n=== Test %0d: Timer Stopped (CS=000) ===", test_num);
        
        write_reg(TCNT2_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_STOP); // Timer stopped
        
        // Wait many cycles
        repeat(100) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        check_value("TCNT2 stopped", read_byte, 8'h00);

        // =====================================================================
        // Test 4: Normal Mode with Prescaler /1
        // =====================================================================
        test_num = 4;
        $display("\n=== Test %0d: Normal Mode, Prescaler /1 ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00); // Normal mode (WGM=000)
        write_reg(TCCR2B_ADDR, CS_DIV1); // clk/1
        
        // Let timer count
        repeat(10) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        // Should have counted ~10 (minus bus transaction time)
        if (read_byte < 8) begin
            $display("ERROR: TCNT2 should be >= 8, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 counted to %d", read_byte);
        end

        // =====================================================================
        // Test 5: Normal Mode Overflow
        // =====================================================================
        test_num = 5;
        $display("\n=== Test %0d: Normal Mode Overflow ===", test_num);
        reset_timer();
        
        write_reg(TCNT2_ADDR, 8'hFC); // Start near overflow
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h01); // Enable overflow interrupt
        
        // Wait for overflow (4 counts to 0xFF, then wrap)
        repeat(10) @(posedge clk);
        
        // Check overflow flag
        read_reg(TIFR2_ADDR);
        check_value("TOV2 set", read_byte[0], 1'b1);
        check_value("IRQ overflow", irq_ovf, 1'b1);
        
        // Clear flag by writing 1
        write_reg(TIFR2_ADDR, 8'h01);
        read_reg(TIFR2_ADDR);
        check_value("TOV2 cleared", read_byte[0], 1'b0);
        check_value("IRQ cleared", irq_ovf, 1'b0);

        // =====================================================================
        // Test 6: Prescaler /8
        // =====================================================================
        test_num = 6;
        $display("\n=== Test %0d: Prescaler /8 ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV8);
        
        // Count for 80 clocks = 10 timer ticks
        repeat(80) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /8 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /8 = %d", read_byte);
        end

        // =====================================================================
        // Test 7: Prescaler /32 (Timer2 specific)
        // =====================================================================
        test_num = 7;
        $display("\n=== Test %0d: Prescaler /32 (Timer2 specific) ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV32);
        
        // Count for 320 clocks = 10 timer ticks
        repeat(320) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /32 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /32 = %d", read_byte);
        end

        // =====================================================================
        // Test 8: Prescaler /64
        // =====================================================================
        test_num = 8;
        $display("\n=== Test %0d: Prescaler /64 ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV64);
        
        // Count for 640 clocks = 10 timer ticks
        repeat(640) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /64 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /64 = %d", read_byte);
        end

        // =====================================================================
        // Test 9: Prescaler /128 (Timer2 specific)
        // =====================================================================
        test_num = 9;
        $display("\n=== Test %0d: Prescaler /128 (Timer2 specific) ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV128);
        
        // Count for 1280 clocks = 10 timer ticks
        repeat(1280) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /128 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /128 = %d", read_byte);
        end

        // =====================================================================
        // Test 10: Prescaler /256
        // =====================================================================
        test_num = 10;
        $display("\n=== Test %0d: Prescaler /256 ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV256);
        
        // Count for 2560 clocks = 10 timer ticks
        repeat(2560) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /256 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /256 = %d", read_byte);
        end

        // =====================================================================
        // Test 11: Prescaler /1024
        // =====================================================================
        test_num = 11;
        $display("\n=== Test %0d: Prescaler /1024 ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1024);
        
        // Count for 10240 clocks = 10 timer ticks
        repeat(10240) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8 || read_byte > 12) begin
            $display("ERROR: TCNT2 with /1024 should be ~10, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 with /1024 = %d", read_byte);
        end

        // =====================================================================
        // Test 12: CTC Mode
        // =====================================================================
        test_num = 12;
        $display("\n=== Test %0d: CTC Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h0F); // TOP = 15
        write_reg(TCCR2A_ADDR, 8'h02); // WGM = 010 (CTC)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h02); // Enable compare A interrupt
        
        // Wait for compare match
        repeat(20) @(posedge clk);
        
        // Check compare match flag and verify TOV2 is NOT set
        read_reg(TIFR2_ADDR);
        check_value("OCF2A set in CTC", read_byte[1], 1'b1);
        check_value("TOV2 clear in CTC", read_byte[0], 1'b0);
        
        // Counter should have wrapped back
        read_reg(TCNT2_ADDR);
        if (read_byte > 8'h0F) begin
            $display("ERROR: TCNT2 should wrap at OCR2A, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 wrapped correctly, now at %d", read_byte);
        end

        // =====================================================================
        // Test 13: Compare Match A Detection
        // =====================================================================
        test_num = 13;
        $display("\n=== Test %0d: Compare Match A Detection ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h05);
        write_reg(TCCR2A_ADDR, 8'h00); // Normal mode
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h02); // Enable OCIE2A
        
        // Wait for match
        repeat(10) @(posedge clk);
        
        read_reg(TIFR2_ADDR);
        check_value("OCF2A set", read_byte[1], 1'b1);
        check_value("IRQ compare A", irq_compa, 1'b1);

        // =====================================================================
        // Test 14: Compare Match B Detection
        // =====================================================================
        test_num = 14;
        $display("\n=== Test %0d: Compare Match B Detection ===", test_num);
        reset_timer();
        
        write_reg(OCR2B_ADDR, 8'h07);
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h04); // Enable OCIE2B
        
        // Wait for match
        repeat(12) @(posedge clk);
        
        read_reg(TIFR2_ADDR);
        check_value("OCF2B set", read_byte[2], 1'b1);
        check_value("IRQ compare B", irq_compb, 1'b1);

        // =====================================================================
        // Test 15: Output Compare Toggle Mode
        // =====================================================================
        test_num = 15;
        $display("\n=== Test %0d: OC2A Toggle Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h04);
        write_reg(TCCR2A_ADDR, 8'h40); // COM2A=01 (toggle), Normal mode
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Initial state
        check_value("OC2A initial", oc2a, 1'b0);
        
        // Wait for first match
        repeat(8) @(posedge clk);
        check_value("OC2A toggled 1", oc2a, 1'b1);
        
        // Count through 0xFF back to match
        repeat(256) @(posedge clk);
        check_value("OC2A toggled 2", oc2a, 1'b0);

        // =====================================================================
        // Test 16: Output Compare Clear Mode
        // =====================================================================
        test_num = 16;
        $display("\n=== Test %0d: OC2A Clear on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h05);
        write_reg(TCCR2A_ADDR, 8'h80); // COM2A=10 (clear on match)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC2A cleared", oc2a, 1'b0);

        // =====================================================================
        // Test 17: Output Compare Set Mode
        // =====================================================================
        test_num = 17;
        $display("\n=== Test %0d: OC2A Set on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h05);
        write_reg(TCCR2A_ADDR, 8'hC0); // COM2A=11 (set on match)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC2A set", oc2a, 1'b1);

        // =====================================================================
        // Test 18: Fast PWM Mode
        // =====================================================================
        test_num = 18;
        $display("\n=== Test %0d: Fast PWM Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h7F); // 50% duty cycle
        write_reg(TCCR2A_ADDR, 8'h83); // COM2A=10 (non-inverting), WGM=011 (Fast PWM)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Run through a complete PWM cycle
        repeat(260) @(posedge clk);
        
        // Check that overflow occurred at TOP=0xFF
        read_reg(TIFR2_ADDR);
        check_value("TOV2 set in Fast PWM", read_byte[0], 1'b1);

        // =====================================================================
        // Test 19: Fast PWM with OCR2A as TOP
        // =====================================================================
        test_num = 19;
        $display("\n=== Test %0d: Fast PWM with OCR2A as TOP ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h1F); // TOP = 31
        write_reg(OCR2B_ADDR, 8'h0F); // 50% duty
        write_reg(TCCR2A_ADDR, 8'h23); // COM2B=10, WGM=011
        write_reg(TCCR2B_ADDR, 8'h09); // WGM22=1, CS=001 -> WGM=111
        
        // Run through a PWM cycle
        repeat(40) @(posedge clk);
        
        // Counter should wrap at 31
        read_reg(TCNT2_ADDR);
        if (read_byte > 8'h1F) begin
            $display("ERROR: TCNT2 should wrap at OCR2A=31, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 wrapped at OCR2A");
        end

        // =====================================================================
        // Test 20: Interrupt Mask/Enable
        // =====================================================================
        test_num = 20;
        $display("\n=== Test %0d: Interrupt Mask/Enable ===", test_num);
        reset_timer();
        
        // Set up for overflow but disable interrupt
        write_reg(TCNT2_ADDR, 8'hFE);
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h00); // Interrupts disabled
        
        repeat(5) @(posedge clk);
        
        // Flag should be set but IRQ should be low
        read_reg(TIFR2_ADDR);
        check_value("TOV2 flag set", read_byte[0], 1'b1);
        check_value("IRQ masked", irq_ovf, 1'b0);
        
        // Enable interrupt
        write_reg(TIMSK2_ADDR, 8'h01);
        #10;
        check_value("IRQ unmasked", irq_ovf, 1'b1);

        // =====================================================================
        // Test 21: Write to TCNT2 While Running
        // =====================================================================
        test_num = 21;
        $display("\n=== Test %0d: Write TCNT2 While Running ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        repeat(20) @(posedge clk);
        
        // Write new value
        write_reg(TCNT2_ADDR, 8'hF0);
        
        read_reg(TCNT2_ADDR);
        if (read_byte < 8'hF0) begin
            $display("ERROR: TCNT2 write while running failed, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 write while running = %d", read_byte);
        end

        // =====================================================================
        // Test 22: Force Output Compare
        // =====================================================================
        test_num = 22;
        $display("\n=== Test %0d: Force Output Compare ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h40); // COM2A=01 (toggle)
        // Timer stopped, FOC2A will toggle OC2A
        
        check_value("OC2A before FOC", oc2a, 1'b0);
        
        write_reg(TCCR2B_ADDR, 8'h80); // FOC2A=1, timer stopped
        #20;
        check_value("OC2A after FOC", oc2a, 1'b1);

        // =====================================================================
        // Test 23: Multiple Flags Set
        // =====================================================================
        test_num = 23;
        $display("\n=== Test %0d: Multiple Flags Set ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h05);
        write_reg(OCR2B_ADDR, 8'h05); // Same value
        write_reg(TCNT2_ADDR, 8'hFC);  // Near overflow
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Wait for all events
        repeat(15) @(posedge clk);
        
        read_reg(TIFR2_ADDR);
        check_value("TOV2 set", read_byte[0], 1'b1);
        check_value("OCF2A set", read_byte[1], 1'b1);
        check_value("OCF2B set", read_byte[2], 1'b1);
        
        // Clear all flags
        write_reg(TIFR2_ADDR, 8'h07);
        read_reg(TIFR2_ADDR);
        check_value("All flags cleared", read_byte[2:0], 3'b000);

        // =====================================================================
        // Test 24: OC2B Output Modes
        // =====================================================================
        test_num = 24;
        $display("\n=== Test %0d: OC2B Toggle Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR2B_ADDR, 8'h04);
        write_reg(TCCR2A_ADDR, 8'h10); // COM2B=01 (toggle)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        check_value("OC2B initial", oc2b, 1'b0);
        
        repeat(8) @(posedge clk);
        check_value("OC2B toggled", oc2b, 1'b1);

        // =====================================================================
        // Test 25: Phase Correct PWM Mode
        // =====================================================================
        test_num = 25;
        $display("\n=== Test %0d: Phase Correct PWM Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h7F); // 50% duty
        write_reg(TCCR2A_ADDR, 8'h81); // COM2A=10, WGM=001 (Phase Correct PWM)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h01); // Enable overflow interrupt
        
        // Run through a complete cycle (up to 255, down to 0)
        repeat(520) @(posedge clk);
        
        // Overflow should occur at BOTTOM in phase correct mode
        read_reg(TIFR2_ADDR);
        check_value("TOV2 at BOTTOM", read_byte[0], 1'b1);

        // =====================================================================
        // Test 26: CTC Mode - TOV2 NOT set when OCR2A < 0xFF
        // =====================================================================
        test_num = 26;
        $display("\n=== Test %0d: CTC Mode - TOV2 not set (OCR2A < 0xFF) ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h0F);  // TOP = 15 (not 0xFF)
        write_reg(TCCR2A_ADDR, 8'h02); // WGM = 010 (CTC mode)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h01); // Enable overflow interrupt
        
        // Run through multiple CTC cycles (count 0->15, clear, repeat)
        repeat(50) @(posedge clk);
        
        // Check that TOV2 is NOT set (counter never reaches 0xFF)
        read_reg(TIFR2_ADDR);
        if (read_byte[0] == 1'b0) begin
            $display("PASS: TOV2 not set in CTC mode (OCR2A < 0xFF)");
        end else begin
            $display("ERROR: TOV2 should NOT be set in CTC mode when OCR2A < 0xFF");
            errors = errors + 1;
        end

        // =====================================================================
        // Test 27: CTC Mode - TOV2 IS set when OCR2A = 0xFF
        // =====================================================================
        test_num = 27;
        $display("\n=== Test %0d: CTC Mode - TOV2 set (OCR2A = 0xFF) ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'hFF);  // TOP = 255 (MAX)
        write_reg(TCCR2A_ADDR, 8'h02); // WGM = 010 (CTC mode)
        write_reg(TCNT2_ADDR, 8'hFD);  // Start near MAX to speed up test
        write_reg(TCCR2B_ADDR, CS_DIV1);
        write_reg(TIMSK2_ADDR, 8'h01); // Enable overflow interrupt
        
        // Wait for counter to reach 0xFF and wrap
        repeat(10) @(posedge clk);
        
        // Check that TOV2 IS set (counter reaches 0xFF and wraps)
        read_reg(TIFR2_ADDR);
        if (read_byte[0] == 1'b1) begin
            $display("PASS: TOV2 set in CTC mode (OCR2A = 0xFF)");
        end else begin
            $display("ERROR: TOV2 should be set in CTC mode when OCR2A = 0xFF");
            errors = errors + 1;
        end

        // =====================================================================
        // Test 28: Force Output Compare B
        // =====================================================================
        test_num = 28;
        $display("\n=== Test %0d: Force Output Compare B ===", test_num);
        reset_timer();
        
        write_reg(TCCR2A_ADDR, 8'h10); // COM2B=01 (toggle)
        
        check_value("OC2B before FOC", oc2b, 1'b0);
        
        write_reg(TCCR2B_ADDR, 8'h40); // FOC2B=1, timer stopped
        #20;
        check_value("OC2B after FOC", oc2b, 1'b1);

        // =====================================================================
        // Test 29: OC2B Clear on Match
        // =====================================================================
        test_num = 29;
        $display("\n=== Test %0d: OC2B Clear on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR2B_ADDR, 8'h05);
        write_reg(TCCR2A_ADDR, 8'h20); // COM2B=10 (clear on match)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC2B cleared", oc2b, 1'b0);

        // =====================================================================
        // Test 30: OC2B Set on Match
        // =====================================================================
        test_num = 30;
        $display("\n=== Test %0d: OC2B Set on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR2B_ADDR, 8'h05);
        write_reg(TCCR2A_ADDR, 8'h30); // COM2B=11 (set on match)
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC2B set", oc2b, 1'b1);

        // =====================================================================
        // Test 31: Phase Correct PWM with OCR2A as TOP
        // =====================================================================
        test_num = 31;
        $display("\n=== Test %0d: Phase Correct PWM with OCR2A as TOP ===", test_num);
        reset_timer();
        
        write_reg(OCR2A_ADDR, 8'h1F); // TOP = 31
        write_reg(OCR2B_ADDR, 8'h0F); // 50% duty
        write_reg(TCCR2A_ADDR, 8'h21); // COM2B=10, WGM=001
        write_reg(TCCR2B_ADDR, 8'h09); // WGM22=1, CS=001 -> WGM=101
        write_reg(TIMSK2_ADDR, 8'h01); // Enable overflow interrupt
        
        // Run through complete up/down cycle (0->31->0)
        repeat(70) @(posedge clk);
        
        // Counter should be below TOP
        read_reg(TCNT2_ADDR);
        if (read_byte > 8'h1F) begin
            $display("ERROR: TCNT2 should be <= OCR2A=31, got %d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT2 within bounds = %d", read_byte);
        end

        // =====================================================================
        // Test 32: All Prescaler Transitions
        // =====================================================================
        test_num = 32;
        $display("\n=== Test %0d: Prescaler Transitions ===", test_num);
        reset_timer();
        
        // Start with /1, then switch to /32
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, CS_DIV1);
        
        repeat(10) @(posedge clk);
        read_reg(TCNT2_ADDR);
        if (read_byte < 5) begin
            $display("ERROR: /1 prescaler not working");
            errors = errors + 1;
        end else begin
            $display("PASS: /1 prescaler working, count=%d", read_byte);
        end
        
        // Stop timer first, then reset counter, then switch to /32
        // This ensures clean prescaler transition
        write_reg(TCCR2B_ADDR, CS_STOP);  // Stop timer
        write_reg(TCNT2_ADDR, 8'h00);     // Reset counter
        write_reg(TCCR2B_ADDR, CS_DIV32); // Start with /32
        
        repeat(96) @(posedge clk);  // 96 clocks = 3 timer ticks at /32
        read_reg(TCNT2_ADDR);
        if (read_byte < 2 || read_byte > 4) begin
            $display("ERROR: /32 prescaler transition failed, count=%d", read_byte);
            errors = errors + 1;
        end else begin
            $display("PASS: /32 prescaler working, count=%d", read_byte);
        end

        // =====================================================================
        // Test 33: Verify No External Clock (Timer2 vs Timer0)
        // Timer2 should NOT have external clock capability
        // =====================================================================
        test_num = 33;
        $display("\n=== Test %0d: Timer2 has no external clock ===", test_num);
        reset_timer();
        
        // CS=110 in Timer2 is /256 (not external clock like Timer0)
        // CS=111 in Timer2 is /1024 (not external clock like Timer0)
        write_reg(TCCR2A_ADDR, 8'h00);
        write_reg(TCCR2B_ADDR, 3'b110); // This is /256 in Timer2
        
        // Wait for 2560 clocks (10 timer ticks at /256)
        repeat(2560) @(posedge clk);
        
        read_reg(TCNT2_ADDR);
        if (read_byte >= 8 && read_byte <= 12) begin
            $display("PASS: CS=110 is /256 prescaler (Timer2 specific), count=%d", read_byte);
        end else begin
            $display("ERROR: CS=110 should be /256, got count=%d", read_byte);
            errors = errors + 1;
        end

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
    
    // Store byte lane for read_byte extraction
    reg [1:0] last_byte_lane;
    
    // Read data from correct byte lane based on last accessed address
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
            word_addr = {addr[31:2], 2'b00};  // Word-align the address
            // Set inputs at negedge so DUT sees them at next posedge
            @(negedge clk);
            mem_valid = 1;
            mem_addr = word_addr;  // Use word-aligned address
            // Place data in correct byte lane and set correct wstrb
            case (byte_lane)
                2'b00: begin mem_wdata = {24'h0, data};        mem_wstrb = 4'b0001; end
                2'b01: begin mem_wdata = {16'h0, data, 8'h0};  mem_wstrb = 4'b0010; end
                2'b10: begin mem_wdata = {8'h0, data, 16'h0};  mem_wstrb = 4'b0100; end
                2'b11: begin mem_wdata = {data, 24'h0};        mem_wstrb = 4'b1000; end
            endcase
            @(posedge clk);  // DUT samples here
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
            last_byte_lane = addr[1:0];  // Remember byte lane for read_byte extraction
            word_addr = {addr[31:2], 2'b00};  // Word-align the address
            @(negedge clk);
            mem_valid = 1;
            mem_addr = word_addr;  // Use word-aligned address
            mem_wstrb = 0;
            @(posedge clk);
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
        end
    endtask

    task reset_timer;
        begin
            // Full hardware reset to clear all state including OC outputs
            rst_n = 0;
            @(posedge clk);
            @(posedge clk);
            rst_n = 1;
            @(posedge clk);
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
