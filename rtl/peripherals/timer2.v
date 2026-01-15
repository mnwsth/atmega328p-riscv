// Timer/Counter 2 - 8-bit Timer with PWM
// ATmega328P-compatible implementation
// 
// Features:
// - 8-bit counter (TCNT2)
// - Two output compare registers (OCR2A, OCR2B)
// - Prescaler: 1, 8, 32, 64, 128, 256, 1024 (different from Timer0)
// - Waveform Generation Modes: Normal, CTC, Fast PWM, Phase Correct PWM
// - Compare Match outputs (OC2A, OC2B)
// - Interrupt flags: Overflow, Compare Match A/B
// - No external clock input (unlike Timer0)
//
// Register Addresses (ATmega328P):
// - TIFR2  = 0x37  (Timer Interrupt Flag Register)
// - TIMSK2 = 0x70  (Timer Interrupt Mask Register)
// - TCCR2A = 0xB0  (Timer Control Register A)
// - TCCR2B = 0xB1  (Timer Control Register B)
// - TCNT2  = 0xB2  (Timer Counter)
// - OCR2A  = 0xB3  (Output Compare Register A)
// - OCR2B  = 0xB4  (Output Compare Register B)

`include "bus/memory_map.vh"

module timer2 (
    input  wire        clk,
    input  wire        rst_n,
    
    // Bus Interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // Output Compare pins
    output wire        oc2a,
    output wire        oc2b,
    
    // Interrupt outputs
    output wire        irq_ovf,   // Overflow interrupt
    output wire        irq_compa, // Compare Match A interrupt
    output wire        irq_compb  // Compare Match B interrupt
);

    // =========================================================================
    // Register Definitions (ATmega328P compatible)
    // =========================================================================
    
    // TCCR2A - Timer/Counter Control Register A (0xB0)
    reg [1:0] com2a;    // Compare Output Mode A
    reg [1:0] com2b;    // Compare Output Mode B
    reg [1:0] wgm_low;  // WGM bits 1:0
    
    // TCCR2B - Timer/Counter Control Register B (0xB1)
    reg       wgm_high; // WGM bit 2
    reg [2:0] cs;       // Clock Select
    
    // TCNT2 - Timer/Counter Register (0xB2)
    reg [7:0] tcnt2;
    
    // OCR2A - Output Compare Register A (0xB3)
    reg [7:0] ocr2a;
    
    // OCR2B - Output Compare Register B (0xB4)
    reg [7:0] ocr2b;
    
    // TIMSK2 - Timer/Counter Interrupt Mask Register (0x70)
    reg       ocie2b;
    reg       ocie2a;
    reg       toie2;
    
    // TIFR2 - Timer/Counter Interrupt Flag Register (0x37)
    reg       ocf2b;
    reg       ocf2a;
    reg       tov2;
    
    // Force output compare (strobe signals)
    reg       foc2a_pulse;
    reg       foc2b_pulse;
    
    // =========================================================================
    // Address Decode - Word-aligned addresses with byte lane selection
    // PicoRV32 sends word-aligned addresses with wstrb indicating byte lane
    // =========================================================================
    wire [31:0] word_addr = {mem_addr[31:2], 2'b00};  // Word-align the address
    
    // Timer2 registers span multiple words:
    // Word 0x20000034: byte 3 = TIFR2 (0x37)
    // Word 0x20000070: byte 0 = TIMSK2 (0x70)
    // Word 0x200000B0: byte 0 = TCCR2A (0xB0), byte 1 = TCCR2B (0xB1), 
    //                  byte 2 = TCNT2 (0xB2), byte 3 = OCR2A (0xB3)
    // Word 0x200000B4: byte 0 = OCR2B (0xB4)
    
    wire sel_word_34   = (word_addr == 32'h20000034);  // Contains TIFR2
    wire sel_word_70   = (word_addr == 32'h20000070);  // Contains TIMSK2
    wire sel_word_b0   = (word_addr == 32'h200000B0);  // Contains TCCR2A, TCCR2B, TCNT2, OCR2A
    wire sel_word_b4   = (word_addr == 32'h200000B4);  // Contains OCR2B
    
    // Write selection - based on which byte lane has wstrb set
    wire sel_tifr2_wr  = sel_word_34 && mem_wstrb[3];   // Byte 3 of word 0x34
    wire sel_timsk2_wr = sel_word_70 && mem_wstrb[0];   // Byte 0 of word 0x70
    wire sel_tccr2a_wr = sel_word_b0 && mem_wstrb[0];   // Byte 0 of word 0xB0
    wire sel_tccr2b_wr = sel_word_b0 && mem_wstrb[1];   // Byte 1 of word 0xB0
    wire sel_tcnt2_wr  = sel_word_b0 && mem_wstrb[2];   // Byte 2 of word 0xB0
    wire sel_ocr2a_wr  = sel_word_b0 && mem_wstrb[3];   // Byte 3 of word 0xB0
    wire sel_ocr2b_wr  = sel_word_b4 && mem_wstrb[0];   // Byte 0 of word 0xB4
    
    wire timer2_sel = sel_word_34 | sel_word_70 | sel_word_b0 | sel_word_b4;

    // =========================================================================
    // Write Data Extraction - get byte from correct lane
    // =========================================================================
    wire [7:0] write_byte_0 = mem_wdata[7:0];
    wire [7:0] write_byte_1 = mem_wdata[15:8];
    wire [7:0] write_byte_2 = mem_wdata[23:16];
    wire [7:0] write_byte_3 = mem_wdata[31:24];

    // =========================================================================
    // Waveform Generation Mode decoding
    // =========================================================================
    wire [2:0] wgm = {wgm_high, wgm_low};
    
    wire mode_normal         = (wgm == 3'b000);
    wire mode_pwm_pc_ff      = (wgm == 3'b001);
    wire mode_ctc            = (wgm == 3'b010);
    wire mode_fast_pwm_ff    = (wgm == 3'b011);
    wire mode_pwm_pc_ocra    = (wgm == 3'b101);
    wire mode_fast_pwm_ocra  = (wgm == 3'b111);
    
    // TOP value based on mode
    wire [7:0] top_value = (mode_ctc || mode_pwm_pc_ocra || mode_fast_pwm_ocra) ? ocr2a : 8'hFF;
    
    // =========================================================================
    // Prescaler and Clock Generation
    // Timer2 has different prescaler values than Timer0:
    // CS=000: Stopped
    // CS=001: clk/1
    // CS=010: clk/8
    // CS=011: clk/32
    // CS=100: clk/64
    // CS=101: clk/128
    // CS=110: clk/256
    // CS=111: clk/1024
    // =========================================================================
    
    reg [9:0] prescaler_cnt;
    
    // Prescaler taps for Timer2's specific dividers
    wire prescale_1    = 1'b1;
    wire prescale_8    = (prescaler_cnt[2:0] == 3'b111);
    wire prescale_32   = (prescaler_cnt[4:0] == 5'b11111);
    wire prescale_64   = (prescaler_cnt[5:0] == 6'b111111);
    wire prescale_128  = (prescaler_cnt[6:0] == 7'b1111111);
    wire prescale_256  = (prescaler_cnt[7:0] == 8'hFF);
    wire prescale_1024 = (prescaler_cnt[9:0] == 10'h3FF);
    
    // Timer clock enable - Timer2 specific prescaler selection
    reg timer_clk_en;
    
    always @(*) begin
        case (cs)
            3'b000: timer_clk_en = 1'b0;        // Stopped
            3'b001: timer_clk_en = prescale_1;   // clk/1
            3'b010: timer_clk_en = prescale_8;   // clk/8
            3'b011: timer_clk_en = prescale_32;  // clk/32
            3'b100: timer_clk_en = prescale_64;  // clk/64
            3'b101: timer_clk_en = prescale_128; // clk/128
            3'b110: timer_clk_en = prescale_256; // clk/256
            3'b111: timer_clk_en = prescale_1024;// clk/1024
            default: timer_clk_en = 1'b0;
        endcase
    end
    
    // Prescaler counter
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            prescaler_cnt <= 10'b0;
        end else if (cs != 3'b000) begin
            prescaler_cnt <= prescaler_cnt + 1'b1;
        end
    end
    
    // =========================================================================
    // Counter Logic
    // =========================================================================
    
    reg count_up;
    
    wire match_a = (tcnt2 == ocr2a);
    wire match_b = (tcnt2 == ocr2b);
    wire at_top  = (tcnt2 == top_value);
    wire at_bottom = (tcnt2 == 8'h00);
    wire at_max  = (tcnt2 == 8'hFF);
    
    reg [7:0] tcnt2_next;
    reg       overflow_event;
    reg       match_a_event;
    reg       match_b_event;
    
    always @(*) begin
        tcnt2_next = tcnt2;
        overflow_event = 1'b0;
        match_a_event = 1'b0;
        match_b_event = 1'b0;
        
        if (timer_clk_en) begin
            match_a_event = match_a;
            match_b_event = match_b;
            
            if (mode_normal) begin
                if (at_max) begin
                    tcnt2_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt2_next = tcnt2 + 1'b1;
                end
            end
            else if (mode_ctc) begin
                if (at_top) begin
                    tcnt2_next = 8'h00;
                    // In CTC mode, overflow only occurs when TOP = MAX (0xFF)
                    if (top_value == 8'hFF) overflow_event = 1'b1;
                end else begin
                    tcnt2_next = tcnt2 + 1'b1;
                end
            end
            else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                if (at_top) begin
                    tcnt2_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt2_next = tcnt2 + 1'b1;
                end
            end
            else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                if (count_up) begin
                    if (at_top) begin
                        tcnt2_next = tcnt2 - 1'b1;
                    end else begin
                        tcnt2_next = tcnt2 + 1'b1;
                    end
                end else begin
                    if (at_bottom) begin
                        tcnt2_next = tcnt2 + 1'b1;
                        overflow_event = 1'b1;
                    end else begin
                        tcnt2_next = tcnt2 - 1'b1;
                    end
                end
            end
            else begin
                if (at_max) begin
                    tcnt2_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt2_next = tcnt2 + 1'b1;
                end
            end
        end
    end
    
    // =========================================================================
    // Output Compare Unit
    // =========================================================================
    
    reg oc2a_reg, oc2b_reg;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            oc2a_reg <= 1'b0;
        end else if (foc2a_pulse && (mode_normal || mode_ctc)) begin
            case (com2a)
                2'b01: oc2a_reg <= ~oc2a_reg;
                2'b10: oc2a_reg <= 1'b0;
                2'b11: oc2a_reg <= 1'b1;
                default: ;
            endcase
        end else if (timer_clk_en && match_a_event) begin
            if (mode_normal || mode_ctc) begin
                case (com2a)
                    2'b01: oc2a_reg <= ~oc2a_reg;
                    2'b10: oc2a_reg <= 1'b0;
                    2'b11: oc2a_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                case (com2a)
                    2'b10: oc2a_reg <= 1'b0;
                    2'b11: oc2a_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                case (com2a)
                    2'b10: oc2a_reg <= count_up ? 1'b0 : 1'b1;
                    2'b11: oc2a_reg <= count_up ? 1'b1 : 1'b0;
                    default: ;
                endcase
            end
        end else if (timer_clk_en && at_bottom && (mode_fast_pwm_ff || mode_fast_pwm_ocra)) begin
            case (com2a)
                2'b10: oc2a_reg <= 1'b1;
                2'b11: oc2a_reg <= 1'b0;
                default: ;
            endcase
        end
    end
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            oc2b_reg <= 1'b0;
        end else if (foc2b_pulse && (mode_normal || mode_ctc)) begin
            case (com2b)
                2'b01: oc2b_reg <= ~oc2b_reg;
                2'b10: oc2b_reg <= 1'b0;
                2'b11: oc2b_reg <= 1'b1;
                default: ;
            endcase
        end else if (timer_clk_en && match_b_event) begin
            if (mode_normal || mode_ctc) begin
                case (com2b)
                    2'b01: oc2b_reg <= ~oc2b_reg;
                    2'b10: oc2b_reg <= 1'b0;
                    2'b11: oc2b_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                case (com2b)
                    2'b10: oc2b_reg <= 1'b0;
                    2'b11: oc2b_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                case (com2b)
                    2'b10: oc2b_reg <= count_up ? 1'b0 : 1'b1;
                    2'b11: oc2b_reg <= count_up ? 1'b1 : 1'b0;
                    default: ;
                endcase
            end
        end else if (timer_clk_en && at_bottom && (mode_fast_pwm_ff || mode_fast_pwm_ocra)) begin
            case (com2b)
                2'b10: oc2b_reg <= 1'b1;
                2'b11: oc2b_reg <= 1'b0;
                default: ;
            endcase
        end
    end
    
    assign oc2a = oc2a_reg;
    assign oc2b = oc2b_reg;
    
    // =========================================================================
    // Bus Interface - Register Writes (Active on bus transaction)
    // =========================================================================
    
    wire bus_write = mem_valid && timer2_sel && |mem_wstrb;
    wire bus_read  = mem_valid && timer2_sel && ~|mem_wstrb;
    
    // Control Register Writes
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            com2a    <= 2'b00;
            com2b    <= 2'b00;
            wgm_low  <= 2'b00;
            wgm_high <= 1'b0;
            cs       <= 3'b000;
            ocr2a    <= 8'h00;
            ocr2b    <= 8'h00;
            ocie2b   <= 1'b0;
            ocie2a   <= 1'b0;
            toie2    <= 1'b0;
            foc2a_pulse <= 1'b0;
            foc2b_pulse <= 1'b0;
        end else begin
            // Default: clear strobe signals
            foc2a_pulse <= 1'b0;
            foc2b_pulse <= 1'b0;
            
            // Register writes (each register uses its specific byte lane)
            if (bus_write) begin
                // TCCR2A is byte 0 of word 0xB0
                if (sel_tccr2a_wr) begin
                    com2a   <= write_byte_0[7:6];
                    com2b   <= write_byte_0[5:4];
                    wgm_low <= write_byte_0[1:0];
                end
                // TCCR2B is byte 1 of word 0xB0
                if (sel_tccr2b_wr) begin
                    foc2a_pulse <= write_byte_1[7];
                    foc2b_pulse <= write_byte_1[6];
                    wgm_high    <= write_byte_1[3];
                    cs          <= write_byte_1[2:0];
                end
                // OCR2A is byte 3 of word 0xB0
                if (sel_ocr2a_wr) begin
                    ocr2a <= write_byte_3;
                end
                // OCR2B is byte 0 of word 0xB4
                if (sel_ocr2b_wr) begin
                    ocr2b <= write_byte_0;
                end
                // TIMSK2 is byte 0 of word 0x70
                if (sel_timsk2_wr) begin
                    ocie2b <= write_byte_0[2];
                    ocie2a <= write_byte_0[1];
                    toie2  <= write_byte_0[0];
                end
            end
        end
    end
    
    // =========================================================================
    // Counter and Flag Updates
    // =========================================================================
    
    // TCNT2 is byte 2 of word 0xB0, TIFR2 is byte 3 of word 0x34
    wire tcnt2_write = bus_write && sel_tcnt2_wr;
    wire tifr2_write = bus_write && sel_tifr2_wr;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            tcnt2    <= 8'h00;
            count_up <= 1'b1;
            tov2     <= 1'b0;
            ocf2a    <= 1'b0;
            ocf2b    <= 1'b0;
        end else begin
            // Counter update (TCNT2 is byte 2)
            if (tcnt2_write) begin
                tcnt2 <= write_byte_2;
            end else if (timer_clk_en) begin
                tcnt2 <= tcnt2_next;
            end
            
            // Count direction for phase correct PWM
            if (timer_clk_en) begin
                if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                    if (at_top) count_up <= 1'b0;
                    else if (at_bottom) count_up <= 1'b1;
                end else begin
                    count_up <= 1'b1;
                end
            end
            
            // Overflow flag (write-1-to-clear, TIFR2 is byte 3)
            if (tifr2_write && write_byte_3[0]) begin
                tov2 <= 1'b0;
            end else if (timer_clk_en && overflow_event) begin
                tov2 <= 1'b1;
            end
            
            // Compare Match A flag
            if (tifr2_write && write_byte_3[1]) begin
                ocf2a <= 1'b0;
            end else if (timer_clk_en && match_a_event) begin
                ocf2a <= 1'b1;
            end
            
            // Compare Match B flag
            if (tifr2_write && write_byte_3[2]) begin
                ocf2b <= 1'b0;
            end else if (timer_clk_en && match_b_event) begin
                ocf2b <= 1'b1;
            end
        end
    end
    
    // =========================================================================
    // Interrupt Outputs
    // =========================================================================
    
    assign irq_ovf   = toie2  && tov2;
    assign irq_compa = ocie2a && ocf2a;
    assign irq_compb = ocie2b && ocf2b;
    
    // =========================================================================
    // Bus Interface - Ready and Read Data
    // =========================================================================
    
    // Read data bytes for each register
    wire [7:0] tccr2a_byte = {com2a, com2b, 2'b00, wgm_low};
    wire [7:0] tccr2b_byte = {2'b00, 1'b0, wgm_high, cs};
    wire [7:0] tcnt2_byte  = tcnt2;
    wire [7:0] ocr2a_byte  = ocr2a;
    wire [7:0] ocr2b_byte  = ocr2b;
    wire [7:0] timsk2_byte = {5'b00000, ocie2b, ocie2a, toie2};
    wire [7:0] tifr2_byte  = {5'b00000, ocf2b, ocf2a, tov2};
    
    // Pack read data based on word address - return full word with all byte lanes
    reg [31:0] read_word;
    always @(*) begin
        read_word = 32'h0;
        if (sel_word_34) begin
            // Word 0x20000034: byte 3 = TIFR2
            read_word = {tifr2_byte, 24'h0};
        end else if (sel_word_70) begin
            // Word 0x20000070: byte 0 = TIMSK2
            read_word = {24'h0, timsk2_byte};
        end else if (sel_word_b0) begin
            // Word 0x200000B0: byte 0 = TCCR2A, byte 1 = TCCR2B, byte 2 = TCNT2, byte 3 = OCR2A
            read_word = {ocr2a_byte, tcnt2_byte, tccr2b_byte, tccr2a_byte};
        end else if (sel_word_b4) begin
            // Word 0x200000B4: byte 0 = OCR2B
            read_word = {24'h0, ocr2b_byte};
        end
    end
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            mem_ready <= 1'b0;
            mem_rdata <= 32'h0;
        end else begin
            mem_ready <= 1'b0;
            
            if (mem_valid && timer2_sel && !mem_ready) begin
                mem_ready <= 1'b1;
                mem_rdata <= read_word;
            end
        end
    end

endmodule
