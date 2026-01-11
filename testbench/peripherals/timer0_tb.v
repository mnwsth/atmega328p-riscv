// Timer/Counter 0 Unit Testbench
// Comprehensive tests for ATmega328P-compatible Timer0
`timescale 1ns / 1ps

module timer0_tb;

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
    reg         t0_pin;
    wire        oc0a;
    wire        oc0b;
    wire        irq_ovf;
    wire        irq_compa;
    wire        irq_compb;

    // Register addresses
    localparam TIFR0_ADDR  = 32'h20000035;
    localparam TCCR0A_ADDR = 32'h20000044;
    localparam TCCR0B_ADDR = 32'h20000045;
    localparam TCNT0_ADDR  = 32'h20000046;
    localparam OCR0A_ADDR  = 32'h20000047;
    localparam OCR0B_ADDR  = 32'h20000048;
    localparam TIMSK0_ADDR = 32'h2000006E;

    // Clock Select values
    localparam CS_STOP   = 3'b000;
    localparam CS_DIV1   = 3'b001;
    localparam CS_DIV8   = 3'b010;
    localparam CS_DIV64  = 3'b011;
    localparam CS_DIV256 = 3'b100;
    localparam CS_DIV1024= 3'b101;
    localparam CS_EXT_F  = 3'b110;
    localparam CS_EXT_R  = 3'b111;

    // Waveform Generation Modes
    localparam WGM_NORMAL      = 3'b000;
    localparam WGM_PWM_PC_FF   = 3'b001;
    localparam WGM_CTC         = 3'b010;
    localparam WGM_FAST_PWM_FF = 3'b011;
    localparam WGM_PWM_PC_OCRA = 3'b101;
    localparam WGM_FAST_PWM_OCRA = 3'b111;

    // Instantiate DUT
    timer0 uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
        .t0_pin(t0_pin),
        .oc0a(oc0a),
        .oc0b(oc0b),
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
        $dumpfile("timer0_tb.vcd");
        $dumpvars(0, timer0_tb);
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
        t0_pin = 0;
        
        // Reset
        #100;
        rst_n = 1;
        #20;

        // =====================================================================
        // Test 1: Reset Values
        // =====================================================================
        test_num = 1;
        $display("\n=== Test %0d: Reset Values ===", test_num);
        
        read_reg(TCCR0A_ADDR);
        check_value("TCCR0A reset", mem_rdata[7:0], 8'h00);
        
        read_reg(TCCR0B_ADDR);
        check_value("TCCR0B reset", mem_rdata[7:0], 8'h00);
        
        read_reg(TCNT0_ADDR);
        check_value("TCNT0 reset", mem_rdata[7:0], 8'h00);
        
        read_reg(OCR0A_ADDR);
        check_value("OCR0A reset", mem_rdata[7:0], 8'h00);
        
        read_reg(OCR0B_ADDR);
        check_value("OCR0B reset", mem_rdata[7:0], 8'h00);
        
        read_reg(TIMSK0_ADDR);
        check_value("TIMSK0 reset", mem_rdata[7:0], 8'h00);
        
        read_reg(TIFR0_ADDR);
        check_value("TIFR0 reset", mem_rdata[7:0], 8'h00);

        // =====================================================================
        // Test 2: Register Read/Write
        // =====================================================================
        test_num = 2;
        $display("\n=== Test %0d: Register Read/Write ===", test_num);
        
        // Write and readback all writable registers
        write_reg(TCCR0A_ADDR, 8'hC3); // COM0A=11, COM0B=00, WGM=11
        read_reg(TCCR0A_ADDR);
        check_value("TCCR0A write", mem_rdata[7:0], 8'hC3);
        
        write_reg(TCCR0B_ADDR, 8'h0D); // WGM02=1, CS=101
        read_reg(TCCR0B_ADDR);
        check_value("TCCR0B write", mem_rdata[7:0], 8'h0D);
        
        write_reg(TCNT0_ADDR, 8'hAA);
        read_reg(TCNT0_ADDR);
        check_value("TCNT0 write", mem_rdata[7:0], 8'hAA);
        
        write_reg(OCR0A_ADDR, 8'h55);
        read_reg(OCR0A_ADDR);
        check_value("OCR0A write", mem_rdata[7:0], 8'h55);
        
        write_reg(OCR0B_ADDR, 8'hBB);
        read_reg(OCR0B_ADDR);
        check_value("OCR0B write", mem_rdata[7:0], 8'hBB);
        
        write_reg(TIMSK0_ADDR, 8'h07); // All interrupt enables
        read_reg(TIMSK0_ADDR);
        check_value("TIMSK0 write", mem_rdata[7:0], 8'h07);
        
        // Reset for next tests
        reset_timer();

        // =====================================================================
        // Test 3: Timer Stopped (CS=000)
        // =====================================================================
        test_num = 3;
        $display("\n=== Test %0d: Timer Stopped (CS=000) ===", test_num);
        
        write_reg(TCNT0_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_STOP); // Timer stopped
        
        // Wait many cycles
        repeat(100) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        check_value("TCNT0 stopped", mem_rdata[7:0], 8'h00);

        // =====================================================================
        // Test 4: Normal Mode with Prescaler /1
        // =====================================================================
        test_num = 4;
        $display("\n=== Test %0d: Normal Mode, Prescaler /1 ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00); // Normal mode (WGM=000)
        write_reg(TCCR0B_ADDR, CS_DIV1); // clk/1
        
        // Let timer count
        repeat(10) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        // Should have counted ~10 (minus bus transaction time)
        if (mem_rdata[7:0] < 8) begin
            $display("ERROR: TCNT0 should be >= 8, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 counted to %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 5: Normal Mode Overflow
        // =====================================================================
        test_num = 5;
        $display("\n=== Test %0d: Normal Mode Overflow ===", test_num);
        reset_timer();
        
        write_reg(TCNT0_ADDR, 8'hFC); // Start near overflow
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h01); // Enable overflow interrupt
        
        // Wait for overflow (4 counts to 0xFF, then wrap)
        repeat(10) @(posedge clk);
        
        // Check overflow flag
        read_reg(TIFR0_ADDR);
        check_value("TOV0 set", mem_rdata[0], 1'b1);
        check_value("IRQ overflow", irq_ovf, 1'b1);
        
        // Clear flag by writing 1
        write_reg(TIFR0_ADDR, 8'h01);
        read_reg(TIFR0_ADDR);
        check_value("TOV0 cleared", mem_rdata[0], 1'b0);
        check_value("IRQ cleared", irq_ovf, 1'b0);

        // =====================================================================
        // Test 6: Prescaler /8
        // =====================================================================
        test_num = 6;
        $display("\n=== Test %0d: Prescaler /8 ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV8);
        
        // Count for 80 clocks = 10 timer ticks
        repeat(80) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 8 || mem_rdata[7:0] > 12) begin
            $display("ERROR: TCNT0 with /8 should be ~10, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with /8 = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 7: Prescaler /64
        // =====================================================================
        test_num = 7;
        $display("\n=== Test %0d: Prescaler /64 ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV64);
        
        // Count for 640 clocks = 10 timer ticks
        repeat(640) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 8 || mem_rdata[7:0] > 12) begin
            $display("ERROR: TCNT0 with /64 should be ~10, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with /64 = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 8: CTC Mode
        // =====================================================================
        test_num = 8;
        $display("\n=== Test %0d: CTC Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h0F); // TOP = 15
        write_reg(TCCR0A_ADDR, 8'h02); // WGM = 010 (CTC)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h02); // Enable compare A interrupt
        
        // Wait for compare match
        repeat(20) @(posedge clk);
        
        // Check compare match flag and verify TOV0 is NOT set
        // Per ATmega328P datasheet: overflow flag should not be set in CTC mode
        // when OCR0A < 0xFF (counter never reaches MAX)
        read_reg(TIFR0_ADDR);
        check_value("OCF0A set in CTC", mem_rdata[1], 1'b1);
        check_value("TOV0 clear in CTC", mem_rdata[0], 1'b0);
        
        // Counter should have wrapped back
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] > 8'h0F) begin
            $display("ERROR: TCNT0 should wrap at OCR0A, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 wrapped correctly, now at %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 9: Compare Match A Detection
        // =====================================================================
        test_num = 9;
        $display("\n=== Test %0d: Compare Match A Detection ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h05);
        write_reg(TCCR0A_ADDR, 8'h00); // Normal mode
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h02); // Enable OCIE0A
        
        // Wait for match
        repeat(10) @(posedge clk);
        
        read_reg(TIFR0_ADDR);
        check_value("OCF0A set", mem_rdata[1], 1'b1);
        check_value("IRQ compare A", irq_compa, 1'b1);

        // =====================================================================
        // Test 10: Compare Match B Detection
        // =====================================================================
        test_num = 10;
        $display("\n=== Test %0d: Compare Match B Detection ===", test_num);
        reset_timer();
        
        write_reg(OCR0B_ADDR, 8'h07);
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h04); // Enable OCIE0B
        
        // Wait for match
        repeat(12) @(posedge clk);
        
        read_reg(TIFR0_ADDR);
        check_value("OCF0B set", mem_rdata[2], 1'b1);
        check_value("IRQ compare B", irq_compb, 1'b1);

        // =====================================================================
        // Test 11: Output Compare Toggle Mode
        // =====================================================================
        test_num = 11;
        $display("\n=== Test %0d: OC0A Toggle Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h04);
        write_reg(TCCR0A_ADDR, 8'h40); // COM0A=01 (toggle), Normal mode
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        // Initial state
        check_value("OC0A initial", oc0a, 1'b0);
        
        // Wait for first match
        repeat(8) @(posedge clk);
        check_value("OC0A toggled 1", oc0a, 1'b1);
        
        // Count through 0xFF back to match
        repeat(256) @(posedge clk);
        check_value("OC0A toggled 2", oc0a, 1'b0);

        // =====================================================================
        // Test 12: Output Compare Clear Mode
        // =====================================================================
        test_num = 12;
        $display("\n=== Test %0d: OC0A Clear on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h05);
        write_reg(TCCR0A_ADDR, 8'h80); // COM0A=10 (clear on match)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC0A cleared", oc0a, 1'b0);

        // =====================================================================
        // Test 13: Output Compare Set Mode
        // =====================================================================
        test_num = 13;
        $display("\n=== Test %0d: OC0A Set on Match ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h05);
        write_reg(TCCR0A_ADDR, 8'hC0); // COM0A=11 (set on match)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        // Wait for match
        repeat(10) @(posedge clk);
        check_value("OC0A set", oc0a, 1'b1);

        // =====================================================================
        // Test 14: Fast PWM Mode
        // =====================================================================
        test_num = 14;
        $display("\n=== Test %0d: Fast PWM Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h7F); // 50% duty cycle
        write_reg(TCCR0A_ADDR, 8'h83); // COM0A=10 (non-inverting), WGM=011 (Fast PWM)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        // Run through a complete PWM cycle
        repeat(260) @(posedge clk);
        
        // Check that overflow occurred at TOP=0xFF
        read_reg(TIFR0_ADDR);
        check_value("TOV0 set in Fast PWM", mem_rdata[0], 1'b1);

        // =====================================================================
        // Test 15: Fast PWM with OCRA as TOP
        // =====================================================================
        test_num = 15;
        $display("\n=== Test %0d: Fast PWM with OCR0A as TOP ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h1F); // TOP = 31
        write_reg(OCR0B_ADDR, 8'h0F); // 50% duty
        write_reg(TCCR0A_ADDR, 8'h23); // COM0B=10, WGM=011
        write_reg(TCCR0B_ADDR, 8'h09); // WGM02=1, CS=001 -> WGM=111
        
        // Run through a PWM cycle
        repeat(40) @(posedge clk);
        
        // Counter should wrap at 31
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] > 8'h1F) begin
            $display("ERROR: TCNT0 should wrap at OCR0A=31, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 wrapped at OCR0A");
        end

        // =====================================================================
        // Test 16: External Clock Falling Edge
        // =====================================================================
        test_num = 16;
        $display("\n=== Test %0d: External Clock Falling Edge ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_EXT_F); // External falling edge
        
        // Generate 5 falling edges
        repeat(5) begin
            t0_pin = 1;
            repeat(4) @(posedge clk);
            t0_pin = 0;
            repeat(4) @(posedge clk);
        end
        
        // Account for synchronizer delay
        repeat(4) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 3 || mem_rdata[7:0] > 6) begin
            $display("ERROR: TCNT0 with ext falling should be ~5, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with ext falling = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 17: External Clock Rising Edge
        // =====================================================================
        test_num = 17;
        $display("\n=== Test %0d: External Clock Rising Edge ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_EXT_R); // External rising edge
        
        // Generate 5 rising edges
        t0_pin = 0;
        repeat(4) @(posedge clk);
        repeat(5) begin
            t0_pin = 1;
            repeat(4) @(posedge clk);
            t0_pin = 0;
            repeat(4) @(posedge clk);
        end
        
        // Account for synchronizer delay
        repeat(4) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 3 || mem_rdata[7:0] > 6) begin
            $display("ERROR: TCNT0 with ext rising should be ~5, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with ext rising = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 18: Interrupt Mask/Enable
        // =====================================================================
        test_num = 18;
        $display("\n=== Test %0d: Interrupt Mask/Enable ===", test_num);
        reset_timer();
        
        // Set up for overflow but disable interrupt
        write_reg(TCNT0_ADDR, 8'hFE);
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h00); // Interrupts disabled
        
        repeat(5) @(posedge clk);
        
        // Flag should be set but IRQ should be low
        read_reg(TIFR0_ADDR);
        check_value("TOV0 flag set", mem_rdata[0], 1'b1);
        check_value("IRQ masked", irq_ovf, 1'b0);
        
        // Enable interrupt
        write_reg(TIMSK0_ADDR, 8'h01);
        #10;
        check_value("IRQ unmasked", irq_ovf, 1'b1);

        // =====================================================================
        // Test 19: Write to TCNT0 While Running
        // =====================================================================
        test_num = 19;
        $display("\n=== Test %0d: Write TCNT0 While Running ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        repeat(20) @(posedge clk);
        
        // Write new value
        write_reg(TCNT0_ADDR, 8'hF0);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 8'hF0) begin
            $display("ERROR: TCNT0 write while running failed, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 write while running = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 20: Force Output Compare
        // =====================================================================
        test_num = 20;
        $display("\n=== Test %0d: Force Output Compare ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h40); // COM0A=01 (toggle)
        // Timer stopped, FOC0A will toggle OC0A
        
        check_value("OC0A before FOC", oc0a, 1'b0);
        
        write_reg(TCCR0B_ADDR, 8'h80); // FOC0A=1, timer stopped
        #20;
        check_value("OC0A after FOC", oc0a, 1'b1);

        // =====================================================================
        // Test 21: Multiple Flags Set
        // =====================================================================
        test_num = 21;
        $display("\n=== Test %0d: Multiple Flags Set ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h05);
        write_reg(OCR0B_ADDR, 8'h05); // Same value
        write_reg(TCNT0_ADDR, 8'hFC);  // Near overflow
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        // Wait for all events
        repeat(15) @(posedge clk);
        
        read_reg(TIFR0_ADDR);
        check_value("TOV0 set", mem_rdata[0], 1'b1);
        check_value("OCF0A set", mem_rdata[1], 1'b1);
        check_value("OCF0B set", mem_rdata[2], 1'b1);
        
        // Clear all flags
        write_reg(TIFR0_ADDR, 8'h07);
        read_reg(TIFR0_ADDR);
        check_value("All flags cleared", mem_rdata[2:0], 3'b000);

        // =====================================================================
        // Test 22: OC0B Output Modes
        // =====================================================================
        test_num = 22;
        $display("\n=== Test %0d: OC0B Toggle Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR0B_ADDR, 8'h04);
        write_reg(TCCR0A_ADDR, 8'h10); // COM0B=01 (toggle)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        
        check_value("OC0B initial", oc0b, 1'b0);
        
        repeat(8) @(posedge clk);
        check_value("OC0B toggled", oc0b, 1'b1);

        // =====================================================================
        // Test 23: Phase Correct PWM Mode
        // =====================================================================
        test_num = 23;
        $display("\n=== Test %0d: Phase Correct PWM Mode ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h7F); // 50% duty
        write_reg(TCCR0A_ADDR, 8'h81); // COM0A=10, WGM=001 (Phase Correct PWM)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h01); // Enable overflow interrupt
        
        // Run through a complete cycle (up to 255, down to 0)
        repeat(520) @(posedge clk);
        
        // Overflow should occur at BOTTOM in phase correct mode
        read_reg(TIFR0_ADDR);
        check_value("TOV0 at BOTTOM", mem_rdata[0], 1'b1);

        // =====================================================================
        // Test 24: Prescaler /256
        // =====================================================================
        test_num = 24;
        $display("\n=== Test %0d: Prescaler /256 ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV256);
        
        // Count for 2560 clocks = 10 timer ticks
        repeat(2560) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 8 || mem_rdata[7:0] > 12) begin
            $display("ERROR: TCNT0 with /256 should be ~10, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with /256 = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 25: Prescaler /1024
        // =====================================================================
        test_num = 25;
        $display("\n=== Test %0d: Prescaler /1024 ===", test_num);
        reset_timer();
        
        write_reg(TCCR0A_ADDR, 8'h00);
        write_reg(TCCR0B_ADDR, CS_DIV1024);
        
        // Count for 10240 clocks = 10 timer ticks
        repeat(10240) @(posedge clk);
        
        read_reg(TCNT0_ADDR);
        if (mem_rdata[7:0] < 8 || mem_rdata[7:0] > 12) begin
            $display("ERROR: TCNT0 with /1024 should be ~10, got %d", mem_rdata[7:0]);
            errors = errors + 1;
        end else begin
            $display("PASS: TCNT0 with /1024 = %d", mem_rdata[7:0]);
        end

        // =====================================================================
        // Test 26: CTC Mode - TOV0 NOT set when OCR0A < 0xFF
        // Per ATmega328P datasheet: In CTC mode, overflow only occurs at MAX
        // =====================================================================
        test_num = 26;
        $display("\n=== Test %0d: CTC Mode - TOV0 not set (OCR0A < 0xFF) ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'h0F);  // TOP = 15 (not 0xFF)
        write_reg(TCCR0A_ADDR, 8'h02); // WGM = 010 (CTC mode)
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h01); // Enable overflow interrupt
        
        // Run through multiple CTC cycles (count 0->15, clear, repeat)
        repeat(50) @(posedge clk);
        
        // Check that TOV0 is NOT set (counter never reaches 0xFF)
        read_reg(TIFR0_ADDR);
        if (mem_rdata[0] == 1'b0) begin
            $display("PASS: TOV0 not set in CTC mode (OCR0A < 0xFF)");
        end else begin
            $display("ERROR: TOV0 should NOT be set in CTC mode when OCR0A < 0xFF");
            errors = errors + 1;
        end

        // =====================================================================
        // Test 27: CTC Mode - TOV0 IS set when OCR0A = 0xFF
        // Per ATmega328P datasheet: When TOP=MAX, overflow occurs
        // =====================================================================
        test_num = 27;
        $display("\n=== Test %0d: CTC Mode - TOV0 set (OCR0A = 0xFF) ===", test_num);
        reset_timer();
        
        write_reg(OCR0A_ADDR, 8'hFF);  // TOP = 255 (MAX)
        write_reg(TCCR0A_ADDR, 8'h02); // WGM = 010 (CTC mode)
        write_reg(TCNT0_ADDR, 8'hFD);  // Start near MAX to speed up test
        write_reg(TCCR0B_ADDR, CS_DIV1);
        write_reg(TIMSK0_ADDR, 8'h01); // Enable overflow interrupt
        
        // Wait for counter to reach 0xFF and wrap
        repeat(10) @(posedge clk);
        
        // Check that TOV0 IS set (counter reaches 0xFF and wraps)
        read_reg(TIFR0_ADDR);
        if (mem_rdata[0] == 1'b1) begin
            $display("PASS: TOV0 set in CTC mode (OCR0A = 0xFF)");
        end else begin
            $display("ERROR: TOV0 should be set in CTC mode when OCR0A = 0xFF");
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
    
    task write_reg;
        input [31:0] addr;
        input [7:0] data;
        begin
            // Set inputs at negedge so DUT sees them at next posedge
            @(negedge clk);
            mem_valid = 1;
            mem_addr = addr;
            mem_wdata = {24'h0, data};
            mem_wstrb = 4'h1;
            @(posedge clk);  // DUT samples here
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
            mem_wstrb = 0;
        end
    endtask

    task read_reg;
        input [31:0] addr;
        begin
            @(negedge clk);
            mem_valid = 1;
            mem_addr = addr;
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
            t0_pin = 0;
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
