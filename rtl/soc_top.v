// Top-level System-on-Chip module
// Integrates RISC-V core, memory, and peripherals

`include "bus/memory_map.vh"

module soc_top (
    input  wire        clk,
    input  wire        rst_n,
    
    // GPIO Port B
    input  wire [7:0]  gpio_pin_in_b,
    output wire [7:0]  gpio_pin_out_b,
    output wire [7:0]  gpio_pin_dir_b,
    
    // GPIO Port D
    input  wire [7:0]  gpio_pin_in_d,
    output wire [7:0]  gpio_pin_out_d,
    output wire [7:0]  gpio_pin_dir_d,

    // GPIO Port C
    input  wire [7:0]  gpio_pin_in_c,
    output wire [7:0]  gpio_pin_out_c,
    output wire [7:0]  gpio_pin_dir_c,

    // Analog Comparator
    input  wire        ain0,
    input  wire        ain1,

    // Timer0 external clock input
    input  wire        t0_pin,
    
    // Timer0 output compare pins
    output wire        oc0a,
    output wire        oc0b,
    
    // Timer2 output compare pins
    output wire        oc2a,
    output wire        oc2b,
    
    // Watchdog Timer reset request output
    output wire        wdt_reset_req,
    
    // SPI pins
    output wire        spi_sck,
    output wire        spi_mosi,
    input  wire        spi_miso,
    output wire        spi_miso_out,  // Slave mode data output (directly drives MISO)
    output wire        spi_miso_oe,   // Slave mode data output enable
    input  wire        spi_mosi_in,   // Slave mode data input (directly from MOSI)
    input  wire        spi_ss_n,
    input  wire        spi_sck_in     // External SCK input for slave mode
);

    // CPU memory interface
    wire        cpu_mem_valid;
    wire        cpu_mem_instr;
    wire        cpu_mem_ready;
    wire [31:0] cpu_mem_addr;
    wire [31:0] cpu_mem_wdata;
    wire [3:0]  cpu_mem_wstrb;
    wire [31:0] cpu_mem_rdata;
    wire        cpu_trap;
    
    // Bus decoder signals
    wire        rom_ce;
    wire [15:0] rom_addr;
    wire [31:0] rom_rdata;
    wire        rom_rdata_valid;
    
    wire        ram_ce;
    wire        ram_we;
    wire [11:0] ram_addr;
    wire [31:0] ram_rdata;
    wire        ram_rdata_valid;
    
    wire        gpio_mem_valid;
    wire [31:0] gpio_mem_addr;
    wire [31:0] gpio_mem_wdata;
    wire [3:0]  gpio_mem_wstrb;
    wire [31:0] gpio_mem_rdata;
    wire        gpio_mem_ready;

    wire        ac_mem_valid;
    wire [31:0] ac_mem_addr;
    wire [31:0] ac_mem_wdata;
    wire [3:0]  ac_mem_wstrb;
    wire [31:0] ac_mem_rdata;
    wire        ac_mem_ready;
    wire        ac_irq;

    wire        timer0_mem_valid;
    wire [31:0] timer0_mem_addr;
    wire [31:0] timer0_mem_wdata;
    wire [3:0]  timer0_mem_wstrb;
    wire [31:0] timer0_mem_rdata;
    wire        timer0_mem_ready;
    wire        timer0_irq_ovf;
    wire        timer0_irq_compa;
    wire        timer0_irq_compb;

    wire        wdt_mem_valid;
    wire [31:0] wdt_mem_addr;
    wire [31:0] wdt_mem_wdata;
    wire [3:0]  wdt_mem_wstrb;
    wire [31:0] wdt_mem_rdata;
    wire        wdt_mem_ready;
    wire        wdt_irq;
    wire        wdt_reset_req_internal;

    wire        timer2_mem_valid;
    wire [31:0] timer2_mem_addr;
    wire [31:0] timer2_mem_wdata;
    wire [3:0]  timer2_mem_wstrb;
    wire [31:0] timer2_mem_rdata;
    wire        timer2_mem_ready;
    wire        timer2_irq_ovf;
    wire        timer2_irq_compa;
    wire        timer2_irq_compb;

    wire        spi_mem_valid;
    wire [31:0] spi_mem_addr;
    wire [31:0] spi_mem_wdata;
    wire [3:0]  spi_mem_wstrb;
    wire [31:0] spi_mem_rdata;
    wire        spi_mem_ready;
    wire        spi_irq;
    
    // Instantiate RISC-V core
    picorv32 #(
        .ENABLE_COUNTERS(1),
        .ENABLE_COUNTERS64(1),
        .ENABLE_REGS_16_31(1),
        .ENABLE_REGS_DUALPORT(1),
        .LATCHED_MEM_RDATA(0),
        .TWO_STAGE_SHIFT(1),
        .BARREL_SHIFTER(0),
        .TWO_CYCLE_COMPARE(0),
        .TWO_CYCLE_ALU(0),
        .COMPRESSED_ISA(0),
        .CATCH_MISALIGN(1),
        .CATCH_ILLINSN(1),
        .ENABLE_PCPI(0),
        .ENABLE_MUL(0),
        .ENABLE_FAST_MUL(0),
        .ENABLE_DIV(0),
        .ENABLE_IRQ(0),
        .ENABLE_IRQ_QREGS(1),
        .ENABLE_IRQ_TIMER(1),
        .ENABLE_TRACE(0),
        .REGS_INIT_ZERO(0),
        .MASKED_IRQ(32'h00000000),
        .LATCHED_IRQ(32'hffffffff),
        .PROGADDR_RESET(32'h00000000),
        .PROGADDR_IRQ(32'h00000010),
        .STACKADDR(32'hffffffff)
    ) cpu (
        .clk(clk),
        .resetn(rst_n),
        .trap(cpu_trap),
        .mem_valid(cpu_mem_valid),
        .mem_instr(cpu_mem_instr),
        .mem_ready(cpu_mem_ready),
        .mem_addr(cpu_mem_addr),
        .mem_wdata(cpu_mem_wdata),
        .mem_wstrb(cpu_mem_wstrb),
        .mem_rdata(cpu_mem_rdata),
        .mem_la_read(),
        .mem_la_write(),
        .mem_la_addr(),
        .mem_la_wdata(),
        .mem_la_wstrb(),
        .pcpi_valid(),
        .pcpi_insn(),
        .pcpi_rs1(),
        .pcpi_rs2(),
        .pcpi_wr(1'b0),
        .pcpi_rd(32'h00000000),
        .pcpi_wait(1'b0),
        .pcpi_ready(1'b0),
        .irq({31'b0, ac_irq}), // Connect AC IRQ to bit 0 for now
        .eoi()
    );
    
    // Instantiate bus decoder
    bus_decoder decoder (
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
        .ac_mem_ready(ac_mem_ready),
        .timer0_mem_valid(timer0_mem_valid),
        .timer0_mem_addr(timer0_mem_addr),
        .timer0_mem_wdata(timer0_mem_wdata),
        .timer0_mem_wstrb(timer0_mem_wstrb),
        .timer0_mem_rdata(timer0_mem_rdata),
        .timer0_mem_ready(timer0_mem_ready),
        .wdt_mem_valid(wdt_mem_valid),
        .wdt_mem_addr(wdt_mem_addr),
        .wdt_mem_wdata(wdt_mem_wdata),
        .wdt_mem_wstrb(wdt_mem_wstrb),
        .wdt_mem_rdata(wdt_mem_rdata),
        .wdt_mem_ready(wdt_mem_ready),
        .timer2_mem_valid(timer2_mem_valid),
        .timer2_mem_addr(timer2_mem_addr),
        .timer2_mem_wdata(timer2_mem_wdata),
        .timer2_mem_wstrb(timer2_mem_wstrb),
        .timer2_mem_rdata(timer2_mem_rdata),
        .timer2_mem_ready(timer2_mem_ready),
        .spi_mem_valid(spi_mem_valid),
        .spi_mem_addr(spi_mem_addr),
        .spi_mem_wdata(spi_mem_wdata),
        .spi_mem_wstrb(spi_mem_wstrb),
        .spi_mem_rdata(spi_mem_rdata),
        .spi_mem_ready(spi_mem_ready)
    );
    
    // Instantiate ROM
    rom #(
        .ADDR_WIDTH(16),
        .DATA_WIDTH(32),
        .INIT_FILE("firmware.mem")
    ) program_rom (
        .clk(clk),
        .rst_n(rst_n),
        .ce(rom_ce),
        .addr(rom_addr),
        .rdata(rom_rdata),
        .rdata_valid(rom_rdata_valid)
    );
    
    // Instantiate RAM
    ram #(
        .ADDR_WIDTH(12),
        .DATA_WIDTH(32)
    ) data_ram (
        .clk(clk),
        .rst_n(rst_n),
        .ce(ram_ce),
        .we(ram_we),
        .wstrb(cpu_mem_wstrb),
        .addr(ram_addr),
        .wdata(cpu_mem_wdata),
        .rdata(ram_rdata),
        .rdata_valid(ram_rdata_valid)
    );
    
    // Instantiate GPIO
    gpio gpio_inst (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(gpio_mem_valid),
        .mem_addr(gpio_mem_addr),
        .mem_wdata(gpio_mem_wdata),
        .mem_wstrb(gpio_mem_wstrb),
        .mem_rdata(gpio_mem_rdata),
        .mem_ready(gpio_mem_ready),
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

    // Instantiate Analog Comparator
    analog_comparator ac_inst (
        .clk(clk),
        .rst_n(rst_n),
        .ain0(ain0),
        .ain1(ain1),
        .mem_valid(ac_mem_valid),
        .mem_addr(ac_mem_addr),
        .mem_wdata(ac_mem_wdata),
        .mem_wstrb(ac_mem_wstrb),
        .mem_rdata(ac_mem_rdata),
        .mem_ready(ac_mem_ready),
        .irq(ac_irq)
    );

    // Instantiate Timer/Counter 0
    timer0 timer0_inst (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(timer0_mem_valid),
        .mem_addr(timer0_mem_addr),
        .mem_wdata(timer0_mem_wdata),
        .mem_wstrb(timer0_mem_wstrb),
        .mem_rdata(timer0_mem_rdata),
        .mem_ready(timer0_mem_ready),
        .t0_pin(t0_pin),
        .oc0a(oc0a),
        .oc0b(oc0b),
        .irq_ovf(timer0_irq_ovf),
        .irq_compa(timer0_irq_compa),
        .irq_compb(timer0_irq_compb)
    );

    // Instantiate Timer/Counter 2
    timer2 timer2_inst (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(timer2_mem_valid),
        .mem_addr(timer2_mem_addr),
        .mem_wdata(timer2_mem_wdata),
        .mem_wstrb(timer2_mem_wstrb),
        .mem_rdata(timer2_mem_rdata),
        .mem_ready(timer2_mem_ready),
        .oc2a(oc2a),
        .oc2b(oc2b),
        .irq_ovf(timer2_irq_ovf),
        .irq_compa(timer2_irq_compa),
        .irq_compb(timer2_irq_compb)
    );

    // Instantiate Watchdog Timer
    watchdog_timer wdt_inst (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(wdt_mem_valid),
        .mem_addr(wdt_mem_addr),
        .mem_wdata(wdt_mem_wdata),
        .mem_wstrb(wdt_mem_wstrb),
        .mem_rdata(wdt_mem_rdata),
        .mem_ready(wdt_mem_ready),
        .irq_wdt(wdt_irq),
        .wdt_reset_req(wdt_reset_req_internal)
    );

    // Export WDT reset request
    assign wdt_reset_req = wdt_reset_req_internal;

    // Instantiate SPI
    spi spi_inst (
        .clk(clk),
        .rst_n(rst_n),
        .mem_valid(spi_mem_valid),
        .mem_addr(spi_mem_addr),
        .mem_wdata(spi_mem_wdata),
        .mem_wstrb(spi_mem_wstrb),
        .mem_rdata(spi_mem_rdata),
        .mem_ready(spi_mem_ready),
        .sck(spi_sck),
        .mosi(spi_mosi),
        .miso(spi_miso),
        .miso_out(spi_miso_out),
        .miso_oe(spi_miso_oe),
        .mosi_in(spi_mosi_in),
        .ss_n(spi_ss_n),
        .sck_in(spi_sck_in),
        .irq_spi(spi_irq)
    );

endmodule
