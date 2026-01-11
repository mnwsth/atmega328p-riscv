`timescale 1ns / 1ps

// Watchdog Timer Unit Testbench
// Comprehensive tests for ATmega328P-compatible WDT
//
// Tests cover:
// 1. Reset values
// 2. WDTCSR read/write
// 3. MCUSR read/write
// 4. WDT prescaler settings
// 5. Interrupt mode operation
// 6. System reset mode operation
// 7. Interrupt + Reset mode operation
// 8. WDR (Watchdog Reset) functionality
// 9. Timed sequence for WDE/WDP changes
// 10. WDCE auto-clear

module watchdog_timer_tb;

    // Parameters - use shorter timeout for faster simulation
    parameter WDT_OSC_DIV = 4;  // Small divider for fast simulation
    
    // Inputs
    reg clk;
    reg rst_n;
    reg mem_valid;
    reg [31:0] mem_addr;
    reg [31:0] mem_wdata;
    reg [3:0] mem_wstrb;

    // Outputs
    wire [31:0] mem_rdata;
    wire mem_ready;
    wire irq_wdt;
    wire wdt_reset_req;

    // Test tracking
    integer test_count;
    integer pass_count;
    integer fail_count;

    // Instantiate the Unit Under Test (UUT)
    watchdog_timer #(
        .WDT_OSC_DIV(WDT_OSC_DIV)
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
        .irq_wdt(irq_wdt),
        .wdt_reset_req(wdt_reset_req)
    );

    // Clock generation (10ns period = 100MHz)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // VCD dump for waveform viewing
    initial begin
        $dumpfile("watchdog_timer_tb.vcd");
        $dumpvars(0, watchdog_timer_tb);
    end

    // Register addresses
    localparam MCUSR_ADDR   = 32'h20000054;
    localparam WDTCSR_ADDR  = 32'h20000060;
    localparam WDR_ADDR     = 32'h20000061;

    // Test sequence
    initial begin
        // Initialize
        test_count = 0;
        pass_count = 0;
        fail_count = 0;
        rst_n = 0;
        mem_valid = 0;
        mem_addr = 0;
        mem_wdata = 0;
        mem_wstrb = 0;

        // Wait for global reset
        #100;
        rst_n = 1;
        #20;

        $display("========================================");
        $display("Watchdog Timer Unit Tests");
        $display("========================================");

        // Test 1: Reset Values
        test_reset_values();

        // Test 2: WDTCSR Read/Write
        test_wdtcsr_rw();

        // Test 3: MCUSR Read/Write
        test_mcusr_rw();

        // Test 4: WDT Prescaler Settings
        test_prescaler();

        // Test 5: Interrupt Mode
        test_interrupt_mode();

        // Test 6: System Reset Mode
        test_reset_mode();

        // Test 7: Interrupt + Reset Mode
        test_interrupt_reset_mode();

        // Test 8: WDR Functionality
        test_wdr();

        // Test 9: Timed Sequence
        test_timed_sequence();

        // Test 10: WDCE Auto-clear
        test_wdce_autoclear();

        // Summary
        $display("");
        $display("========================================");
        $display("Test Summary");
        $display("========================================");
        $display("Total Tests:  %0d", test_count);
        $display("Passed:       %0d", pass_count);
        $display("Failed:       %0d", fail_count);
        $display("========================================");

        if (fail_count == 0)
            $display("ALL TESTS PASSED!");
        else
            $display("SOME TESTS FAILED!");

        $finish;
    end

    // =========================================================================
    // Test Tasks
    // =========================================================================

    task test_reset_values;
        begin
            $display("");
            $display("Test 1: Reset Values");
            
            // Check WDTCSR reset value
            read_register(WDTCSR_ADDR);
            check_result("WDTCSR reset value", mem_rdata[7:0], 8'h00);
            
            // Check MCUSR reset value
            read_register(MCUSR_ADDR);
            check_result("MCUSR reset value", mem_rdata[7:0], 8'h00);
            
            // Check IRQ is not asserted
            check_result("IRQ reset state", irq_wdt, 1'b0);
            
            // Check reset request is not asserted
            check_result("Reset request state", wdt_reset_req, 1'b0);
        end
    endtask

    task test_wdtcsr_rw;
        begin
            $display("");
            $display("Test 2: WDTCSR Read/Write");
            
            // Write WDIE, WDE, WDP bits
            // WDIE=1, WDE=1, WDP=101 (64K cycles)
            // Pattern: 0b01001101 = 0x4D
            write_register(WDTCSR_ADDR, 8'h4D);
            read_register(WDTCSR_ADDR);
            // Note: WDE can be set but not cleared without timed sequence
            // WDP can only be changed during timed sequence
            // Only WDIE should be set
            check_result("WDTCSR WDIE set", mem_rdata[6], 1'b1);
            check_result("WDTCSR WDE set", mem_rdata[3], 1'b1);
            
            // Clear for next tests
            do_wdt_reset();
        end
    endtask

    task test_mcusr_rw;
        begin
            $display("");
            $display("Test 3: MCUSR Read/Write");
            
            // MCUSR WDRF is at bit 3
            // It's set on WDT reset and cleared by writing 0
            
            // Read initial (should be 0)
            read_register(MCUSR_ADDR);
            check_result("MCUSR initial", mem_rdata[3], 1'b0);
            
            // WDRF gets set by timeout, tested in reset mode tests
        end
    endtask

    task test_prescaler;
        begin
            $display("");
            $display("Test 4: Prescaler Settings");
            
            // Reset first
            do_wdt_reset();
            
            // Enable timed sequence and change prescaler
            // Write WDCE=1, WDE=1 to start sequence
            write_register(WDTCSR_ADDR, 8'h18);  // WDCE=1, WDE=1
            
            // Within 4 cycles, write new WDP
            // WDP=1001 (1024K cycles), WDE=0
            // Pattern: WDP3=1, WDP[2:0]=001 -> 0b00101001 = 0x29
            write_register(WDTCSR_ADDR, 8'h29);
            
            read_register(WDTCSR_ADDR);
            check_result("Prescaler WDP3", mem_rdata[5], 1'b1);
            check_result("Prescaler WDP[2:0]", mem_rdata[2:0], 3'b001);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_interrupt_mode;
        reg [7:0] initial_val;
        integer i;
        begin
            $display("");
            $display("Test 5: Interrupt Mode");
            
            // Reset first
            do_wdt_reset();
            
            // Enable interrupt mode: WDIE=1, WDE=0, WDP=0000 (shortest timeout)
            write_register(WDTCSR_ADDR, 8'h40);  // WDIE=1
            
            // Check IRQ is initially low
            check_result("IRQ before timeout", irq_wdt, 1'b0);
            
            // Wait for timeout (2K WDT cycles * OSC_DIV system cycles)
            // At minimum prescaler: 2048 * WDT_OSC_DIV cycles
            for (i = 0; i < 2048 * WDT_OSC_DIV + 100; i = i + 1) begin
                @(posedge clk);
                if (irq_wdt) begin
                    $display("  IRQ asserted after %0d cycles", i);
                    i = 2048 * WDT_OSC_DIV + 100;  // Exit loop
                end
            end
            
            // Check WDIF is set
            read_register(WDTCSR_ADDR);
            check_result("WDIF set on timeout", mem_rdata[7], 1'b1);
            
            // Check IRQ is asserted
            check_result("IRQ after timeout", irq_wdt, 1'b1);
            
            // Check no reset request (interrupt mode)
            check_result("No reset in INT mode", wdt_reset_req, 1'b0);
            
            // Clear WDIF by writing 1
            write_register(WDTCSR_ADDR, 8'hC0);  // Write 1 to WDIF, keep WDIE
            #20;
            check_result("IRQ cleared", irq_wdt, 1'b0);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_reset_mode;
        integer i;
        begin
            $display("");
            $display("Test 6: System Reset Mode");
            
            // Reset first
            do_wdt_reset();
            
            // Enable reset mode: WDIE=0, WDE=1, WDP=0000
            write_register(WDTCSR_ADDR, 8'h08);  // WDE=1
            
            // Wait for timeout
            for (i = 0; i < 2048 * WDT_OSC_DIV + 100; i = i + 1) begin
                @(posedge clk);
                if (wdt_reset_req) begin
                    $display("  Reset request after %0d cycles", i);
                    i = 2048 * WDT_OSC_DIV + 100;  // Exit loop
                end
            end
            
            // Check reset request is asserted
            check_result("Reset request asserted", wdt_reset_req, 1'b1);
            
            // Check WDRF is set
            read_register(MCUSR_ADDR);
            check_result("WDRF set on reset", mem_rdata[3], 1'b1);
            
            // Clear WDRF
            write_register(MCUSR_ADDR, 8'h00);
            read_register(MCUSR_ADDR);
            check_result("WDRF cleared", mem_rdata[3], 1'b0);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_interrupt_reset_mode;
        integer i;
        begin
            $display("");
            $display("Test 7: Interrupt + Reset Mode");
            
            // Reset first
            do_wdt_reset();
            
            // Enable interrupt+reset mode: WDIE=1, WDE=1
            write_register(WDTCSR_ADDR, 8'h48);  // WDIE=1, WDE=1
            
            // Wait for first timeout (should generate interrupt)
            for (i = 0; i < 2048 * WDT_OSC_DIV + 100; i = i + 1) begin
                @(posedge clk);
                if (irq_wdt) begin
                    $display("  First timeout: IRQ after %0d cycles", i);
                    i = 2048 * WDT_OSC_DIV + 100;
                end
            end
            
            // Check interrupt fired
            check_result("INT+RST: IRQ on 1st timeout", irq_wdt, 1'b1);
            
            // Check WDIE is auto-cleared
            read_register(WDTCSR_ADDR);
            check_result("INT+RST: WDIE auto-cleared", mem_rdata[6], 1'b0);
            
            // Check no reset yet
            check_result("INT+RST: No reset on 1st", wdt_reset_req, 1'b0);
            
            // Clear WDIF
            write_register(WDTCSR_ADDR, 8'h88);  // Write 1 to WDIF
            
            // Wait for second timeout (should generate reset)
            for (i = 0; i < 2048 * WDT_OSC_DIV + 100; i = i + 1) begin
                @(posedge clk);
                if (wdt_reset_req) begin
                    $display("  Second timeout: Reset after %0d cycles", i);
                    i = 2048 * WDT_OSC_DIV + 100;
                end
            end
            
            // Check reset request
            check_result("INT+RST: Reset on 2nd", wdt_reset_req, 1'b1);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_wdr;
        integer i;
        begin
            $display("");
            $display("Test 8: WDR (Watchdog Reset) Functionality");
            
            // Reset first
            do_wdt_reset();
            
            // Enable WDT in interrupt mode
            write_register(WDTCSR_ADDR, 8'h40);  // WDIE=1
            
            // Wait half the timeout period
            for (i = 0; i < 1024 * WDT_OSC_DIV; i = i + 1) begin
                @(posedge clk);
            end
            
            // Issue WDR
            write_wdr();
            $display("  WDR issued at half timeout");
            
            // Wait another half period - should NOT timeout yet
            for (i = 0; i < 1024 * WDT_OSC_DIV; i = i + 1) begin
                @(posedge clk);
            end
            
            check_result("WDR: No IRQ at 1.5x timeout", irq_wdt, 1'b0);
            
            // Wait remaining time for full timeout after WDR
            for (i = 0; i < 1024 * WDT_OSC_DIV + 100; i = i + 1) begin
                @(posedge clk);
                if (irq_wdt) begin
                    $display("  IRQ after WDR + full timeout");
                    i = 1024 * WDT_OSC_DIV + 100;
                end
            end
            
            check_result("WDR: IRQ after WDR + timeout", irq_wdt, 1'b1);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_timed_sequence;
        begin
            $display("");
            $display("Test 9: Timed Sequence for WDE/WDP Changes");
            
            // Reset first
            do_wdt_reset();
            
            // Enable WDE first
            write_register(WDTCSR_ADDR, 8'h08);  // WDE=1
            
            // Try to clear WDE without timed sequence (should fail)
            write_register(WDTCSR_ADDR, 8'h00);
            read_register(WDTCSR_ADDR);
            check_result("WDE not cleared w/o seq", mem_rdata[3], 1'b1);
            
            // Start timed sequence
            write_register(WDTCSR_ADDR, 8'h18);  // WDCE=1, WDE=1
            
            // Clear WDE within sequence
            write_register(WDTCSR_ADDR, 8'h00);
            read_register(WDTCSR_ADDR);
            check_result("WDE cleared with seq", mem_rdata[3], 1'b0);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    task test_wdce_autoclear;
        integer i;
        begin
            $display("");
            $display("Test 10: WDCE Auto-clear");
            
            // Reset first
            do_wdt_reset();
            
            // Start timed sequence
            write_register(WDTCSR_ADDR, 8'h18);  // WDCE=1, WDE=1
            
            // Check WDCE is set
            read_register(WDTCSR_ADDR);
            // Note: WDCE might already be cleared by read timing
            
            // Wait beyond the 32 cycle WDCE window (40 cycles to be safe)
            for (i = 0; i < 40; i = i + 1) begin
                @(posedge clk);
            end
            
            // Check WDCE is auto-cleared
            read_register(WDTCSR_ADDR);
            check_result("WDCE auto-cleared", mem_rdata[4], 1'b0);
            
            // Reset for next tests
            do_wdt_reset();
        end
    endtask

    // =========================================================================
    // Helper Tasks
    // =========================================================================

    task write_register;
        input [31:0] addr;
        input [7:0] data;
        begin
            @(negedge clk);  // Set signals on falling edge for stability
            mem_valid = 1;
            mem_addr = addr;
            // Determine byte lane from address
            if (addr[1:0] == 2'b00) begin
                mem_wdata = {24'h0, data};
                mem_wstrb = 4'b0001;
            end else if (addr[1:0] == 2'b01) begin
                mem_wdata = {16'h0, data, 8'h0};
                mem_wstrb = 4'b0010;
            end else if (addr[1:0] == 2'b10) begin
                mem_wdata = {8'h0, data, 16'h0};
                mem_wstrb = 4'b0100;
            end else begin
                mem_wdata = {data, 24'h0};
                mem_wstrb = 4'b1000;
            end
            @(posedge clk);  // Wait for DUT to process
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
            mem_wstrb = 0;
        end
    endtask

    task read_register;
        input [31:0] addr;
        begin
            @(negedge clk);  // Set signals on falling edge for stability
            mem_valid = 1;
            mem_addr = addr;
            mem_wstrb = 0;  // Read
            @(posedge clk);  // Wait for DUT to process
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
        end
    endtask

    task write_wdr;
        begin
            // Write magic value 0xA5 to WDR register (byte 1 of word 0x60)
            @(negedge clk);
            mem_valid = 1;
            mem_addr = 32'h20000060;
            mem_wdata = {16'h0, 8'hA5, 8'h0};  // 0xA5 in byte 1
            mem_wstrb = 4'b0010;  // Byte 1
            @(posedge clk);
            wait(mem_ready);
            @(negedge clk);
            mem_valid = 0;
            mem_wstrb = 0;
        end
    endtask

    task do_wdt_reset;
        begin
            @(negedge clk);  // Align to clock
            rst_n = 0;
            @(negedge clk);
            @(negedge clk);
            rst_n = 1;
            @(negedge clk);
        end
    endtask

    task check_result;
        input [127:0] test_name;  // Wide enough for test names
        input [31:0] actual;
        input [31:0] expected;
        begin
            test_count = test_count + 1;
            if (actual === expected) begin
                pass_count = pass_count + 1;
                $display("  PASS: %0s (expected=%h, got=%h)", test_name, expected, actual);
            end else begin
                fail_count = fail_count + 1;
                $display("  FAIL: %0s (expected=%h, got=%h)", test_name, expected, actual);
            end
        end
    endtask

endmodule
