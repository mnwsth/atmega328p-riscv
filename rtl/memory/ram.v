// Data RAM - Stores runtime data
// Implemented using FPGA block RAM

module ram #(
    parameter ADDR_WIDTH = 12,
    parameter DATA_WIDTH = 32
) (
    input  wire                  clk,
    input  wire                  rst_n,
    input  wire                  ce,           // Chip enable
    input  wire                  we,        // Write enable
    input  wire [3:0]            wstrb,     // Write strobe (byte enables)
    input  wire [ADDR_WIDTH-1:0] addr,      // Word address
    input  wire [DATA_WIDTH-1:0] wdata,     // Write data
    output reg  [DATA_WIDTH-1:0] rdata,     // Read data
    output reg                   rdata_valid // Read data valid
);

    // Memory array - 2^ADDR_WIDTH words of DATA_WIDTH bits
    reg [DATA_WIDTH-1:0] mem [(1<<ADDR_WIDTH)-1:0];
    
    // Initialize memory to zero
    integer i;
    initial begin
        for (i = 0; i < (1<<ADDR_WIDTH); i = i + 1) begin
            mem[i] = 32'h00000000;
        end
    end
    
    // Synchronous read/write
    always @(posedge clk) begin
        if (!rst_n) begin
            rdata <= 32'h00000000;
            rdata_valid <= 1'b0;
        end else if (ce) begin
            rdata_valid <= 1'b1;
            if (we) begin
                // Write with byte enables
                if (wstrb[0]) mem[addr][7:0]   <= wdata[7:0];
                if (wstrb[1]) mem[addr][15:8]  <= wdata[15:8];
                if (wstrb[2]) mem[addr][23:16] <= wdata[23:16];
                if (wstrb[3]) mem[addr][31:24] <= wdata[31:24];
                // Read-after-write
                rdata <= mem[addr];
            end else begin
                rdata <= mem[addr];
            end
        end else begin
            rdata_valid <= 1'b0;
        end
    end

endmodule

