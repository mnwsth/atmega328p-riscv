// Testbench for RAM Module
// Tests RAM read/write functionality with byte strobes

`timescale 1ns / 1ps

module tb_ram;

    // Parameters
    parameter ADDR_WIDTH = 12;
    parameter DATA_WIDTH = 32;
    
    // Clock and reset
    reg clk;
    reg rst_n;
    
    // RAM interface
    reg                  ce;
    reg                  we;
    reg [3:0]            wstrb;
    reg [ADDR_WIDTH-1:0] addr;
    reg [DATA_WIDTH-1:0] wdata;
    wire [DATA_WIDTH-1:0] rdata;
    wire                  rdata_valid;
    
    // Instantiate RAM module
    ram #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) dut (
        .clk(clk),
        .rst_n(rst_n),
        .ce(ce),
        .we(we),
        .wstrb(wstrb),
        .addr(addr),
        .wdata(wdata),
        .rdata(rdata),
        .rdata_valid(rdata_valid)
    );
    
    // Clock generation (10MHz)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;
    end
    
    // Task: Write word to RAM
    task write_word(input [ADDR_WIDTH-1:0] address, input [DATA_WIDTH-1:0] data);
        begin
            @(posedge clk);
            ce = 1'b1;
            we = 1'b1;
            wstrb = 4'b1111;
            addr = address;
            wdata = data;
            @(posedge clk);
            ce = 1'b0;
            we = 1'b0;
            wstrb = 4'b0000;
            #100;
        end
    endtask
    
    // Task: Write byte to RAM
    task write_byte(input [ADDR_WIDTH-1:0] address, input [1:0] byte_lane, input [7:0] data);
        begin
            @(posedge clk);
            ce = 1'b1;
            we = 1'b1;
            addr = address;
            case (byte_lane)
                2'b00: begin wstrb = 4'b0001; wdata = {24'h000000, data}; end
                2'b01: begin wstrb = 4'b0010; wdata = {16'h0000, data, 8'h00}; end
                2'b10: begin wstrb = 4'b0100; wdata = {8'h00, data, 16'h0000}; end
                2'b11: begin wstrb = 4'b1000; wdata = {data, 24'h000000}; end
            endcase
            @(posedge clk);
            ce = 1'b0;
            we = 1'b0;
            wstrb = 4'b0000;
            #100;
        end
    endtask
    
    // Task: Read word from RAM
    task read_word(input [ADDR_WIDTH-1:0] address, output [DATA_WIDTH-1:0] data);
        begin
            @(posedge clk);
            ce = 1'b1;
            we = 1'b0;
            wstrb = 4'b0000;
            addr = address;
            @(posedge clk);
            wait(rdata_valid);
            data = rdata;
            ce = 1'b0;
            #100;
        end
    endtask
    
    // Test sequence
    integer errors;
    reg [DATA_WIDTH-1:0] read_data;
    
    initial begin
        errors = 0;
        ce = 1'b0;
        we = 1'b0;
        wstrb = 4'b0000;
        addr = 0;
        wdata = 0;
        
        // Reset
        rst_n = 0;
        #1000;
        rst_n = 1;
        #100;
        
        $display("========================================");
        $display("RAM Module Test Suite");
        $display("========================================");
        
        // Test 1: Read uninitialized memory (should be 0)
        $display("\nTest 1: Read uninitialized memory");
        read_word(12'h000, read_data);
        if (read_data !== 32'h00000000) begin
            $display("ERROR: Address 0x000 = 0x%08x, expected 0x00000000", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x000 = 0x%08x", read_data);
        end
        
        // Test 2: Write and read full word
        $display("\nTest 2: Write and read full word");
        write_word(12'h000, 32'hDEADBEEF);
        read_word(12'h000, read_data);
        if (read_data !== 32'hDEADBEEF) begin
            $display("ERROR: Address 0x000 = 0x%08x, expected 0xDEADBEEF", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x000 = 0x%08x", read_data);
        end
        
        // Test 3: Write individual bytes
        $display("\nTest 3: Write individual bytes");
        write_word(12'h004, 32'h00000000);  // Clear first
        write_byte(12'h004, 2'b00, 8'h11);  // Byte 0
        write_byte(12'h004, 2'b01, 8'h22);  // Byte 1
        write_byte(12'h004, 2'b10, 8'h33);  // Byte 2
        write_byte(12'h004, 2'b11, 8'h44);  // Byte 3
        read_word(12'h004, read_data);
        if (read_data !== 32'h44332211) begin
            $display("ERROR: Address 0x004 = 0x%08x, expected 0x44332211", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x004 = 0x%08x", read_data);
        end
        
        // Test 4: Verify byte write only affects target byte
        $display("\nTest 4: Byte write isolation");
        write_word(12'h008, 32'hFFFFFFFF);  // Fill with FF
        write_byte(12'h008, 2'b01, 8'h00);  // Clear only byte 1
        read_word(12'h008, read_data);
        if (read_data !== 32'hFFFF00FF) begin
            $display("ERROR: Address 0x008 = 0x%08x, expected 0xFFFF00FF", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x008 = 0x%08x", read_data);
        end
        
        // Test 5: Write to multiple addresses
        $display("\nTest 5: Multiple address writes");
        write_word(12'h010, 32'h11111111);
        write_word(12'h014, 32'h22222222);
        write_word(12'h018, 32'h33333333);
        
        read_word(12'h010, read_data);
        if (read_data !== 32'h11111111) begin
            $display("ERROR: Address 0x010 = 0x%08x, expected 0x11111111", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x010 = 0x%08x", read_data);
        end
        
        read_word(12'h014, read_data);
        if (read_data !== 32'h22222222) begin
            $display("ERROR: Address 0x014 = 0x%08x, expected 0x22222222", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x014 = 0x%08x", read_data);
        end
        
        read_word(12'h018, read_data);
        if (read_data !== 32'h33333333) begin
            $display("ERROR: Address 0x018 = 0x%08x, expected 0x33333333", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Address 0x018 = 0x%08x", read_data);
        end
        
        // Test 6: Test first and last addresses
        $display("\nTest 6: Edge addresses (first and last)");
        write_word(12'h000, 32'hAAAAAAAA);  // First address
        write_word(12'hFFF, 32'h55555555);  // Last address (4095)
        
        read_word(12'h000, read_data);
        if (read_data !== 32'hAAAAAAAA) begin
            $display("ERROR: First address = 0x%08x, expected 0xAAAAAAAA", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: First address (0x000) = 0x%08x", read_data);
        end
        
        read_word(12'hFFF, read_data);
        if (read_data !== 32'h55555555) begin
            $display("ERROR: Last address = 0x%08x, expected 0x55555555", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Last address (0xFFF) = 0x%08x", read_data);
        end
        
        // Test 7: rdata_valid signal
        $display("\nTest 7: rdata_valid signal");
        @(posedge clk);
        ce = 1'b0;
        @(posedge clk);  // Wait for synchronous update
        @(posedge clk);  // Check after update
        if (rdata_valid !== 1'b0) begin
            $display("ERROR: rdata_valid = %b when ce=0, expected 0", rdata_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: rdata_valid = 0 when ce=0");
        end
        
        ce = 1'b1;
        we = 1'b0;
        addr = 12'h000;
        @(posedge clk);  // Apply ce=1
        @(posedge clk);  // Wait for synchronous output
        if (rdata_valid !== 1'b1) begin
            $display("ERROR: rdata_valid = %b when ce=1, expected 1", rdata_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: rdata_valid = 1 when ce=1");
        end
        ce = 1'b0;
        #100;
        
        // Test 8: Back-to-back operations
        $display("\nTest 8: Back-to-back operations");
        write_word(12'h020, 32'h12345678);
        write_word(12'h024, 32'h9ABCDEF0);
        read_word(12'h020, read_data);
        if (read_data !== 32'h12345678) begin
            $display("ERROR: Back-to-back read = 0x%08x, expected 0x12345678", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Back-to-back read = 0x%08x", read_data);
        end
        read_word(12'h024, read_data);
        if (read_data !== 32'h9ABCDEF0) begin
            $display("ERROR: Back-to-back read = 0x%08x, expected 0x9ABCDEF0", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Back-to-back read = 0x%08x", read_data);
        end
        
        // Test 9: Half-word write (bytes 0-1)
        $display("\nTest 9: Half-word write (bytes 0-1)");
        write_word(12'h030, 32'hFFFFFFFF);  // Fill with FF
        @(posedge clk);
        ce = 1'b1;
        we = 1'b1;
        wstrb = 4'b0011;  // Write bytes 0 and 1
        addr = 12'h030;
        wdata = 32'h0000ABCD;
        @(posedge clk);
        ce = 1'b0;
        we = 1'b0;
        wstrb = 4'b0000;
        #100;
        
        read_word(12'h030, read_data);
        if (read_data !== 32'hFFFFABCD) begin
            $display("ERROR: Half-word write = 0x%08x, expected 0xFFFFABCD", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Half-word write = 0x%08x", read_data);
        end
        
        // Test 10: Half-word write (bytes 2-3)
        $display("\nTest 10: Half-word write (bytes 2-3)");
        write_word(12'h034, 32'hFFFFFFFF);  // Fill with FF
        @(posedge clk);
        ce = 1'b1;
        we = 1'b1;
        wstrb = 4'b1100;  // Write bytes 2 and 3
        addr = 12'h034;
        wdata = 32'h12340000;
        @(posedge clk);
        ce = 1'b0;
        we = 1'b0;
        wstrb = 4'b0000;
        #100;
        
        read_word(12'h034, read_data);
        if (read_data !== 32'h1234FFFF) begin
            $display("ERROR: Half-word write = 0x%08x, expected 0x1234FFFF", read_data);
            errors = errors + 1;
        end else begin
            $display("PASS: Half-word write = 0x%08x", read_data);
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
        $dumpfile("tb_ram.vcd");
        $dumpvars(0, tb_ram);
    end

endmodule