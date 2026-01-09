// GPIO Peripheral - Port B and Port D
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
    
    // Address decoding - Port B
    wire sel_pinb  = (mem_addr == `GPIOB_PINB);
    wire sel_ddrb  = (mem_addr == `GPIOB_DDRB);
    wire sel_portb = (mem_addr == `GPIOB_PORTB);
    
    // Address decoding - Port D
    wire sel_pind  = (mem_addr == `GPIOD_PIND);
    wire sel_ddrd  = (mem_addr == `GPIOD_DDRD);
    wire sel_portd = (mem_addr == `GPIOD_PORTD);

    // Address decoding - Port C
    wire sel_pinc  = (mem_addr == `GPIOC_PINC);
    wire sel_ddrc  = (mem_addr == `GPIOC_DDRC);
    wire sel_portc = (mem_addr == `GPIOC_PORTC);
    
    wire gpio_sel  = sel_pinb | sel_ddrb | sel_portb | sel_pind | sel_ddrd | sel_portd | sel_pinc | sel_ddrc | sel_portc;
    
    // Read logic
    always @(*) begin
        mem_rdata = 32'h00000000;
        if (gpio_sel) begin
            case (mem_addr)
                `GPIOB_PINB:  mem_rdata = {24'h000000, gpio_pin_in_b};   // Read actual pin state
                `GPIOB_DDRB:  mem_rdata = {24'h000000, ddrb};
                `GPIOB_PORTB: mem_rdata = {24'h000000, portb};
                `GPIOD_PIND:  mem_rdata = {24'h000000, gpio_pin_in_d};   // Read actual pin state
                `GPIOD_DDRD:  mem_rdata = {24'h000000, ddrd};
                `GPIOD_PORTD: mem_rdata = {24'h000000, portd};
                `GPIOC_PINC:  mem_rdata = {24'h000000, gpio_pin_in_c};   // Read actual pin state
                `GPIOC_DDRC:  mem_rdata = {24'h000000, ddrc};
                `GPIOC_PORTC: mem_rdata = {24'h000000, portc};
                default:      mem_rdata = 32'h00000000;
            endcase
        end
    end
    
    // Write logic
    always @(posedge clk) begin
        if (!rst_n) begin
            ddrb  <= 8'h00;
            portb <= 8'h00;
            ddrd  <= 8'h00;
            portd <= 8'h00;
            ddrc  <= 8'h00;
            portc <= 8'h00;
        end else if (mem_valid && gpio_sel && |mem_wstrb) begin
            case (mem_addr)
                `GPIOB_DDRB: begin
                    if (mem_wstrb[0]) ddrb <= mem_wdata[7:0];
                end
                `GPIOB_PORTB: begin
                    if (mem_wstrb[0]) portb <= mem_wdata[7:0];
                end
                `GPIOD_DDRD: begin
                    if (mem_wstrb[0]) ddrd <= mem_wdata[7:0];
                end
                `GPIOD_PORTD: begin
                    if (mem_wstrb[0]) portd <= mem_wdata[7:0];
                end
                `GPIOC_DDRC: begin
                    if (mem_wstrb[0]) ddrc <= mem_wdata[7:0];
                end
                `GPIOC_PORTC: begin
                    if (mem_wstrb[0]) portc <= mem_wdata[7:0];
                end
                // PINB, PIND, PINC are read-only, ignore writes
            endcase
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

