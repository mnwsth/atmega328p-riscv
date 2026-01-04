// GPIO Peripheral - Port B
// Implements ATmega328P-style GPIO registers:
// - PINB: Input pin register (read-only, reflects actual pin state)
// - DDRB: Data direction register (1=output, 0=input)
// - PORTB: Output data register

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
    
    // GPIO pins
    input  wire [7:0]  gpio_pin_in,   // Actual pin input
    output reg  [7:0]  gpio_pin_out,  // Pin output value
    output reg  [7:0]  gpio_pin_dir   // Pin direction (1=out, 0=in)
);

    // Internal registers
    reg [7:0] ddrb;   // Data Direction Register B
    reg [7:0] portb;  // Port B Data Register
    
    // Address decoding
    wire sel_pinb  = (mem_addr == `GPIOB_PINB);
    wire sel_ddrb  = (mem_addr == `GPIOB_DDRB);
    wire sel_portb = (mem_addr == `GPIOB_PORTB);
    wire gpio_sel  = sel_pinb | sel_ddrb | sel_portb;
    
    // Read logic
    always @(*) begin
        mem_rdata = 32'h00000000;
        if (gpio_sel) begin
            case (mem_addr)
                `GPIOB_PINB:  mem_rdata = {24'h000000, gpio_pin_in};   // Read actual pin state
                `GPIOB_DDRB:  mem_rdata = {24'h000000, ddrb};
                `GPIOB_PORTB: mem_rdata = {24'h000000, portb};
                default:      mem_rdata = 32'h00000000;
            endcase
        end
    end
    
    // Write logic
    always @(posedge clk) begin
        if (!rst_n) begin
            ddrb  <= 8'h00;
            portb <= 8'h00;
        end else if (mem_valid && gpio_sel && |mem_wstrb) begin
            case (mem_addr)
                `GPIOB_DDRB: begin
                    if (mem_wstrb[0]) ddrb <= mem_wdata[7:0];
                end
                `GPIOB_PORTB: begin
                    if (mem_wstrb[0]) portb <= mem_wdata[7:0];
                end
                // PINB is read-only, ignore writes
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
    
    // Drive output pins based on DDRB and PORTB
    always @(*) begin
        gpio_pin_dir = ddrb;
        gpio_pin_out = portb;
    end

endmodule

