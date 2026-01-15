`timescale 1ns / 1ps

// Watchdog Timer Integration Testbench
// Tests WDT integration with full SoC using firmware
//
// This testbench runs firmware that:
// 1. Configures the watchdog timer
// 2. Tests WDT interrupt mode
// 3. Tests WDR (watchdog reset) command
// 4. Tests WDT timeout and reset behavior
// 5. Signals test status via GPIO

module tb_soc_wdt;

    // Clock and reset
    reg clk;
    reg rst_n;

    // GPIO Port B
    reg [7:0] gpio_pin_in_b;
    wire [7:0] gpio_pin_out_b;
    wire [7:0] gpio_pin_dir_b;

    // GPIO Port D
    reg [7:0] gpio_pin_in_d;
    wire [7:0] gpio_pin_out_d;
    wire [7:0] gpio_pin_dir_d;

    // GPIO Port C
    reg [7:0] gpio_pin_in_c;
    wire [7:0] gpio_pin_out_c;
    wire [7:0] gpio_pin_dir_c;

    // Analog Comparator
    reg ain0;
    reg ain1;

    // Timer0
    reg t0_pin;
    wire oc0a;
    wire oc0b;

    // Watchdog Timer
    wire wdt_reset_req;
    
    // Timer2 signals
    wire oc2a;
    wire oc2b;
    
    // SPI signals (tie off unused)
    wire spi_sck;
    wire spi_mosi;
    wire spi_miso = spi_mosi;
    wire spi_miso_out;
    wire spi_miso_oe;
    wire spi_mosi_in = 1'b0;
    wire spi_ss_n = 1'b1;
    wire spi_sck_in = 1'b0;

    // Test tracking
    integer cycle_count;
    integer last_gpio_change;
    reg [7:0] prev_gpio_b;
    reg test_passed;
    reg test_failed;

    // Instantiate the SoC
    soc_top #(
    ) soc (
        .clk(clk),
        .rst_n(rst_n),
        .gpio_pin_in_b(gpio_pin_in_b),
        .gpio_pin_out_b(gpio_pin_out_b),
        .gpio_pin_dir_b(gpio_pin_dir_b),
        .gpio_pin_in_d(gpio_pin_in_d),
        .gpio_pin_out_d(gpio_pin_out_d),
        .gpio_pin_dir_d(gpio_pin_dir_d),
        .gpio_pin_in_c(gpio_pin_in_c),
        .gpio_pin_out_c(gpio_pin_out_c),
        .gpio_pin_dir_c(gpio_pin_dir_c),
        .ain0(ain0),
        .ain1(ain1),
        .t0_pin(t0_pin),
        .oc0a(oc0a),
        .oc0b(oc0b),
        .oc2a(oc2a),
        .oc2b(oc2b),
        .wdt_reset_req(wdt_reset_req),
        .spi_sck(spi_sck),
        .spi_mosi(spi_mosi),
        .spi_miso(spi_miso),
        .spi_miso_out(spi_miso_out),
        .spi_miso_oe(spi_miso_oe),
        .spi_mosi_in(spi_mosi_in),
        .spi_ss_n(spi_ss_n),
        .spi_sck_in(spi_sck_in)
    );

    // Clock generation (10ns period = 100MHz)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // VCD dump
    initial begin
        $dumpfile("tb_soc_wdt.vcd");
        $dumpvars(0, tb_soc_wdt);
    end

    // Cycle counter
    always @(posedge clk) begin
        if (!rst_n)
            cycle_count <= 0;
        else
            cycle_count <= cycle_count + 1;
    end

    // GPIO monitoring
    always @(posedge clk) begin
        if (gpio_pin_out_b !== prev_gpio_b) begin
            $display("[%0d] GPIO_B changed: 0x%02x -> 0x%02x", 
                     cycle_count, prev_gpio_b, gpio_pin_out_b);
            prev_gpio_b <= gpio_pin_out_b;
            last_gpio_change <= cycle_count;
        end
    end

    // WDT reset monitoring
    always @(posedge clk) begin
        if (wdt_reset_req) begin
            $display("[%0d] WDT Reset Request Detected!", cycle_count);
        end
    end

    // Test success/failure detection
    // Firmware signals:
    // - GPIO_B = 0xAA: Test passed
    // - GPIO_B = 0x55: Test failed
    // - GPIO_B bit patterns indicate test stages
    always @(posedge clk) begin
        if (gpio_pin_out_b == 8'hAA && !test_passed) begin
            test_passed <= 1'b1;
            $display("[%0d] *** TEST PASSED ***", cycle_count);
        end
        if (gpio_pin_out_b == 8'h55 && !test_failed) begin
            test_failed <= 1'b1;
            $display("[%0d] *** TEST FAILED ***", cycle_count);
        end
    end

    // Main test sequence
    initial begin
        // Initialize
        rst_n = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00;
        ain0 = 0;
        ain1 = 0;
        t0_pin = 0;
        prev_gpio_b = 8'h00;
        test_passed = 1'b0;
        test_failed = 1'b0;

        // Reset
        #100;
        rst_n = 1;

        $display("========================================");
        $display("Watchdog Timer Integration Test");
        $display("========================================");
        $display("Starting SoC with WDT test firmware...");

        // Run for a sufficient number of cycles
        // WDT timeout at shortest setting: 2048 * 128 = ~262K system cycles
        // Multiple tests need multiple timeouts, so run longer
        #20000000;

        // Check results
        $display("");
        $display("========================================");
        $display("Integration Test Summary");
        $display("========================================");
        $display("Total cycles: %0d", cycle_count);
        
        if (test_passed) begin
            $display("RESULT: PASSED");
        end else if (test_failed) begin
            $display("RESULT: FAILED");
        end else begin
            $display("RESULT: INCONCLUSIVE (no result signal)");
            $display("Last GPIO_B value: 0x%02x", gpio_pin_out_b);
        end

        $display("");
        $display("Note: Check GPIO_B patterns for test stage information");
        $display("  Bit 0: WDT register access OK");
        $display("  Bit 1: WDR command works");
        $display("  Bit 2: WDT interrupt fires");
        $display("  Bit 3: WDIF flag set correctly");
        $display("  0xAA = All tests passed");
        $display("  0x55 = Test failed");

        $finish;
    end

    // Timeout watchdog for simulation
    initial begin
        #50000000;  // 50ms simulation time max
        $display("");
        $display("SIMULATION TIMEOUT - Forcing end");
        $display("Final GPIO_B: 0x%02x", gpio_pin_out_b);
        $finish;
    end

endmodule
