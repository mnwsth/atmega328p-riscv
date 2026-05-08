// Bus Decoder - Routes memory accesses to appropriate peripherals
// Simple address-based decoding
// verilog_format: off
`include "memory_map.vh"

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
    input  wire        gpio_mem_ready,

    // SPI interface
    output reg         spi_mem_valid,
    output reg  [31:0] spi_mem_addr,
    output reg  [31:0] spi_mem_wdata,
    output reg  [3:0]  spi_mem_wstrb,
    input  wire [31:0] spi_mem_rdata,
    input  wire        spi_mem_ready
);

    // Address decoding
    wire rom_sel = (cpu_mem_addr & `MEM_ROM_MASK) == `MEM_ROM_BASE;
    wire ram_sel = (cpu_mem_addr & `MEM_RAM_MASK) == `MEM_RAM_BASE;
    wire io_sel  = (cpu_mem_addr & `MEM_IO_MASK)  == `MEM_IO_BASE;

    // Word-aligned address for peripheral selection
    wire [31:0] word_addr = {cpu_mem_addr[31:2], 2'b00};

    // Peripheral selection within IO space
    wire ac_sel = (cpu_mem_addr == `ACSR);

    // Timer0 word-aligned selection (Timer0 spans multiple words)
    wire timer0_sel = (word_addr == 32'h20000034) ||  // TIFR0
                      (word_addr == 32'h20000044) ||  // TCCR0A, TCCR0B, TCNT0, OCR0A
                      (word_addr == 32'h20000048) ||  // OCR0B
                      (word_addr == 32'h2000006C);    // TIMSK0

    // Watchdog Timer word-aligned selection
    wire wdt_sel = (word_addr == 32'h20000054) ||     // MCUSR
                   (word_addr == 32'h20000060);       // WDTCSR, WDR

    // Timer2 word-aligned selection (Timer2 spans multiple words)
    // Note: TIFR2 (0x37) is in word 0x34, but we need separate handling
    // to avoid conflict with TIFR0 (0x35) which is in same word
    wire timer2_sel = (word_addr == 32'h20000034) ||  // TIFR2 (byte 3)
                      (word_addr == 32'h20000070) ||  // TIMSK2
                      (word_addr == 32'h200000B0) ||  // TCCR2A, TCCR2B, TCNT2, OCR2A
                      (word_addr == 32'h200000B4);    // OCR2B

    // SPI word-aligned selection
    // SPI registers: SPCR (0x4C), SPSR (0x4D), SPDR (0x4E) all in word 0x4C
    wire spi_sel = (word_addr == 32'h2000004C);

    // GPIO is default for other IO addresses
    wire gpio_sel = io_sel && !ac_sel && !timer0_sel && !wdt_sel && !timer2_sel && !spi_sel;

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

        if (cpu_mem_valid && gpio_sel) begin
            gpio_mem_valid = 1'b1;
            gpio_mem_addr = cpu_mem_addr;
            gpio_mem_wdata = cpu_mem_wdata;
            gpio_mem_wstrb = cpu_mem_wstrb;
        end
    end

    // SPI interface
    always @(*) begin
        spi_mem_valid = 1'b0;
        spi_mem_addr = 32'h00000000;
        spi_mem_wdata = 32'h00000000;
        spi_mem_wstrb = 4'h0;

        if (cpu_mem_valid && spi_sel) begin
            spi_mem_valid = 1'b1;
            spi_mem_addr = cpu_mem_addr;
            spi_mem_wdata = cpu_mem_wdata;
            spi_mem_wstrb = cpu_mem_wstrb;
        end
    end

    // Read data mux
    // Note: For word 0x20000034, both Timer0 (TIFR0 @ byte 1) and Timer2 (TIFR2 @ byte 3)
    // share the same word. We merge their responses.
    wire shared_word_34 = (word_addr == 32'h20000034);

    always @(*) begin
        cpu_mem_rdata = 32'h00000000;
        cpu_mem_ready = 1'b0;

        if (rom_sel) begin
            cpu_mem_rdata = rom_rdata;
            cpu_mem_ready = rom_rdata_valid;
        end else if (ram_sel) begin
            cpu_mem_rdata = ram_rdata;
            cpu_mem_ready = ram_rdata_valid;
        end else if (gpio_sel) begin
            cpu_mem_rdata = gpio_mem_rdata;
            cpu_mem_ready = gpio_mem_ready;
        end else if (spi_sel) begin
            cpu_mem_rdata = spi_mem_rdata;
            cpu_mem_ready = spi_mem_ready;
        end
    end

endmodule
