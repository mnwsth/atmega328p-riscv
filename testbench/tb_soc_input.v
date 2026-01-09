// Testbench for GPIO Input Verification
// Drives Port C (Input) and monitors Port B (Output)
// Verifies that firmware correctly reads PINC and writes PORTB

`timescale 1ns / 1ps

module tb_soc_input;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // GPIO Signals
    reg [7:0] gpio_pin_in_b;
    wire [7:0] gpio_pin_out_b;
    wire [7:0] gpio_pin_dir_b;
    
    reg [7:0] gpio_pin_in_d;
    wire [7:0] gpio_pin_out_d;
    wire [7:0] gpio_pin_dir_d;

    reg [7:0] gpio_pin_in_c;
    wire [7:0] gpio_pin_out_c;
    wire [7:0] gpio_pin_dir_c;
    
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
        .gpio_pin_dir_c(gpio_pin_dir_c)
    );
    
    // Clock generation (10MHz)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;
    end
    
    // Monitor CPU signals
    initial begin
        forever begin
            @(posedge clk);
            if (dut.cpu_mem_valid)
                $display("CPU: Valid access to %x (Instr: %b)", dut.cpu_mem_addr, dut.cpu_mem_instr);
            if (dut.rom_ce)
                $display("Decoder: ROM CE asserted for %x", dut.rom_addr);
        end
    end
    
    // Test Sequence
    initial begin
        $dumpfile("tb_soc_input.vcd");
        $dumpvars(0, tb_soc_input);
        
        // Initialize
        rst_n = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00; // Start with Input Low
        
        // Reset
        #1000;
        rst_n = 1;
        $display("Reset released");
        
        // Wait for firmware to initialize (set DDRB)
        // This might take a few thousand cycles
        #10000000; 
        
        // Check Heartbeat (PB7 should be toggling)
        if (gpio_pin_out_b & 8'h80) 
            $display("INFO: Heartbeat detected (PB7 High)");
        else
            $display("INFO: Heartbeat Low (might toggle later)");
            
        // Test 1: Input Low -> Expect Output Low
        $display("Test 1: Driving PC0 Low");
        gpio_pin_in_c = 8'h00;
        #50000; // Wait for loop to process
        
        if ((gpio_pin_out_b & 8'h01) == 8'h00) 
            $display("PASS: PB0 is Low");
        else 
            $display("FAIL: PB0 is High (Expected Low)");
            
        // Test 2: Input High -> Expect Output High
        $display("Test 2: Driving PC0 High");
        gpio_pin_in_c = 8'h01;
        #50000;
        
        if ((gpio_pin_out_b & 8'h01) == 8'h01) 
            $display("PASS: PB0 is High");
        else 
            $display("FAIL: PB0 is Low (Expected High)");
            
        // Test 3: Input Low again
        $display("Test 3: Driving PC0 Low");
        gpio_pin_in_c = 8'h00;
        #50000;
        
        if ((gpio_pin_out_b & 8'h01) == 8'h00) 
            $display("PASS: PB0 is Low");
        else 
            $display("FAIL: PB0 is High (Expected Low)");
            
        $finish;
    end

endmodule
