// Watchdog Timer - ATmega328P-compatible implementation
//
// Features per ATmega328P Datasheet Section 10.8:
// - Separate 128kHz oscillator (simulated via prescaler from system clock)
// - Three operating modes: Interrupt, System Reset, Interrupt + System Reset
// - Selectable time-out periods from 16ms to 8s (10 prescaler values)
// - Timed sequence for changing WDE or WDP bits
// - WDRF flag in MCUSR for detecting watchdog reset
//
// Registers:
// - MCUSR (0x54): MCU Status Register - contains WDRF
// - WDTCSR (0x60): Watchdog Timer Control and Status Register
// - WDR (0x61): Watchdog Reset - write 0xA5 to reset counter (custom for RISC-V)

`include "bus/memory_map.vh"

module watchdog_timer (
    input  wire        clk,
    input  wire        rst_n,
    
    // Bus Interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // Interrupt output
    output wire        irq_wdt,
    
    // System reset request (active high)
    output reg         wdt_reset_req
);

    // =========================================================================
    // Register Definitions (ATmega328P compatible)
    // =========================================================================
    
    // MCUSR - MCU Status Register (0x54)
    // Bit 3: WDRF - Watchdog System Reset Flag
    reg wdrf;
    
    // WDTCSR - Watchdog Timer Control and Status Register (0x60)
    reg wdif;  // Bit 7: Watchdog Interrupt Flag
    reg wdie;  // Bit 6: Watchdog Interrupt Enable
    reg wdce;  // Bit 4: Watchdog Change Enable (auto-clears after 4 cycles)
    reg wde;   // Bit 3: Watchdog System Reset Enable
    reg wdp3;  // Bit 5: Prescaler bit 3
    reg [2:0] wdp;  // Bits 2:0: Prescaler bits 2:0
    
    // Combined prescaler value
    wire [3:0] wdp_full = {wdp3, wdp};
    
    // =========================================================================
    // Address Decoding
    // =========================================================================
    
    wire [31:0] word_addr = {mem_addr[31:2], 2'b00};
    
    wire sel_word_54 = (word_addr == 32'h20000054);  // MCUSR
    wire sel_word_60 = (word_addr == 32'h20000060);  // WDTCSR, WDR
    
    wire sel_mcusr_wr  = sel_word_54 && mem_wstrb[0];
    wire sel_wdtcsr_wr = sel_word_60 && mem_wstrb[0];
    wire sel_wdr_wr    = sel_word_60 && mem_wstrb[1];
    
    wire wdt_sel = sel_word_54 | sel_word_60;
    
    wire [7:0] write_byte_0 = mem_wdata[7:0];
    wire [7:0] write_byte_1 = mem_wdata[15:8];
    
    // =========================================================================
    // Watchdog Oscillator Simulation
    // =========================================================================
    
    parameter WDT_OSC_DIV = 128;
    
    reg [9:0] osc_counter;
    wire wdt_osc_tick = (osc_counter == WDT_OSC_DIV - 1);
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            osc_counter <= 10'b0;
        end else begin
            if (wdt_osc_tick) begin
                osc_counter <= 10'b0;
            end else begin
                osc_counter <= osc_counter + 1'b1;
            end
        end
    end
    
    // =========================================================================
    // Timeout Period Calculation
    // =========================================================================
    
    reg [19:0] timeout_cycles;
    
    always @(*) begin
        case (wdp_full)
            4'b0000: timeout_cycles = 20'd2048;
            4'b0001: timeout_cycles = 20'd4096;
            4'b0010: timeout_cycles = 20'd8192;
            4'b0011: timeout_cycles = 20'd16384;
            4'b0100: timeout_cycles = 20'd32768;
            4'b0101: timeout_cycles = 20'd65536;
            4'b0110: timeout_cycles = 20'd131072;
            4'b0111: timeout_cycles = 20'd262144;
            4'b1000: timeout_cycles = 20'd524288;
            4'b1001: timeout_cycles = 20'd1048575;
            default: timeout_cycles = 20'd2048;
        endcase
    end
    
    // =========================================================================
    // Watchdog Counter and Timeout
    // =========================================================================
    
    reg [19:0] wdt_counter;
    wire wdt_enabled = wde | wdie;
    wire wdt_timeout = wdt_enabled && (wdt_counter >= timeout_cycles);
    reg timeout_event;
    
    // =========================================================================
    // WDCE Timed Sequence Counter
    // =========================================================================
    
    reg [5:0] wdce_counter;
    wire wdce_active = (wdce_counter > 0);
    
    // =========================================================================
    // Bus Interface and Register Updates (all in one block for correct timing)
    // =========================================================================
    
    wire [7:0] mcusr_byte  = {4'b0000, wdrf, 3'b000};
    wire [7:0] wdtcsr_byte = {wdif, wdie, wdp3, wdce, wde, wdp};
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            // Reset all registers
            wdrf <= 1'b0;
            wdif <= 1'b0;
            wdie <= 1'b0;
            wdce <= 1'b0;
            wde  <= 1'b0;
            wdp3 <= 1'b0;
            wdp  <= 3'b0;
            wdce_counter <= 6'b0;
            wdt_counter <= 20'b0;
            timeout_event <= 1'b0;
            wdt_reset_req <= 1'b0;
            mem_ready <= 1'b0;
            mem_rdata <= 32'h0;
        end else begin
            // Default: clear pulse signals
            mem_ready <= 1'b0;
            timeout_event <= 1'b0;
            wdt_reset_req <= 1'b0;
            
            // =====================================================
            // Bus Transaction Handling (only on first cycle)
            // =====================================================
            if (mem_valid && wdt_sel && !mem_ready) begin
                mem_ready <= 1'b1;
                
                // WDTCSR write
                if (sel_wdtcsr_wr) begin
                    // WDIF: write-1-to-clear
                    if (write_byte_0[7]) begin
                        wdif <= 1'b0;
                    end
                    
                    // WDIE: always writable
                    wdie <= write_byte_0[6];
                    
                    // WDCE: always writable (but auto-clears)
                    wdce <= write_byte_0[4];
                    
                    // Start timed sequence if writing WDCE=1 and WDE=1
                    // Note: Original ATmega328P uses 4 CPU cycles, but since we're
                    // running on RISC-V with multi-cycle instructions, we use a
                    // larger window (32 cycles) to allow for instruction execution
                    if (write_byte_0[4] && write_byte_0[3]) begin
                        wdce_counter <= 6'd32;
                    end
                    
                    // WDE and WDP: only changeable during timed sequence
                    if (wdce_active) begin
                        wde  <= write_byte_0[3];
                        wdp3 <= write_byte_0[5];
                        wdp  <= write_byte_0[2:0];
                    end else begin
                        // WDE can only be SET (not cleared) outside sequence
                        wde <= wde | write_byte_0[3];
                    end
                end
                
                // WDR write (Watchdog Reset command)
                if (sel_wdr_wr && (write_byte_1 == 8'hA5)) begin
                    wdt_counter <= 20'b0;
                end
                
                // MCUSR write
                if (sel_mcusr_wr) begin
                    // WDRF: clear by writing 0
                    if (!write_byte_0[3]) begin
                        wdrf <= 1'b0;
                    end
                end
                
                // Read data
                if (sel_word_54) begin
                    mem_rdata <= {24'h0, mcusr_byte};
                end else if (sel_word_60) begin
                    mem_rdata <= {16'h0, 8'h00, wdtcsr_byte};
                end else begin
                    mem_rdata <= 32'h0;
                end
            end
            
            // =====================================================
            // WDCE Counter Decrement (outside bus transaction)
            // =====================================================
            if (wdce_counter > 0 && !(mem_valid && wdt_sel && !mem_ready && sel_wdtcsr_wr)) begin
                wdce_counter <= wdce_counter - 1'b1;
                if (wdce_counter == 1) begin
                    wdce <= 1'b0;
                end
            end
            
            // =====================================================
            // Watchdog Counter Logic
            // =====================================================
            // Note: WDR command (inside bus transaction above) takes priority
            // by being the ONLY place that sets wdt_counter when WDR is issued
            if (!(mem_valid && wdt_sel && !mem_ready && sel_wdr_wr && (write_byte_1 == 8'hA5))) begin
                // Only update counter if WDR was NOT issued this cycle
                if (wdt_timeout) begin
                    timeout_event <= 1'b1;
                    wdt_counter <= 20'b0;
                    
                    // Handle timeout based on mode
                    if (wdie) begin
                        // Interrupt mode: set WDIF
                        wdif <= 1'b1;
                        // In INT+RST mode, clear WDIE after interrupt
                        if (wde) begin
                            wdie <= 1'b0;
                        end
                    end else if (wde) begin
                        // Reset mode: set WDRF and request reset
                        wdrf <= 1'b1;
                        wdt_reset_req <= 1'b1;
                    end
                end else if (wdt_enabled && wdt_osc_tick) begin
                    wdt_counter <= wdt_counter + 1'b1;
                end else if (!wdt_enabled) begin
                    wdt_counter <= 20'b0;
                end
            end
        end
    end
    
    // Interrupt output - fires when WDIF is set (interrupt is pending)
    // WDIE only controls whether interrupts are generated, not the IRQ line
    assign irq_wdt = wdif;

endmodule
