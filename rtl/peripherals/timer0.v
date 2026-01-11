// Timer/Counter 0 - 8-bit Timer with PWM
// ATmega328P-compatible implementation
// 
// Features:
// - 8-bit counter (TCNT0)
// - Two output compare registers (OCR0A, OCR0B)
// - Prescaler: 1, 8, 64, 256, 1024, external clock
// - Waveform Generation Modes: Normal, CTC, Fast PWM, Phase Correct PWM
// - Compare Match outputs (directly controllable)
// - Interrupt flags: Overflow, Compare Match A/B

`include "bus/memory_map.vh"

module timer0 (
    input  wire        clk,
    input  wire        rst_n,
    
    // Bus Interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // External clock input (T0 pin)
    input  wire        t0_pin,
    
    // Output Compare pins
    output wire        oc0a,
    output wire        oc0b,
    
    // Interrupt outputs
    output wire        irq_ovf,   // Overflow interrupt
    output wire        irq_compa, // Compare Match A interrupt
    output wire        irq_compb  // Compare Match B interrupt
);

    // =========================================================================
    // Register Definitions (ATmega328P compatible)
    // =========================================================================
    
    // TCCR0A - Timer/Counter Control Register A (0x44)
    reg [1:0] com0a;    // Compare Output Mode A
    reg [1:0] com0b;    // Compare Output Mode B
    reg [1:0] wgm_low;  // WGM bits 1:0
    
    // TCCR0B - Timer/Counter Control Register B (0x45)
    reg       wgm_high; // WGM bit 2
    reg [2:0] cs;       // Clock Select
    
    // TCNT0 - Timer/Counter Register (0x46)
    reg [7:0] tcnt0;
    
    // OCR0A - Output Compare Register A (0x47)
    reg [7:0] ocr0a;
    
    // OCR0B - Output Compare Register B (0x48)
    reg [7:0] ocr0b;
    
    // TIMSK0 - Timer/Counter Interrupt Mask Register (0x6E)
    reg       ocie0b;
    reg       ocie0a;
    reg       toie0;
    
    // TIFR0 - Timer/Counter Interrupt Flag Register (0x35)
    reg       ocf0b;
    reg       ocf0a;
    reg       tov0;
    
    // Force output compare (strobe signals)
    reg       foc0a_pulse;
    reg       foc0b_pulse;
    
    // =========================================================================
    // Address Decode
    // =========================================================================
    wire sel_tifr0  = (mem_addr[7:0] == 8'h35);
    wire sel_tccr0a = (mem_addr[7:0] == 8'h44);
    wire sel_tccr0b = (mem_addr[7:0] == 8'h45);
    wire sel_tcnt0  = (mem_addr[7:0] == 8'h46);
    wire sel_ocr0a  = (mem_addr[7:0] == 8'h47);
    wire sel_ocr0b  = (mem_addr[7:0] == 8'h48);
    wire sel_timsk0 = (mem_addr[7:0] == 8'h6E);
    
    wire timer0_sel = sel_tifr0 | sel_tccr0a | sel_tccr0b | 
                      sel_tcnt0 | sel_ocr0a | sel_ocr0b | sel_timsk0;

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
    wire [7:0] top_value = (mode_ctc || mode_pwm_pc_ocra || mode_fast_pwm_ocra) ? ocr0a : 8'hFF;
    
    // =========================================================================
    // Prescaler and Clock Generation
    // =========================================================================
    
    reg [9:0] prescaler_cnt;
    
    // Prescaler taps
    wire prescale_1    = 1'b1;
    wire prescale_8    = (prescaler_cnt[2:0] == 3'b111);
    wire prescale_64   = (prescaler_cnt[5:0] == 6'b111111);
    wire prescale_256  = (prescaler_cnt[7:0] == 8'hFF);
    wire prescale_1024 = (prescaler_cnt[9:0] == 10'h3FF);
    
    // External clock edge detection
    reg t0_sync1, t0_sync2, t0_prev;
    wire t0_falling = t0_prev && !t0_sync2;
    wire t0_rising  = !t0_prev && t0_sync2;
    
    // Timer clock enable
    reg timer_clk_en;
    
    always @(*) begin
        case (cs)
            3'b000: timer_clk_en = 1'b0;
            3'b001: timer_clk_en = prescale_1;
            3'b010: timer_clk_en = prescale_8;
            3'b011: timer_clk_en = prescale_64;
            3'b100: timer_clk_en = prescale_256;
            3'b101: timer_clk_en = prescale_1024;
            3'b110: timer_clk_en = t0_falling;
            3'b111: timer_clk_en = t0_rising;
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
    
    // T0 synchronizer
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            t0_sync1 <= 1'b0;
            t0_sync2 <= 1'b0;
            t0_prev  <= 1'b0;
        end else begin
            t0_sync1 <= t0_pin;
            t0_sync2 <= t0_sync1;
            t0_prev  <= t0_sync2;
        end
    end
    
    // =========================================================================
    // Counter Logic
    // =========================================================================
    
    reg count_up;
    
    wire match_a = (tcnt0 == ocr0a);
    wire match_b = (tcnt0 == ocr0b);
    wire at_top  = (tcnt0 == top_value);
    wire at_bottom = (tcnt0 == 8'h00);
    wire at_max  = (tcnt0 == 8'hFF);
    
    reg [7:0] tcnt0_next;
    reg       overflow_event;
    reg       match_a_event;
    reg       match_b_event;
    
    always @(*) begin
        tcnt0_next = tcnt0;
        overflow_event = 1'b0;
        match_a_event = 1'b0;
        match_b_event = 1'b0;
        
        if (timer_clk_en) begin
            match_a_event = match_a;
            match_b_event = match_b;
            
            if (mode_normal) begin
                if (at_max) begin
                    tcnt0_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt0_next = tcnt0 + 1'b1;
                end
            end
            else if (mode_ctc) begin
                if (at_top) begin
                    tcnt0_next = 8'h00;
                    // In CTC mode, overflow only occurs when TOP = MAX (0xFF)
                    // per ATmega328P datasheet behavior
                    if (top_value == 8'hFF) overflow_event = 1'b1;
                end else begin
                    tcnt0_next = tcnt0 + 1'b1;
                end
            end
            else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                if (at_top) begin
                    tcnt0_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt0_next = tcnt0 + 1'b1;
                end
            end
            else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                if (count_up) begin
                    if (at_top) begin
                        tcnt0_next = tcnt0 - 1'b1;
                    end else begin
                        tcnt0_next = tcnt0 + 1'b1;
                    end
                end else begin
                    if (at_bottom) begin
                        tcnt0_next = tcnt0 + 1'b1;
                        overflow_event = 1'b1;
                    end else begin
                        tcnt0_next = tcnt0 - 1'b1;
                    end
                end
            end
            else begin
                if (at_max) begin
                    tcnt0_next = 8'h00;
                    overflow_event = 1'b1;
                end else begin
                    tcnt0_next = tcnt0 + 1'b1;
                end
            end
        end
    end
    
    // =========================================================================
    // Output Compare Unit
    // =========================================================================
    
    reg oc0a_reg, oc0b_reg;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            oc0a_reg <= 1'b0;
        end else if (foc0a_pulse && (mode_normal || mode_ctc)) begin
            case (com0a)
                2'b01: oc0a_reg <= ~oc0a_reg;
                2'b10: oc0a_reg <= 1'b0;
                2'b11: oc0a_reg <= 1'b1;
                default: ;
            endcase
        end else if (timer_clk_en && match_a_event) begin
            if (mode_normal || mode_ctc) begin
                case (com0a)
                    2'b01: oc0a_reg <= ~oc0a_reg;
                    2'b10: oc0a_reg <= 1'b0;
                    2'b11: oc0a_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                case (com0a)
                    2'b10: oc0a_reg <= 1'b0;
                    2'b11: oc0a_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                case (com0a)
                    2'b10: oc0a_reg <= count_up ? 1'b0 : 1'b1;
                    2'b11: oc0a_reg <= count_up ? 1'b1 : 1'b0;
                    default: ;
                endcase
            end
        end else if (timer_clk_en && at_bottom && (mode_fast_pwm_ff || mode_fast_pwm_ocra)) begin
            case (com0a)
                2'b10: oc0a_reg <= 1'b1;
                2'b11: oc0a_reg <= 1'b0;
                default: ;
            endcase
        end
    end
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            oc0b_reg <= 1'b0;
        end else if (foc0b_pulse && (mode_normal || mode_ctc)) begin
            case (com0b)
                2'b01: oc0b_reg <= ~oc0b_reg;
                2'b10: oc0b_reg <= 1'b0;
                2'b11: oc0b_reg <= 1'b1;
                default: ;
            endcase
        end else if (timer_clk_en && match_b_event) begin
            if (mode_normal || mode_ctc) begin
                case (com0b)
                    2'b01: oc0b_reg <= ~oc0b_reg;
                    2'b10: oc0b_reg <= 1'b0;
                    2'b11: oc0b_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_fast_pwm_ff || mode_fast_pwm_ocra) begin
                case (com0b)
                    2'b10: oc0b_reg <= 1'b0;
                    2'b11: oc0b_reg <= 1'b1;
                    default: ;
                endcase
            end else if (mode_pwm_pc_ff || mode_pwm_pc_ocra) begin
                case (com0b)
                    2'b10: oc0b_reg <= count_up ? 1'b0 : 1'b1;
                    2'b11: oc0b_reg <= count_up ? 1'b1 : 1'b0;
                    default: ;
                endcase
            end
        end else if (timer_clk_en && at_bottom && (mode_fast_pwm_ff || mode_fast_pwm_ocra)) begin
            case (com0b)
                2'b10: oc0b_reg <= 1'b1;
                2'b11: oc0b_reg <= 1'b0;
                default: ;
            endcase
        end
    end
    
    assign oc0a = oc0a_reg;
    assign oc0b = oc0b_reg;
    
    // =========================================================================
    // Bus Interface - Register Writes (Active on bus transaction)
    // =========================================================================
    
    wire bus_write = mem_valid && timer0_sel && |mem_wstrb;
    wire bus_read  = mem_valid && timer0_sel && ~|mem_wstrb;
    
    // Control Register Writes
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            com0a    <= 2'b00;
            com0b    <= 2'b00;
            wgm_low  <= 2'b00;
            wgm_high <= 1'b0;
            cs       <= 3'b000;
            ocr0a    <= 8'h00;
            ocr0b    <= 8'h00;
            ocie0b   <= 1'b0;
            ocie0a   <= 1'b0;
            toie0    <= 1'b0;
            foc0a_pulse <= 1'b0;
            foc0b_pulse <= 1'b0;
        end else begin
            // Default: clear strobe signals
            foc0a_pulse <= 1'b0;
            foc0b_pulse <= 1'b0;
            
            // Register writes
            if (bus_write) begin
                if (sel_tccr0a && mem_wstrb[0]) begin
                    com0a   <= mem_wdata[7:6];
                    com0b   <= mem_wdata[5:4];
                    wgm_low <= mem_wdata[1:0];
                end
                if (sel_tccr0b && mem_wstrb[0]) begin
                    foc0a_pulse <= mem_wdata[7];
                    foc0b_pulse <= mem_wdata[6];
                    wgm_high    <= mem_wdata[3];
                    cs          <= mem_wdata[2:0];
                end
                if (sel_ocr0a && mem_wstrb[0]) begin
                    ocr0a <= mem_wdata[7:0];
                end
                if (sel_ocr0b && mem_wstrb[0]) begin
                    ocr0b <= mem_wdata[7:0];
                end
                if (sel_timsk0 && mem_wstrb[0]) begin
                    ocie0b <= mem_wdata[2];
                    ocie0a <= mem_wdata[1];
                    toie0  <= mem_wdata[0];
                end
            end
        end
    end
    
    // =========================================================================
    // Counter and Flag Updates
    // =========================================================================
    
    wire tcnt0_write = bus_write && sel_tcnt0 && mem_wstrb[0];
    wire tifr0_write = bus_write && sel_tifr0 && mem_wstrb[0];
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            tcnt0    <= 8'h00;
            count_up <= 1'b1;
            tov0     <= 1'b0;
            ocf0a    <= 1'b0;
            ocf0b    <= 1'b0;
        end else begin
            // Counter update
            if (tcnt0_write) begin
                tcnt0 <= mem_wdata[7:0];
            end else if (timer_clk_en) begin
                tcnt0 <= tcnt0_next;
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
            
            // Overflow flag (write-1-to-clear)
            if (tifr0_write && mem_wdata[0]) begin
                tov0 <= 1'b0;
            end else if (timer_clk_en && overflow_event) begin
                tov0 <= 1'b1;
            end
            
            // Compare Match A flag
            if (tifr0_write && mem_wdata[1]) begin
                ocf0a <= 1'b0;
            end else if (timer_clk_en && match_a_event) begin
                ocf0a <= 1'b1;
            end
            
            // Compare Match B flag
            if (tifr0_write && mem_wdata[2]) begin
                ocf0b <= 1'b0;
            end else if (timer_clk_en && match_b_event) begin
                ocf0b <= 1'b1;
            end
        end
    end
    
    // =========================================================================
    // Interrupt Outputs
    // =========================================================================
    
    assign irq_ovf   = toie0  && tov0;
    assign irq_compa = ocie0a && ocf0a;
    assign irq_compb = ocie0b && ocf0b;
    
    // =========================================================================
    // Bus Interface - Ready and Read Data
    // =========================================================================
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            mem_ready <= 1'b0;
            mem_rdata <= 32'h0;
        end else begin
            mem_ready <= 1'b0;
            
            if (mem_valid && timer0_sel && !mem_ready) begin
                mem_ready <= 1'b1;
                
                // Read data mux
                case (1'b1)
                    sel_tccr0a: mem_rdata <= {24'h0, com0a, com0b, 2'b00, wgm_low};
                    sel_tccr0b: mem_rdata <= {24'h0, 2'b00, 1'b0, wgm_high, cs};
                    sel_tcnt0:  mem_rdata <= {24'h0, tcnt0};
                    sel_ocr0a:  mem_rdata <= {24'h0, ocr0a};
                    sel_ocr0b:  mem_rdata <= {24'h0, ocr0b};
                    sel_timsk0: mem_rdata <= {24'h0, 5'b00000, ocie0b, ocie0a, toie0};
                    sel_tifr0:  mem_rdata <= {24'h0, 5'b00000, ocf0b, ocf0a, tov0};
                    default:    mem_rdata <= 32'h0;
                endcase
            end
        end
    end

endmodule
