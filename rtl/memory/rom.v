// Program ROM - Stores the program code
// Implemented using FPGA block RAM

module rom #(
    parameter ADDR_WIDTH = 16,
    parameter DATA_WIDTH = 32,
    parameter INIT_FILE = "firmware.mem"
) (
    input  wire                  clk,
    input  wire                  rst_n,
    input  wire                  ce,           // Chip enable
    input  wire [ADDR_WIDTH-1:0] addr,         // Word address
    output reg  [DATA_WIDTH-1:0] rdata,       // Read data
    output reg                   rdata_valid   // Read data valid
);

    // Memory array - 2^ADDR_WIDTH words of DATA_WIDTH bits
    reg [DATA_WIDTH-1:0] mem [(1<<ADDR_WIDTH)-1:0];
    
    // Initialize memory from file if provided
    integer i;
    initial begin
        // First, initialize all memory with NOPs (0x00000013 = ADDI x0, x0, 0)
        for (i = 0; i < (1<<ADDR_WIDTH); i = i + 1) begin
            mem[i] = 32'h00000013;
        end
        // Then load firmware file, which will overwrite the initial NOPs
        if (INIT_FILE != "") begin
            $readmemh(INIT_FILE, mem, 0, (1<<ADDR_WIDTH)-1);
        end
    end
    
    // Synchronous read
    always @(posedge clk) begin
        if (!rst_n) begin
            rdata <= 32'h00000013; // NOP
            rdata_valid <= 1'b0;
        end else if (ce) begin
            rdata <= mem[addr];
            rdata_valid <= 1'b1;
        end else begin
            rdata_valid <= 1'b0;
        end
    end

endmodule

