// Bus Decoder - Routes memory accesses to appropriate peripherals
// Simple address-based decoding

`include "bus/memory_map.vh"

module bus_decoder (
    // CPU interface
    input  wire        cpu_mem_valid,
    input  wire [31:0] cpu_mem_addr,
    input  wire [31:0] cpu_mem_wdata,
    input  wire [3:0]  cpu_mem_wstrb,
    output reg  [31:0] cpu_mem_rdata,
    output reg         cpu_mem_ready,
    
    // ROM interface
    output reg         rom_ce,
    output reg  [15:0] rom_addr,
    input  wire [31:0] rom_rdata,
    input  wire        rom_rdata_valid,
    
    // RAM interface
    output reg         ram_ce,
    output reg         ram_we,
    output reg  [11:0] ram_addr,
    input  wire [31:0] ram_rdata,
    input  wire        ram_rdata_valid,
    
    // GPIO interface
    output reg         gpio_mem_valid,
    output reg  [31:0] gpio_mem_addr,
    output reg  [31:0] gpio_mem_wdata,
    output reg  [3:0]  gpio_mem_wstrb,
    input  wire [31:0] gpio_mem_rdata,
    input  wire        gpio_mem_ready
);

    // Address decoding
    wire rom_sel = (cpu_mem_addr & `MEM_ROM_MASK) == `MEM_ROM_BASE;
    wire ram_sel = (cpu_mem_addr & `MEM_RAM_MASK) == `MEM_RAM_BASE;
    wire io_sel  = (cpu_mem_addr & `MEM_IO_MASK)  == `MEM_IO_BASE;
    
    // Address calculation (convert byte address to word address)
    wire [15:0] rom_word_addr = cpu_mem_addr[15:2];  // 64KB = 16K words (bits 15:2)
    wire [11:0] ram_word_addr = cpu_mem_addr[13:2];  // 4KB = 1K words (bits 13:2)
    
    // ROM interface
    always @(*) begin
        rom_ce = 1'b0;
        rom_addr = 16'h0000;
        
        if (cpu_mem_valid && rom_sel) begin
            rom_ce = 1'b1;
            rom_addr = rom_word_addr;
        end
    end
    
    // RAM interface
    always @(*) begin
        ram_ce = 1'b0;
        ram_we = 1'b0;
        ram_addr = 12'h000;
        
        if (cpu_mem_valid && ram_sel) begin
            ram_ce = 1'b1;
            ram_we = |cpu_mem_wstrb;
            ram_addr = ram_word_addr;
        end
    end
    
    // GPIO interface
    always @(*) begin
        gpio_mem_valid = 1'b0;
        gpio_mem_addr = 32'h00000000;
        gpio_mem_wdata = 32'h00000000;
        gpio_mem_wstrb = 4'h0;
        
        if (cpu_mem_valid && io_sel) begin
            gpio_mem_valid = 1'b1;
            gpio_mem_addr = cpu_mem_addr;
            gpio_mem_wdata = cpu_mem_wdata;
            gpio_mem_wstrb = cpu_mem_wstrb;
        end
    end
    
    // Read data mux
    always @(*) begin
        cpu_mem_rdata = 32'h00000000;
        cpu_mem_ready = 1'b0;
        
        if (rom_sel) begin
            cpu_mem_rdata = rom_rdata;
            cpu_mem_ready = rom_rdata_valid;
        end else if (ram_sel) begin
            cpu_mem_rdata = ram_rdata;
            cpu_mem_ready = ram_rdata_valid;
        end else if (io_sel) begin
            cpu_mem_rdata = gpio_mem_rdata;
            cpu_mem_ready = gpio_mem_ready;
        end
    end

endmodule

