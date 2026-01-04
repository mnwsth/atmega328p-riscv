// Testbench for SoC
// Simulates the SoC running the blinky program

`timescale 1ns / 1ps

module tb_soc;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // GPIO signals
    reg [7:0] gpio_pin_in;
    wire [7:0] gpio_pin_out;
    wire [7:0] gpio_pin_dir;
    
    // Instantiate DUT
    soc_top dut (
        .clk(clk),
        .rst_n(rst_n),
        .gpio_pin_in(gpio_pin_in),
        .gpio_pin_out(gpio_pin_out),
        .gpio_pin_dir(gpio_pin_dir)
    );
    
    // Clock generation (10MHz = 100ns period)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;  // 50ns half period = 100ns period
    end
    
    // Reset generation
    initial begin
        rst_n = 0;
        gpio_pin_in = 8'h00;
        #1000;  // Hold reset for 1us
        rst_n = 1;
        $display("Reset released at time %0t", $time);
    end
    
    // Monitor GPIO output
    reg [7:0] prev_gpio_out;
    initial begin
        prev_gpio_out = 8'h00;
        forever begin
            @(posedge clk);
            if (gpio_pin_out != prev_gpio_out) begin
                $display("Time %0t: GPIO_PORTB = 0x%02x, GPIO_DIR = 0x%02x", 
                         $time, gpio_pin_out, gpio_pin_dir);
                prev_gpio_out = gpio_pin_out;
            end
        end
    end
    
    // Simulation control
    initial begin
        $dumpfile("tb_soc.vcd");
        $dumpvars(0, tb_soc);
        
        // Run for a while to see LED toggle
        #100000000;  // 100ms at 10MHz
        
        $display("Simulation complete");
        $finish;
    end

endmodule

