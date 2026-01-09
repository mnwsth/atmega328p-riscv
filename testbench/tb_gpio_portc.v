// Testbench for GPIO Port C
// Tests Port C register functionality independently

`timescale 1ns / 1ps

`include "../rtl/bus/memory_map.vh"

module tb_gpio_portc;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // Memory bus interface (simulated CPU)
    reg        mem_valid;
    reg [31:0] mem_addr;
    reg [31:0] mem_wdata;
    reg [3:0]  mem_wstrb;
    wire [31:0] mem_rdata;
    wire        mem_ready;
    
    // GPIO Port B signals (not tested)
    reg [7:0]  gpio_pin_in_b;
    wire [7:0] gpio_pin_out_b;
    wire [7:0] gpio_pin_dir_b;
    
    // GPIO Port D signals (not tested)
    reg [7:0]  gpio_pin_in_d;
    wire [7:0] gpio_pin_out_d;
    wire [7:0] gpio_pin_dir_d;

    // GPIO Port C signals (under test)
    reg [7:0]  gpio_pin_in_c;
    wire [7:0] gpio_pin_out_c;
    wire [7:0] gpio_pin_dir_c;
    
    // Instantiate GPIO module
    gpio dut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
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
    
    // Clock generation (10MHz = 100ns period)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;
    end
    
    // Test task: Write to register (using word-aligned bus protocol)
    // The GPIO uses word-aligned addresses, so we need to:
    // 1. Convert byte address to word address
    // 2. Set the correct byte strobe
    // 3. Put the data in the correct byte lane
    task write_reg(input [31:0] byte_addr, input [7:0] data);
        reg [31:0] word_addr;
        reg [1:0] byte_offset;
        begin
            word_addr = {byte_addr[31:2], 2'b00};
            byte_offset = byte_addr[1:0];
            
            // Set up address and data BEFORE asserting valid
            mem_addr = word_addr;
            case (byte_offset)
                2'b00: begin mem_wdata = {24'h000000, data}; mem_wstrb = 4'b0001; end
                2'b01: begin mem_wdata = {16'h0000, data, 8'h00}; mem_wstrb = 4'b0010; end
                2'b10: begin mem_wdata = {8'h00, data, 16'h0000}; mem_wstrb = 4'b0100; end
                2'b11: begin mem_wdata = {data, 24'h000000}; mem_wstrb = 4'b1000; end
            endcase
            
            // Assert valid on clock edge
            @(posedge clk);
            mem_valid = 1'b1;
            
            @(posedge clk);
            wait(mem_ready);
            @(posedge clk);
            mem_valid = 1'b0;
            mem_wstrb = 4'b0000;
            #100;
        end
    endtask
    
    // Test task: Read from register (using word-aligned bus protocol)
    // The GPIO returns the full word, we need to extract the correct byte
    task read_reg(input [31:0] byte_addr, output [7:0] data);
        reg [31:0] word_addr;
        reg [1:0] byte_offset;
        reg [31:0] word_data;
        begin
            word_addr = {byte_addr[31:2], 2'b00};
            byte_offset = byte_addr[1:0];
            
            // Set up address BEFORE asserting valid
            mem_addr = word_addr;
            mem_wstrb = 4'b0000;
            
            @(posedge clk);
            mem_valid = 1'b1;
            
            @(posedge clk);
            wait(mem_ready);
            word_data = mem_rdata;
            
            // Extract correct byte from word
            case (byte_offset)
                2'b00: data = word_data[7:0];
                2'b01: data = word_data[15:8];
                2'b10: data = word_data[23:16];
                2'b11: data = word_data[31:24];
            endcase
            
            @(posedge clk);
            mem_valid = 1'b0;
            #100;
        end
    endtask
    
    // Test sequence
    integer errors;
    reg [7:0] read_data;
    
    initial begin
        errors = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00;
        mem_valid = 1'b0;
        mem_addr = 32'h00000000;
        mem_wdata = 32'h00000000;
        mem_wstrb = 4'h0;
        
        // Reset
        rst_n = 0;
        #1000;
        rst_n = 1;
        #100;
        
        $display("========================================");
        $display("GPIO Port C Test Suite");
        $display("========================================");
        
        // Test 1: Read DDRC (should be 0x00 after reset)
        $display("\nTest 1: Read DDRC after reset");
        read_reg(`GPIOC_DDRC, read_data);
        if (read_data !== 8'h00) begin
            $display("ERROR: DDRC = 0x%02x, expected 0x00", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: DDRC = 0x%02x", read_data);
        end
        
        // Test 2: Read PORTC (should be 0x00 after reset)
        $display("\nTest 2: Read PORTC after reset");
        read_reg(`GPIOC_PORTC, read_data);
        if (read_data !== 8'h00) begin
            $display("ERROR: PORTC = 0x%02x, expected 0x00", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PORTC = 0x%02x", read_data);
        end
        
        // Test 3: Read PINC (should reflect pin input)
        $display("\nTest 3: Read PINC (input register)");
        gpio_pin_in_c = 8'hAA;
        #100;
        read_reg(`GPIOC_PINC, read_data);
        if (read_data !== 8'hAA) begin
            $display("ERROR: PINC = 0x%02x, expected 0xAA", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PINC = 0x%02x", read_data);
        end
        
        // Test 4: Write DDRC (configure pins as outputs)
        $display("\nTest 4: Write DDRC = 0xFF (all outputs)");
        write_reg(`GPIOC_DDRC, 8'hFF);
        read_reg(`GPIOC_DDRC, read_data);
        if (read_data !== 8'hFF) begin
            $display("ERROR: DDRC = 0x%02x, expected 0xFF", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: DDRC = 0x%02x", read_data);
        end
        if (gpio_pin_dir_c !== 8'hFF) begin
            $display("ERROR: gpio_pin_dir_c = 0x%02x, expected 0xFF", gpio_pin_dir_c);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_pin_dir_c = 0x%02x", gpio_pin_dir_c);
        end
        
        // Test 5: Write PORTC (drive output pins)
        $display("\nTest 5: Write PORTC = 0x55");
        write_reg(`GPIOC_PORTC, 8'h55);
        read_reg(`GPIOC_PORTC, read_data);
        if (read_data !== 8'h55) begin
            $display("ERROR: PORTC = 0x%02x, expected 0x55", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PORTC = 0x%02x", read_data);
        end
        if (gpio_pin_out_c !== 8'h55) begin
            $display("ERROR: gpio_pin_out_c = 0x%02x, expected 0x55", gpio_pin_out_c);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_pin_out_c = 0x%02x", gpio_pin_out_c);
        end
        
        // Test 6: Change PORTC value
        $display("\nTest 6: Write PORTC = 0xAA");
        write_reg(`GPIOC_PORTC, 8'hAA);
        read_reg(`GPIOC_PORTC, read_data);
        if (read_data !== 8'hAA) begin
            $display("ERROR: PORTC = 0x%02x, expected 0xAA", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PORTC = 0x%02x", read_data);
        end
        if (gpio_pin_out_c !== 8'hAA) begin
            $display("ERROR: gpio_pin_out_c = 0x%02x, expected 0xAA", gpio_pin_out_c);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_pin_out_c = 0x%02x", gpio_pin_out_c);
        end
        
        // Test 7: Configure mixed input/output (DDRC = 0x0F)
        $display("\nTest 7: Configure mixed I/O (DDRC = 0x0F)");
        write_reg(`GPIOC_DDRC, 8'h0F);
        read_reg(`GPIOC_DDRC, read_data);
        if (read_data !== 8'h0F) begin
            $display("ERROR: DDRC = 0x%02x, expected 0x0F", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: DDRC = 0x%02x", read_data);
        end
        if (gpio_pin_dir_c !== 8'h0F) begin
            $display("ERROR: gpio_pin_dir_c = 0x%02x, expected 0x0F", gpio_pin_dir_c);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_pin_dir_c = 0x%02x", gpio_pin_dir_c);
        end
        
        // Test 8: Verify PINC still reads input when pins are outputs
        $display("\nTest 8: Verify PINC reads input (pins 7:4 are inputs)");
        gpio_pin_in_c = 8'hF0;
        #100;
        read_reg(`GPIOC_PINC, read_data);
        if (read_data !== 8'hF0) begin
            $display("ERROR: PINC = 0x%02x, expected 0xF0", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PINC = 0x%02x", read_data);
        end
        
        // Test 9: Verify Port B and D are unaffected
        $display("\nTest 9: Verify Port B and D are unaffected");
        read_reg(`GPIOB_DDRB, read_data);
        if (read_data !== 8'h00) begin
            $display("ERROR: DDRB = 0x%02x, expected 0x00", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: DDRB = 0x%02x (Port B unaffected)", read_data);
        end
        read_reg(`GPIOD_DDRD, read_data);
        if (read_data !== 8'h00) begin
            $display("ERROR: DDRD = 0x%02x, expected 0x00", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: DDRD = 0x%02x (Port D unaffected)", read_data);
        end
        
        // Test 10: Verify PINC is read-only
        $display("\nTest 10: Verify PINC is read-only");
        write_reg(`GPIOC_PINC, 8'hFF);  // Attempt to write
        gpio_pin_in_c = 8'h33;
        #100;
        read_reg(`GPIOC_PINC, read_data);
        if (read_data !== 8'h33) begin
            $display("ERROR: PINC = 0x%02x, expected 0x33 (write should be ignored)", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: PINC = 0x%02x (write ignored, reads pin state)", read_data);
        end
        
        // Summary
        $display("\n========================================");
        if (errors == 0) begin
            $display("ALL TESTS PASSED!");
        end else begin
            $display("TESTS FAILED: %0d error(s)", errors);
        end
        $display("========================================");
        
        #1000;
        $finish;
    end
    
    // Waveform dump
    initial begin
        $dumpfile("tb_gpio_portc.vcd");
        $dumpvars(0, tb_gpio_portc);
    end

endmodule
