// GPIO Peripheral - Port B, Port C and Port D
// Implements ATmega328P-style GPIO registers:
// Port B: PINB, DDRB, PORTB
// Port C: PINC, DDRC, PORTC
// Port D: PIND, DDRD, PORTD

`include "bus/memory_map.vh"

module gpio (
    input  wire        clk,
    input  wire        rst_n,
    
    // Memory bus interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // GPIO Port B pins
    input  wire [7:0]  gpio_pin_in_b,   // Port B actual pin input
    output reg  [7:0]  gpio_pin_out_b,  // Port B pin output value
    output reg  [7:0]  gpio_pin_dir_b,  // Port B pin direction (1=out, 0=in)
    
    // GPIO Port D pins
    input  wire [7:0]  gpio_pin_in_d,   // Port D actual pin input
    output reg  [7:0]  gpio_pin_out_d,  // Port D pin output value
    output reg  [7:0]  gpio_pin_dir_d,  // Port D pin direction (1=out, 0=in)

    // GPIO Port C pins
    input  wire [7:0]  gpio_pin_in_c,   // Port C actual pin input
    output reg  [7:0]  gpio_pin_out_c,  // Port C pin output value
    output reg  [7:0]  gpio_pin_dir_c   // Port C pin direction (1=out, 0=in)
);

    // Internal registers - Port B
    reg [7:0] ddrb;   // Data Direction Register B
    reg [7:0] portb;  // Port B Data Register
    
    // Internal registers - Port D
    reg [7:0] ddrd;   // Data Direction Register D
    reg [7:0] portd;  // Port D Data Register

    // Internal registers - Port C
    reg [7:0] ddrc;   // Data Direction Register C
    reg [7:0] portc;  // Port C Data Register
    
    // Word-aligned address (CPU sends word-aligned addresses)
    wire [31:0] word_addr = {mem_addr[31:2], 2'b00};
    
    // Port B registers are at 0x20000023, 0x20000024, 0x20000025
    // Word containing PINB (0x23): word addr 0x20000020, byte 3
    // Word containing DDRB (0x24), PORTB (0x25): word addr 0x20000024, bytes 0 and 1
    wire sel_portb_word = (word_addr == 32'h20000024);  // Contains DDRB (byte 0) and PORTB (byte 1)
    wire sel_pinb_word  = (word_addr == 32'h20000020);  // Contains PINB (byte 3)
    
    // Port C registers are at 0x20000026, 0x20000027, 0x20000028
    // Word containing PINC (0x26), DDRC (0x27): word addr 0x20000024, bytes 2 and 3
    // Word containing PORTC (0x28): word addr 0x20000028, byte 0
    wire sel_portc_word = (word_addr == 32'h20000028);  // Contains PORTC (byte 0)
    // PINC and DDRC are in same word as DDRB/PORTB (0x20000024)
    
    // Port D registers are at 0x20000029, 0x2000002A, 0x2000002B
    // Word containing PIND (0x29), DDRD (0x2A), PORTD (0x2B): word addr 0x20000028, bytes 1, 2, 3
    wire sel_portd_word = (word_addr == 32'h20000028);  // Contains PIND (byte 1), DDRD (byte 2), PORTD (byte 3)
    
    wire gpio_sel = sel_portb_word | sel_pinb_word | sel_portc_word | sel_portd_word;
    
    // Read logic - pack registers into word based on address
    always @(*) begin
        mem_rdata = 32'h00000000;
        if (sel_pinb_word) begin
            // 0x20000020: byte 3 = PINB (0x23)
            mem_rdata = {gpio_pin_in_b, 24'h000000};
        end else if (sel_portb_word) begin
            // 0x20000024: byte 0 = DDRB, byte 1 = PORTB, byte 2 = PINC, byte 3 = DDRC
            mem_rdata = {ddrc, gpio_pin_in_c, portb, ddrb};
        end else if (sel_portc_word || sel_portd_word) begin
            // 0x20000028: byte 0 = PORTC, byte 1 = PIND, byte 2 = DDRD, byte 3 = PORTD
            mem_rdata = {portd, ddrd, gpio_pin_in_d, portc};
        end
    end
    
    // Write logic - use wstrb to select correct byte lane
    always @(posedge clk) begin
        if (!rst_n) begin
            ddrb  <= 8'h00;
            portb <= 8'h00;
            ddrd  <= 8'h00;
            portd <= 8'h00;
            ddrc  <= 8'h00;
            portc <= 8'h00;
        end else if (mem_valid && gpio_sel && |mem_wstrb) begin
            // Word at 0x20000024
            if (sel_portb_word) begin
                if (mem_wstrb[0]) begin // DDRB at byte 0
                    ddrb <= mem_wdata[7:0];
                end
                if (mem_wstrb[1]) begin // PORTB at byte 1
                    portb <= mem_wdata[15:8];
                    
                end
                if (mem_wstrb[2]) begin // PINC at byte 2 - read only, ignore
                end
                if (mem_wstrb[3]) begin // DDRC at byte 3
                    ddrc <= mem_wdata[31:24];
                    
                end
            end
            // Word at 0x20000028
            if (sel_portc_word || sel_portd_word) begin
                if (mem_wstrb[0]) begin // PORTC at byte 0
                    portc <= mem_wdata[7:0];
                    
                end
                if (mem_wstrb[1]) begin // PIND at byte 1 - read only, ignore
                end
                if (mem_wstrb[2]) begin // DDRD at byte 2
                    ddrd <= mem_wdata[23:16];
                    
                end
                if (mem_wstrb[3]) begin // PORTD at byte 3
                    portd <= mem_wdata[31:24];
                    
                end
            end
        end
    end
    
    // Ready signal - GPIO responds immediately
    always @(posedge clk) begin
        if (!rst_n) begin
            mem_ready <= 1'b0;
        end else begin
            mem_ready <= mem_valid && gpio_sel;
        end
    end
    
    // Drive output pins based on DDR and PORT registers
    always @(*) begin
        // Port B
        gpio_pin_dir_b = ddrb;
        gpio_pin_out_b = portb;
        
        // Port D
        gpio_pin_dir_d = ddrd;
        gpio_pin_out_d = portd;

        // Port C
        gpio_pin_dir_c = ddrc;
        gpio_pin_out_c = portc;
    end

endmodule

