// Testbench for SoC
// Simulates the SoC running the blinky program

`timescale 1ns / 1ps

module tb_soc;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // GPIO Port B signals
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
    
    // Timer2 signals
    wire oc2a;
    wire oc2b;
    
    // Watchdog Timer signals
    wire wdt_reset_req;
    
    // SPI signals (directly tie off unused ports)
    wire spi_sck;
    wire spi_mosi;
    wire spi_miso = spi_mosi;  // Loopback
    wire spi_miso_out;
    wire spi_miso_oe;
    wire spi_mosi_in = 1'b0;
    wire spi_ss_n = 1'b1;
    wire spi_sck_in = 1'b0;
    
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
    
    // Clock generation (10MHz = 100ns period)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;  // 50ns half period = 100ns period
    end
    
    // Reset generation
    initial begin
        rst_n = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00;
        ain0 = 0;
        ain1 = 0;
        t0_pin = 0;
        #1000;  // Hold reset for 1us
        rst_n = 1;
        $display("Reset released at time %0t", $time);
    end
    
    // Monitor GPIO Port B output
    reg [7:0] prev_gpio_out_b;
    initial begin
        prev_gpio_out_b = 8'h00;
        forever begin
            @(posedge clk);
            if (gpio_pin_out_b != prev_gpio_out_b) begin
                $display("Time %0t: GPIO_PORTB = 0x%02x, GPIO_DIR_B = 0x%02x", 
                         $time, gpio_pin_out_b, gpio_pin_dir_b);
                prev_gpio_out_b = gpio_pin_out_b;
            end
        end
    end
    
    // Monitor GPIO Port D output
    reg [7:0] prev_gpio_out_d;
    initial begin
        prev_gpio_out_d = 8'h00;
        forever begin
            @(posedge clk);
            if (gpio_pin_out_d != prev_gpio_out_d) begin
                $display("Time %0t: GPIO_PORTD = 0x%02x, GPIO_DIR_D = 0x%02x", 
                         $time, gpio_pin_out_d, gpio_pin_dir_d);
                prev_gpio_out_d = gpio_pin_out_d;
            end
        end
    end
    
    // Simulation control
    initial begin
        $dumpfile("tb_soc.vcd");
        $dumpvars(0, tb_soc);
        
        // Run for a short time first to debug
        #10000000;  // 10ms at 10MHz = 100,000 cycles
        
        $display("Simulation complete");
        $display("Final GPIO_PORTB = 0x%02x, GPIO_DIR_B = 0x%02x", gpio_pin_out_b, gpio_pin_dir_b);
        $finish;
    end
    
    // Monitor CPU trap
    always @(posedge clk) begin
        if (dut.cpu_trap) begin
            $display("ERROR: CPU TRAP detected at time %0t", $time);
        end
    end
    
    // One-time status at startup
    initial begin
        #10000; // After 10us
        $display("Status at 10us: GPIO_PORTB = 0x%02x, GPIO_DIR_B = 0x%02x", gpio_pin_out_b, gpio_pin_dir_b);
        #90000; // After 100us total
        $display("Status at 100us: GPIO_PORTB = 0x%02x, GPIO_DIR_B = 0x%02x", gpio_pin_out_b, gpio_pin_dir_b);
    end

    // Monitor GPIO Port C output
    reg [7:0] prev_gpio_out_c;
    initial begin
        prev_gpio_out_c = 8'h00;
        forever begin
            @(posedge clk);
            if (gpio_pin_out_c != prev_gpio_out_c) begin
                $display("Time %0t: GPIO_PORTC = 0x%02x, GPIO_DIR_C = 0x%02x", 
                         $time, gpio_pin_out_c, gpio_pin_dir_c);
                prev_gpio_out_c = gpio_pin_out_c;
            end
        end
    end

endmodule
