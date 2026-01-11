// Testbench for Bus Decoder Module
// Tests address decoding and peripheral routing

`timescale 1ns / 1ps

`include "../rtl/bus/memory_map.vh"

module tb_bus_decoder;

    // CPU interface
    reg        cpu_mem_valid;
    reg [31:0] cpu_mem_addr;
    reg [31:0] cpu_mem_wdata;
    reg [3:0]  cpu_mem_wstrb;
    wire [31:0] cpu_mem_rdata;
    wire        cpu_mem_ready;
    
    // ROM interface
    wire        rom_ce;
    wire [15:0] rom_addr;
    reg [31:0]  rom_rdata;
    reg         rom_rdata_valid;
    
    // RAM interface
    wire        ram_ce;
    wire        ram_we;
    wire [11:0] ram_addr;
    reg [31:0]  ram_rdata;
    reg         ram_rdata_valid;
    
    // GPIO interface
    wire        gpio_mem_valid;
    wire [31:0] gpio_mem_addr;
    wire [31:0] gpio_mem_wdata;
    wire [3:0]  gpio_mem_wstrb;
    reg [31:0]  gpio_mem_rdata;
    reg         gpio_mem_ready;
    
    // Analog Comparator interface
    wire        ac_mem_valid;
    wire [31:0] ac_mem_addr;
    wire [31:0] ac_mem_wdata;
    wire [3:0]  ac_mem_wstrb;
    reg [31:0]  ac_mem_rdata;
    reg         ac_mem_ready;
    
    // Instantiate bus decoder
    bus_decoder dut (
        .cpu_mem_valid(cpu_mem_valid),
        .cpu_mem_addr(cpu_mem_addr),
        .cpu_mem_wdata(cpu_mem_wdata),
        .cpu_mem_wstrb(cpu_mem_wstrb),
        .cpu_mem_rdata(cpu_mem_rdata),
        .cpu_mem_ready(cpu_mem_ready),
        .rom_ce(rom_ce),
        .rom_addr(rom_addr),
        .rom_rdata(rom_rdata),
        .rom_rdata_valid(rom_rdata_valid),
        .ram_ce(ram_ce),
        .ram_we(ram_we),
        .ram_addr(ram_addr),
        .ram_rdata(ram_rdata),
        .ram_rdata_valid(ram_rdata_valid),
        .gpio_mem_valid(gpio_mem_valid),
        .gpio_mem_addr(gpio_mem_addr),
        .gpio_mem_wdata(gpio_mem_wdata),
        .gpio_mem_wstrb(gpio_mem_wstrb),
        .gpio_mem_rdata(gpio_mem_rdata),
        .gpio_mem_ready(gpio_mem_ready),
        .ac_mem_valid(ac_mem_valid),
        .ac_mem_addr(ac_mem_addr),
        .ac_mem_wdata(ac_mem_wdata),
        .ac_mem_wstrb(ac_mem_wstrb),
        .ac_mem_rdata(ac_mem_rdata),
        .ac_mem_ready(ac_mem_ready)
    );
    
    // Test sequence
    integer errors;
    
    initial begin
        errors = 0;
        
        // Initialize inputs
        cpu_mem_valid = 1'b0;
        cpu_mem_addr = 32'h00000000;
        cpu_mem_wdata = 32'h00000000;
        cpu_mem_wstrb = 4'b0000;
        rom_rdata = 32'h00000000;
        rom_rdata_valid = 1'b0;
        ram_rdata = 32'h00000000;
        ram_rdata_valid = 1'b0;
        gpio_mem_rdata = 32'h00000000;
        gpio_mem_ready = 1'b0;
        ac_mem_rdata = 32'h00000000;
        ac_mem_ready = 1'b0;
        
        #100;
        
        $display("========================================");
        $display("Bus Decoder Test Suite");
        $display("========================================");
        
        // Test 1: ROM address decoding
        $display("\nTest 1: ROM address decoding (0x00000000)");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h00000000;  // ROM base
        cpu_mem_wstrb = 4'b0000;  // Read
        rom_rdata = 32'h12345678;
        rom_rdata_valid = 1'b1;
        #10;
        
        if (rom_ce !== 1'b1) begin
            $display("ERROR: rom_ce = %b, expected 1", rom_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: rom_ce = 1");
        end
        if (ram_ce !== 1'b0) begin
            $display("ERROR: ram_ce = %b, expected 0", ram_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_ce = 0");
        end
        if (gpio_mem_valid !== 1'b0) begin
            $display("ERROR: gpio_mem_valid = %b, expected 0", gpio_mem_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_valid = 0");
        end
        if (cpu_mem_rdata !== 32'h12345678) begin
            $display("ERROR: cpu_mem_rdata = 0x%08x, expected 0x12345678", cpu_mem_rdata);
            errors = errors + 1;
        end else begin
            $display("PASS: cpu_mem_rdata = 0x%08x", cpu_mem_rdata);
        end
        
        cpu_mem_valid = 1'b0;
        rom_rdata_valid = 1'b0;
        #10;
        
        // Test 2: ROM word address calculation
        $display("\nTest 2: ROM word address calculation");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h00001000;  // Byte address 0x1000
        rom_rdata_valid = 1'b1;
        #10;
        
        // Word address should be 0x1000 >> 2 = 0x400
        if (rom_addr !== 16'h0400) begin
            $display("ERROR: rom_addr = 0x%04x, expected 0x0400", rom_addr);
            errors = errors + 1;
        end else begin
            $display("PASS: rom_addr = 0x%04x (word address)", rom_addr);
        end
        
        cpu_mem_valid = 1'b0;
        rom_rdata_valid = 1'b0;
        #10;
        
        // Test 3: RAM address decoding
        $display("\nTest 3: RAM address decoding (0x10000000)");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h10000000;  // RAM base
        cpu_mem_wstrb = 4'b0000;  // Read
        ram_rdata = 32'hABCDEF01;
        ram_rdata_valid = 1'b1;
        #10;
        
        if (ram_ce !== 1'b1) begin
            $display("ERROR: ram_ce = %b, expected 1", ram_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_ce = 1");
        end
        if (rom_ce !== 1'b0) begin
            $display("ERROR: rom_ce = %b, expected 0", rom_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: rom_ce = 0");
        end
        if (cpu_mem_rdata !== 32'hABCDEF01) begin
            $display("ERROR: cpu_mem_rdata = 0x%08x, expected 0xABCDEF01", cpu_mem_rdata);
            errors = errors + 1;
        end else begin
            $display("PASS: cpu_mem_rdata = 0x%08x", cpu_mem_rdata);
        end
        
        cpu_mem_valid = 1'b0;
        ram_rdata_valid = 1'b0;
        #10;
        
        // Test 4: RAM write enable
        $display("\nTest 4: RAM write enable");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h10000004;
        cpu_mem_wstrb = 4'b1111;  // Write all bytes
        ram_rdata_valid = 1'b1;
        #10;
        
        if (ram_we !== 1'b1) begin
            $display("ERROR: ram_we = %b, expected 1", ram_we);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_we = 1 when wstrb != 0");
        end
        
        cpu_mem_wstrb = 4'b0000;  // Read
        #10;
        if (ram_we !== 1'b0) begin
            $display("ERROR: ram_we = %b, expected 0", ram_we);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_we = 0 when wstrb == 0");
        end
        
        cpu_mem_valid = 1'b0;
        ram_rdata_valid = 1'b0;
        #10;
        
        // Test 5: GPIO address decoding
        $display("\nTest 5: GPIO address decoding (0x20000024 - DDRB)");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = `GPIOB_DDRB;
        cpu_mem_wstrb = 4'b0000;
        gpio_mem_rdata = 32'h12341234;
        gpio_mem_ready = 1'b1;
        #10;
        
        if (gpio_mem_valid !== 1'b1) begin
            $display("ERROR: gpio_mem_valid = %b, expected 1", gpio_mem_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_valid = 1");
        end
        if (rom_ce !== 1'b0) begin
            $display("ERROR: rom_ce = %b, expected 0", rom_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: rom_ce = 0");
        end
        if (ram_ce !== 1'b0) begin
            $display("ERROR: ram_ce = %b, expected 0", ram_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_ce = 0");
        end
        if (gpio_mem_addr !== `GPIOB_DDRB) begin
            $display("ERROR: gpio_mem_addr = 0x%08x, expected 0x%08x", gpio_mem_addr, `GPIOB_DDRB);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_addr = 0x%08x", gpio_mem_addr);
        end
        
        cpu_mem_valid = 1'b0;
        gpio_mem_ready = 1'b0;
        #10;
        
        // Test 6: Analog Comparator address decoding
        $display("\nTest 6: Analog Comparator address decoding (0x20000050 - ACSR)");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = `ACSR;
        cpu_mem_wstrb = 4'b0000;
        ac_mem_rdata = 32'hAC0DA7A1;
        ac_mem_ready = 1'b1;
        #10;
        
        if (ac_mem_valid !== 1'b1) begin
            $display("ERROR: ac_mem_valid = %b, expected 1", ac_mem_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: ac_mem_valid = 1");
        end
        if (gpio_mem_valid !== 1'b0) begin
            $display("ERROR: gpio_mem_valid = %b, expected 0 (AC should take precedence)", gpio_mem_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_valid = 0 (AC takes precedence in IO space)");
        end
        if (cpu_mem_ready !== 1'b1) begin
            $display("ERROR: cpu_mem_ready = %b, expected 1", cpu_mem_ready);
            errors = errors + 1;
        end else begin
            $display("PASS: cpu_mem_ready = 1");
        end
        
        cpu_mem_valid = 1'b0;
        ac_mem_ready = 1'b0;
        #10;
        
        // Test 7: Data passthrough to peripherals
        $display("\nTest 7: Data passthrough to GPIO");
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = `GPIOB_PORTB;
        cpu_mem_wdata = 32'h55AA55AA;
        cpu_mem_wstrb = 4'b0001;
        gpio_mem_ready = 1'b1;
        #10;
        
        if (gpio_mem_wdata !== 32'h55AA55AA) begin
            $display("ERROR: gpio_mem_wdata = 0x%08x, expected 0x55AA55AA", gpio_mem_wdata);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_wdata = 0x%08x", gpio_mem_wdata);
        end
        if (gpio_mem_wstrb !== 4'b0001) begin
            $display("ERROR: gpio_mem_wstrb = 0x%01x, expected 0x1", gpio_mem_wstrb);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_wstrb = 0x%01x", gpio_mem_wstrb);
        end
        
        cpu_mem_valid = 1'b0;
        gpio_mem_ready = 1'b0;
        #10;
        
        // Test 8: Ready signal routing
        $display("\nTest 8: Ready signal routing");
        
        // ROM ready
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h00000000;
        rom_rdata_valid = 1'b1;
        #10;
        if (cpu_mem_ready !== 1'b1) begin
            $display("ERROR: ROM ready not routed");
            errors = errors + 1;
        end else begin
            $display("PASS: ROM ready routed");
        end
        rom_rdata_valid = 1'b0;
        cpu_mem_valid = 1'b0;
        #10;
        
        // RAM ready
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = 32'h10000000;
        ram_rdata_valid = 1'b1;
        #10;
        if (cpu_mem_ready !== 1'b1) begin
            $display("ERROR: RAM ready not routed");
            errors = errors + 1;
        end else begin
            $display("PASS: RAM ready routed");
        end
        ram_rdata_valid = 1'b0;
        cpu_mem_valid = 1'b0;
        #10;
        
        // Test 9: No peripheral selected when not valid
        $display("\nTest 9: No selection when valid=0");
        cpu_mem_valid = 1'b0;
        cpu_mem_addr = 32'h00000000;  // Would select ROM
        #10;
        
        if (rom_ce !== 1'b0) begin
            $display("ERROR: rom_ce = %b when valid=0", rom_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: rom_ce = 0 when valid=0");
        end
        if (ram_ce !== 1'b0) begin
            $display("ERROR: ram_ce = %b when valid=0", ram_ce);
            errors = errors + 1;
        end else begin
            $display("PASS: ram_ce = 0 when valid=0");
        end
        if (gpio_mem_valid !== 1'b0) begin
            $display("ERROR: gpio_mem_valid = %b when valid=0", gpio_mem_valid);
            errors = errors + 1;
        end else begin
            $display("PASS: gpio_mem_valid = 0 when valid=0");
        end
        
        // Test 10: All GPIO port addresses
        $display("\nTest 10: All GPIO port address routing");
        gpio_mem_ready = 1'b1;
        
        // Port B
        cpu_mem_valid = 1'b1;
        cpu_mem_addr = `GPIOB_PINB;
        #10;
        if (gpio_mem_valid !== 1'b1) begin
            $display("ERROR: PINB not routed to GPIO");
            errors = errors + 1;
        end else begin
            $display("PASS: PINB (0x%08x) routed to GPIO", `GPIOB_PINB);
        end
        
        cpu_mem_addr = `GPIOC_PORTC;
        #10;
        if (gpio_mem_valid !== 1'b1) begin
            $display("ERROR: PORTC not routed to GPIO");
            errors = errors + 1;
        end else begin
            $display("PASS: PORTC (0x%08x) routed to GPIO", `GPIOC_PORTC);
        end
        
        cpu_mem_addr = `GPIOD_DDRD;
        #10;
        if (gpio_mem_valid !== 1'b1) begin
            $display("ERROR: DDRD not routed to GPIO");
            errors = errors + 1;
        end else begin
            $display("PASS: DDRD (0x%08x) routed to GPIO", `GPIOD_DDRD);
        end
        
        cpu_mem_valid = 1'b0;
        gpio_mem_ready = 1'b0;
        #10;
        
        // Summary
        $display("\n========================================");
        if (errors == 0) begin
            $display("ALL TESTS PASSED!");
        end else begin
            $display("TESTS FAILED: %0d error(s)", errors);
        end
        $display("========================================");
        
        #100;
        $finish;
    end
    
    // Waveform dump
    initial begin
        $dumpfile("tb_bus_decoder.vcd");
        $dumpvars(0, tb_bus_decoder);
    end

endmodule