// SPI - Serial Peripheral Interface
// ATmega328P-compatible implementation
// 
// Features:
// - Master and Slave modes
// - 8 clock rate options (fosc/2, /4, /8, /16, /32, /64, /128)
// - 4 SPI modes (CPOL/CPHA combinations)
// - MSB or LSB first data order
// - Interrupt on transfer complete
// - Write collision detection

`include "bus/memory_map.vh"

module spi (
    input  wire        clk,
    input  wire        rst_n,
    
    // Bus Interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // SPI Pins
    output wire        sck,       // Serial Clock (output in Master mode)
    output wire        mosi,      // Master Out Slave In (active in Master mode)
    input  wire        miso,      // Master In Slave Out (sampled in Master mode)
    output wire        miso_out,  // Slave data output (active in Slave mode, directly drives MISO)
    output wire        miso_oe,   // Slave data output enable (active in Slave mode)
    input  wire        mosi_in,   // Slave data input (sampled in Slave mode, directly from MOSI)
    input  wire        ss_n,      // Slave Select (active low, input in Slave mode)
    
    // Directly drive internal SCK for slave mode input
    input  wire        sck_in,    // External SCK input for slave mode
    
    // Interrupt output
    output wire        irq_spi
);

    // =========================================================================
    // Register Definitions (ATmega328P compatible)
    // =========================================================================
    
    // SPCR - SPI Control Register (0x4C)
    reg       spie;     // Bit 7: SPI Interrupt Enable
    reg       spe;      // Bit 6: SPI Enable
    reg       dord;     // Bit 5: Data Order (0=MSB first, 1=LSB first)
    reg       mstr;     // Bit 4: Master/Slave Select
    reg       cpol;     // Bit 3: Clock Polarity
    reg       cpha;     // Bit 2: Clock Phase
    reg [1:0] spr;      // Bits 1:0: SPI Clock Rate Select
    
    // SPSR - SPI Status Register (0x4D)
    reg       spif;     // Bit 7: SPI Interrupt Flag (read-only)
    reg       wcol;     // Bit 6: Write Collision Flag (read-only)
    reg       spi2x;    // Bit 0: Double SPI Speed
    
    // SPDR - SPI Data Register (0x4E)
    reg [7:0] tx_data;      // Transmit data buffer
    reg [7:0] rx_data;      // Receive data buffer
    reg [7:0] shift_reg;    // Shift register for active transfer
    
    // =========================================================================
    // Address Decode - Word-aligned addresses with byte lane selection
    // =========================================================================
    wire [31:0] word_addr = {mem_addr[31:2], 2'b00};
    
    // SPI registers are in word 0x2000004C:
    // byte 0 = SPCR (0x4C), byte 1 = SPSR (0x4D), byte 2 = SPDR (0x4E)
    wire sel_word_4c = (word_addr == 32'h2000004C);
    
    // Write selection based on byte lane
    wire sel_spcr_wr = sel_word_4c && mem_wstrb[0];   // Byte 0
    wire sel_spsr_wr = sel_word_4c && mem_wstrb[1];   // Byte 1
    wire sel_spdr_wr = sel_word_4c && mem_wstrb[2];   // Byte 2
    
    wire spi_sel = sel_word_4c;
    
    // =========================================================================
    // Write Data Extraction
    // =========================================================================
    wire [7:0] write_byte_0 = mem_wdata[7:0];
    wire [7:0] write_byte_1 = mem_wdata[15:8];
    wire [7:0] write_byte_2 = mem_wdata[23:16];
    
    // =========================================================================
    // Bus transaction detection
    // =========================================================================
    wire bus_write = mem_valid && spi_sel && |mem_wstrb;
    wire bus_read  = mem_valid && spi_sel && ~|mem_wstrb;
    
    // =========================================================================
    // SPIF Clear Mechanism  
    // ATmega328P clears SPIF by reading SPSR (with SPIF set) then reading SPDR
    // Since our bus returns all registers in one word, we track:
    // 1. First read with SPIF set = latch the flag  
    // 2. Second read = clear SPIF
    // =========================================================================
    reg spsr_read_with_spif;  // Latched when SPSR is read while SPIF=1
    
    // Detect read transaction: valid read access being acknowledged
    // This is high on the same cycle mem_ready goes high
    wire is_read_access = mem_valid && spi_sel && ~|mem_wstrb;
    
    // Use the condition for generating mem_ready to detect read completion
    // mem_ready is set when: mem_valid && spi_sel && !mem_ready
    // So read_completed is true on that cycle
    wire read_completing = is_read_access && !mem_ready;
    
    // We need to delay this by one cycle to get the "completed" pulse
    reg read_completed;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            read_completed <= 1'b0;
        else
            read_completed <= read_completing;
    end
    
    // =========================================================================
    // Clock Prescaler and SCK Generation
    // =========================================================================
    reg [6:0] prescaler_cnt;
    reg       sck_internal;
    
    // Prescaler divider based on SPI2X and SPR
    // Per ATmega328P datasheet Table 18-5: SPI2X=1, SPR=11 yields fosc/64 (same as SPI2X=0, SPR=10)
    reg [6:0] prescaler_top;
    always @(*) begin
        case ({spi2x, spr})
            3'b000: prescaler_top = 7'd1;    // fosc/4  (toggle every 2 clocks)
            3'b001: prescaler_top = 7'd7;    // fosc/16
            3'b010: prescaler_top = 7'd31;   // fosc/64
            3'b011: prescaler_top = 7'd63;   // fosc/128
            3'b100: prescaler_top = 7'd0;    // fosc/2  (toggle every clock)
            3'b101: prescaler_top = 7'd3;    // fosc/8
            3'b110: prescaler_top = 7'd15;   // fosc/32
            3'b111: prescaler_top = 7'd31;   // fosc/64 (same as 3'b010 per datasheet)
            default: prescaler_top = 7'd1;
        endcase
    end
    
    // =========================================================================
    // Transfer State Machine
    // =========================================================================
    localparam STATE_IDLE     = 2'b00;
    localparam STATE_TRANSFER = 2'b01;
    localparam STATE_COMPLETE = 2'b10;
    
    reg [1:0] state;
    reg [3:0] bit_cnt;      // Use 4 bits to count 0-15 (8 samples + 8 shifts)
    reg       transfer_active;
    reg       first_edge;   // Track if this is the first edge after transfer start
    
    // SCK idle level based on CPOL
    wire sck_idle = cpol;
    
    // Start transfer condition
    wire start_transfer = spe && mstr && sel_spdr_wr && bus_write && (state == STATE_IDLE);
    
    // Prescaler and SCK generation
    wire prescaler_match = (prescaler_cnt >= prescaler_top);
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            prescaler_cnt <= 7'd0;
            sck_internal <= 1'b0;
            first_edge <= 1'b0;
        end else if (state == STATE_IDLE && start_transfer) begin
            // Initialize SCK to idle level when starting
            prescaler_cnt <= 7'd0;
            sck_internal <= sck_idle;
            first_edge <= 1'b1;
        end else if (state == STATE_TRANSFER) begin
            if (prescaler_match) begin
                prescaler_cnt <= 7'd0;
                sck_internal <= ~sck_internal;
                first_edge <= 1'b0;
            end else begin
                prescaler_cnt <= prescaler_cnt + 1'b1;
            end
        end else begin
            prescaler_cnt <= 7'd0;
            sck_internal <= sck_idle;
            first_edge <= 1'b0;
        end
    end
    
    // Detect SCK edges for data shifting
    reg sck_prev;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            sck_prev <= 1'b0;
        else
            sck_prev <= sck_internal;
    end
    
    wire sck_rising  = sck_internal && !sck_prev;
    wire sck_falling = !sck_internal && sck_prev;
    
    // Determine sample and shift edges based on mode
    // Mode 0 (CPOL=0, CPHA=0): Sample on rising edge, shift on falling edge
    // Mode 1 (CPOL=0, CPHA=1): Sample on falling edge, shift on rising edge  
    // Mode 2 (CPOL=1, CPHA=0): Sample on falling edge, shift on rising edge
    // Mode 3 (CPOL=1, CPHA=1): Sample on rising edge, shift on falling edge
    wire do_sample = (cpol == cpha) ? sck_rising : sck_falling;
    wire do_shift  = (cpol == cpha) ? sck_falling : sck_rising;
    
    // =========================================================================
    // Slave Mode Logic
    // =========================================================================
    
    // Synchronize external SCK for slave mode
    reg sck_sync1, sck_sync2, sck_sync_prev;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            sck_sync1 <= 1'b0;
            sck_sync2 <= 1'b0;
            sck_sync_prev <= 1'b0;
        end else begin
            sck_sync1 <= sck_in;
            sck_sync2 <= sck_sync1;
            sck_sync_prev <= sck_sync2;
        end
    end
    
    wire slave_sck_rising  = sck_sync2 && !sck_sync_prev;
    wire slave_sck_falling = !sck_sync2 && sck_sync_prev;
    
    // Slave mode sample/shift edges
    wire slave_sample = (cpol == cpha) ? slave_sck_rising : slave_sck_falling;
    wire slave_shift  = (cpol == cpha) ? slave_sck_falling : slave_sck_rising;
    
    // Slave select synchronizer
    reg ss_sync1, ss_sync2;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ss_sync1 <= 1'b1;
            ss_sync2 <= 1'b1;
        end else begin
            ss_sync1 <= ss_n;
            ss_sync2 <= ss_sync1;
        end
    end
    
    wire slave_selected = !ss_sync2 && spe && !mstr;
    
    // =========================================================================
    // Main State Machine and Shift Register
    // =========================================================================
    
    reg [2:0] sample_cnt;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= STATE_IDLE;
            bit_cnt <= 4'd0;
            sample_cnt <= 3'd0;
            shift_reg <= 8'h00;
            rx_data <= 8'h00;
            tx_data <= 8'h00;
            transfer_active <= 1'b0;
        end else begin
            case (state)
                STATE_IDLE: begin
                    transfer_active <= 1'b0;
                    bit_cnt <= 4'd0;
                    sample_cnt <= 3'd0;
                    
                    if (mstr) begin
                        // Master mode - start on SPDR write
                        if (start_transfer) begin
                            shift_reg <= write_byte_2;
                            tx_data <= write_byte_2;
                            state <= STATE_TRANSFER;
                            transfer_active <= 1'b1;
                        end
                    end else if (slave_selected) begin
                        // Slave mode - start when selected
                        shift_reg <= tx_data;
                        state <= STATE_TRANSFER;
                        transfer_active <= 1'b1;
                    end
                end
                
                STATE_TRANSFER: begin
                    if (mstr) begin
                        // Master mode shifting
                        if (do_sample) begin
                            // Sample MISO
                            if (dord)
                                shift_reg <= {miso, shift_reg[7:1]};  // LSB first
                            else
                                shift_reg <= {shift_reg[6:0], miso};  // MSB first
                            sample_cnt <= sample_cnt + 1'b1;
                        end
                        
                        if (do_shift) begin
                            bit_cnt <= bit_cnt + 1'b1;
                        end
                        
                        // Complete after 8 samples
                        if (sample_cnt == 3'd7 && do_sample) begin
                            state <= STATE_COMPLETE;
                        end
                    end else begin
                        // Slave mode shifting
                        if (!slave_selected) begin
                            // Deselected - abort transfer
                            state <= STATE_IDLE;
                        end else begin
                            if (slave_sample) begin
                                // Slave receives on MOSI (mosi_in)
                                if (dord)
                                    shift_reg <= {mosi_in, shift_reg[7:1]};
                                else
                                    shift_reg <= {shift_reg[6:0], mosi_in};
                                sample_cnt <= sample_cnt + 1'b1;
                            end
                            
                            if (slave_shift) begin
                                bit_cnt <= bit_cnt + 1'b1;
                            end
                            
                            // Complete after 8 samples
                            if (sample_cnt == 3'd7 && slave_sample) begin
                                state <= STATE_COMPLETE;
                            end
                        end
                    end
                end
                
                STATE_COMPLETE: begin
                    rx_data <= shift_reg;
                    transfer_active <= 1'b0;
                    state <= STATE_IDLE;
                end
                
                default: state <= STATE_IDLE;
            endcase
            
            // Handle SPDR write for tx_data update (when not starting transfer)
            if (sel_spdr_wr && bus_write && state != STATE_IDLE) begin
                // Write collision - don't update tx_data during transfer
            end else if (sel_spdr_wr && bus_write && !start_transfer) begin
                tx_data <= write_byte_2;
            end
        end
    end
    
    // =========================================================================
    // MOSI Output (Master mode)
    // =========================================================================
    
    // Output the current bit being transmitted
    wire tx_bit = dord ? shift_reg[0] : shift_reg[7];
    
    // Master mode: output on MOSI
    assign mosi = (spe && mstr && transfer_active) ? tx_bit : 1'b0;
    
    // =========================================================================
    // MISO Output (Slave mode)
    // =========================================================================
    
    // Slave mode: output on MISO
    assign miso_out = tx_bit;
    assign miso_oe  = spe && !mstr && slave_selected && transfer_active;
    
    // =========================================================================
    // SCK Output
    // =========================================================================
    
    assign sck = (spe && mstr) ? sck_internal : sck_idle;
    
    // =========================================================================
    // Control Register Writes
    // =========================================================================
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            spie  <= 1'b0;
            spe   <= 1'b0;
            dord  <= 1'b0;
            mstr  <= 1'b0;
            cpol  <= 1'b0;
            cpha  <= 1'b0;
            spr   <= 2'b00;
            spi2x <= 1'b0;
        end else if (bus_write) begin
            // SPCR write (byte 0)
            if (sel_spcr_wr) begin
                spie <= write_byte_0[7];
                spe  <= write_byte_0[6];
                dord <= write_byte_0[5];
                mstr <= write_byte_0[4];
                cpol <= write_byte_0[3];
                cpha <= write_byte_0[2];
                spr  <= write_byte_0[1:0];
            end
            
            // SPSR write (byte 1) - only SPI2X is writable
            if (sel_spsr_wr) begin
                spi2x <= write_byte_1[0];
            end
        end
    end
    
    // =========================================================================
    // Status Flags (SPIF, WCOL)
    // =========================================================================
    
    // Track SPSR read with SPIF set for clear sequence
    // We need to track: read1 with SPIF -> read2 clears SPIF
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            spsr_read_with_spif <= 1'b0;
        end else begin
            if (read_completed) begin
                if (spsr_read_with_spif) begin
                    // Second read - clear the latch
                    spsr_read_with_spif <= 1'b0;
                end else if (spif) begin
                    // First read with SPIF set - set the latch
                    spsr_read_with_spif <= 1'b1;
                end
            end
        end
    end
    
    // Clear signal is asserted on the second read when spsr_read_with_spif was set
    wire clear_flags = read_completed && spsr_read_with_spif;
    
    // SPIF flag - set on transfer complete, cleared by read sequence
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            spif <= 1'b0;
        end else begin
            if (state == STATE_COMPLETE) begin
                spif <= 1'b1;
            end else if (clear_flags) begin
                spif <= 1'b0;
            end
        end
    end
    
    // WCOL flag - set on write collision, cleared by read sequence
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wcol <= 1'b0;
        end else begin
            if (sel_spdr_wr && bus_write && state == STATE_TRANSFER) begin
                wcol <= 1'b1;
            end else if (clear_flags) begin
                wcol <= 1'b0;
            end
        end
    end
    
    // =========================================================================
    // Interrupt Output
    // =========================================================================
    
    assign irq_spi = spie && spif;
    
    // =========================================================================
    // Bus Interface - Ready and Read Data
    // =========================================================================
    
    wire [7:0] spcr_byte = {spie, spe, dord, mstr, cpol, cpha, spr};
    wire [7:0] spsr_byte = {spif, wcol, 5'b00000, spi2x};
    wire [7:0] spdr_byte = rx_data;
    
    // Pack read data - word 0x4C contains SPCR, SPSR, SPDR in bytes 0, 1, 2
    wire [31:0] read_word = {8'h00, spdr_byte, spsr_byte, spcr_byte};
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            mem_ready <= 1'b0;
            mem_rdata <= 32'h0;
        end else begin
            mem_ready <= 1'b0;
            
            if (mem_valid && spi_sel && !mem_ready) begin
                mem_ready <= 1'b1;
                mem_rdata <= read_word;
            end
        end
    end

endmodule
