// Timer0 Integration Testbench
// Tests Timer0 functionality within the full SoC
// Runs timer0_test.c firmware and verifies PORTB output indicates all tests passed

`timescale 1ns / 1ps

module tb_soc_timer0;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // GPIO Port B signals (used for test status output)
    reg [7:0] gpio_pin_in_b;
    wire [7:0] gpio_pin_out_b;
    wire [7:0] gpio_pin_dir_b;
    
    // GPIO Port D signals
    reg [7:0] gpio_pin_in_d;
    wire [7:0] gpio_pin_out_d;
    wire [7:0] gpio_pin_dir_d;

    // GPIO Port C signals
    reg [7:0] gpio_pin_in_c;
    wire [7:0] gpio_pin_out_c;
    wire [7:0] gpio_pin_dir_c;

    // Analog Comparator signals
    reg ain0;
    reg ain1;
    
    // Timer0 signals
    reg t0_pin;
    wire oc0a;
    wire oc0b;
    
    // Test tracking
    reg [7:0] prev_portb;
    reg [7:0] tests_detected;
    integer test_timeout;
    
    // Instantiate DUT
    soc_top dut (
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
        .oc0b(oc0b)
    );
    
    // Clock generation (100MHz = 10ns period for faster simulation)
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    // VCD dump
    initial begin
        $dumpfile("tb_soc_timer0.vcd");
        $dumpvars(0, tb_soc_timer0);
    end
    
    // Reset and initialization
    initial begin
        rst_n = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00;
        ain0 = 0;
        ain1 = 0;
        t0_pin = 0;
        prev_portb = 8'h00;
        tests_detected = 8'h00;
        
        #100;
        rst_n = 1;
        $display("===========================================");
        $display("Timer0 Integration Test Started");
        $display("===========================================");
        $display("Time %0t: Reset released", $time);
    end
    
    // Monitor PORTB for test results
    // Each bit indicates a test passed (see timer0_test.c)
    always @(posedge clk) begin
        if (gpio_pin_out_b != prev_portb && rst_n) begin
            prev_portb <= gpio_pin_out_b;
            
            // Detect which tests just passed
            if (gpio_pin_out_b[0] && !tests_detected[0]) begin
                $display("Time %0t: TEST 0 PASSED - Register access", $time);
                tests_detected[0] <= 1'b1;
            end
            if (gpio_pin_out_b[1] && !tests_detected[1]) begin
                $display("Time %0t: TEST 1 PASSED - Normal mode counting", $time);
                tests_detected[1] <= 1'b1;
            end
            if (gpio_pin_out_b[2] && !tests_detected[2]) begin
                $display("Time %0t: TEST 2 PASSED - Overflow detection", $time);
                tests_detected[2] <= 1'b1;
            end
            if (gpio_pin_out_b[3] && !tests_detected[3]) begin
                $display("Time %0t: TEST 3 PASSED - Compare match A", $time);
                tests_detected[3] <= 1'b1;
            end
            if (gpio_pin_out_b[4] && !tests_detected[4]) begin
                $display("Time %0t: TEST 4 PASSED - Compare match B", $time);
                tests_detected[4] <= 1'b1;
            end
            if (gpio_pin_out_b[5] && !tests_detected[5]) begin
                $display("Time %0t: TEST 5 PASSED - CTC mode", $time);
                tests_detected[5] <= 1'b1;
            end
            if (gpio_pin_out_b[6] && !tests_detected[6]) begin
                $display("Time %0t: TEST 6 PASSED - Prescaler /8", $time);
                tests_detected[6] <= 1'b1;
            end
            if (gpio_pin_out_b[7] && !tests_detected[7]) begin
                $display("Time %0t: TEST 7 PASSED - All tests complete!", $time);
                tests_detected[7] <= 1'b1;
            end
        end
    end
    
    // Monitor Timer0 output compare pins
    reg prev_oc0a, prev_oc0b;
    initial begin
        prev_oc0a = 0;
        prev_oc0b = 0;
    end
    
    always @(posedge clk) begin
        if (oc0a != prev_oc0a) begin
            $display("Time %0t: OC0A changed to %b", $time, oc0a);
            prev_oc0a <= oc0a;
        end
        if (oc0b != prev_oc0b) begin
            $display("Time %0t: OC0B changed to %b", $time, oc0b);
            prev_oc0b <= oc0b;
        end
    end
    
    // Monitor CPU trap
    always @(posedge clk) begin
        if (dut.cpu_trap) begin
            $display("ERROR: CPU TRAP detected at time %0t", $time);
            $display("Final PORTB = 0x%02x", gpio_pin_out_b);
            $display("Tests passed: %b", tests_detected);
            $finish;
        end
    end
    
    
    // Simulation timeout and result checking
    initial begin
        // Wait for reset to complete before starting polling
        #200;
        
        // Wait for tests to complete (with timeout)
        // At 100MHz, 10ms = 1,000,000 cycles
        test_timeout = 0;
        
        // Poll for completion
        while (gpio_pin_out_b !== 8'hFF && test_timeout < 2000000) begin
            @(posedge clk);
            test_timeout = test_timeout + 1;
        end
        
        // Give a little extra time after all tests detected
        repeat(1000) @(posedge clk);
        
        // Report results
        $display("");
        $display("===========================================");
        $display("Timer0 Integration Test Results");
        $display("===========================================");
        $display("Final PORTB = 0x%02x", gpio_pin_out_b);
        $display("Tests passed bitmap: %b", tests_detected);
        $display("");
        
        if (gpio_pin_out_b == 8'hFF) begin
            $display("*** ALL TESTS PASSED! ***");
        end else begin
            $display("*** SOME TESTS FAILED ***");
            if (!tests_detected[0]) $display("  - Test 0 FAILED: Register access");
            if (!tests_detected[1]) $display("  - Test 1 FAILED: Normal mode counting");
            if (!tests_detected[2]) $display("  - Test 2 FAILED: Overflow detection");
            if (!tests_detected[3]) $display("  - Test 3 FAILED: Compare match A");
            if (!tests_detected[4]) $display("  - Test 4 FAILED: Compare match B");
            if (!tests_detected[5]) $display("  - Test 5 FAILED: CTC mode");
            if (!tests_detected[6]) $display("  - Test 6 FAILED: Prescaler /8");
            if (!tests_detected[7]) $display("  - Test 7 FAILED: Final check");
        end
        
        if (test_timeout >= 2000000) begin
            $display("WARNING: Simulation timed out!");
        end
        
        $display("===========================================");
        $display("Simulation completed at time %0t", $time);
        $display("===========================================");
        $finish;
    end

endmodule
