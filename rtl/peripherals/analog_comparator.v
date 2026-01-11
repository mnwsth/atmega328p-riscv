module analog_comparator (
    input  wire        clk,
    input  wire        rst_n,
    
    // External Inputs
    input  wire        ain0,
    input  wire        ain1,
    
    // Bus Interface
    input  wire        mem_valid,
    input  wire [31:0] mem_addr,
    input  wire [31:0] mem_wdata,
    input  wire [3:0]  mem_wstrb,
    output reg  [31:0] mem_rdata,
    output reg         mem_ready,
    
    // Interrupt
    output reg         irq
);

    // Register Address Offsets (relative to base)
    // ACSR is at 0x50 in I/O space. 
    // In our memory map, I/O base is 0x20000000.
    // So ACSR absolute address is 0x20000050.
    // The bus decoder will likely pass the full address, so we check the lower bits.
    
    // ACSR Register Bits
    reg        acd;   // Bit 7: Analog Comparator Disable
    reg        acbg;  // Bit 6: Analog Comparator Bandgap Select
    reg        aco;   // Bit 5: Analog Comparator Output
    reg        aci;   // Bit 4: Analog Comparator Interrupt Flag
    reg        acie;  // Bit 3: Analog Comparator Interrupt Enable
    reg        acic;  // Bit 2: Analog Comparator Input Capture Enable
    reg  [1:0] acis;  // Bits 1:0: Analog Comparator Interrupt Mode Select
    
    // Internal signals
    wire       ac_out_raw;
    reg        ac_out_sync_1;
    reg        ac_out_sync_2;
    reg        ac_out_prev;
    
    // Comparator Logic
    // If ACD is set, the comparator is disabled.
    // We'll assume 0 output when disabled for now, or hold previous value.
    // Real hardware might float or clamp.
    assign ac_out_raw = (acd) ? 1'b0 : (ain0 > ain1);
    
    // Synchronizer for the comparator output
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ac_out_sync_1 <= 1'b0;
            ac_out_sync_2 <= 1'b0;
            ac_out_prev   <= 1'b0;
        end else begin
            ac_out_sync_1 <= ac_out_raw;
            ac_out_sync_2 <= ac_out_sync_1;
            ac_out_prev   <= ac_out_sync_2;
        end
    end
    
    // Update ACO bit (read-only from software perspective, reflects sync output)
    always @(*) begin
        aco = ac_out_sync_2;
    end
    
    // Interrupt Generation Logic
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            aci <= 1'b0;
        end else begin
            // Clear interrupt if 1 is written to ACI
            if (mem_valid && |mem_wstrb && (mem_addr[7:0] == 8'h50) && mem_wdata[4]) begin
                aci <= 1'b0;
            end else if (!acd) begin
                case (acis)
                    2'b00: begin // Toggle
                        if (ac_out_sync_2 != ac_out_prev)
                            aci <= 1'b1;
                    end
                    2'b01: begin // Reserved
                        // No action
                    end
                    2'b10: begin // Falling Edge
                        if (ac_out_prev && !ac_out_sync_2)
                            aci <= 1'b1;
                    end
                    2'b11: begin // Rising Edge
                        if (!ac_out_prev && ac_out_sync_2)
                            aci <= 1'b1;
                    end
                endcase
            end
        end
    end
    
    // Interrupt Output
    always @(*) begin
        irq = acie && aci;
    end

    // Bus Interface Logic
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            acd   <= 1'b0;
            acbg  <= 1'b0;
            acie  <= 1'b0;
            acic  <= 1'b0;
            acis  <= 2'b00;
            mem_ready <= 1'b0;
            mem_rdata <= 32'h0;
        end else begin
            mem_ready <= 1'b0;
            
            if (mem_valid && !mem_ready) begin
                mem_ready <= 1'b1;
                
                // Address decoding (check lower 8 bits for 0x50)
                if (mem_addr[7:0] == 8'h50) begin
                    // Write
                    if (|mem_wstrb) begin
                        if (mem_wstrb[0]) begin
                            acd   <= mem_wdata[7];
                            acbg  <= mem_wdata[6];
                            // Bit 5 (ACO) is read-only
                            // Bit 4 (ACI) is cleared by writing 1 (handled in separate block)
                            acie  <= mem_wdata[3];
                            acic  <= mem_wdata[2];
                            acis  <= mem_wdata[1:0];
                        end
                    end
                    
                    // Read
                    mem_rdata <= {24'h000000, acd, acbg, aco, aci, acie, acic, acis};
                end else begin
                    mem_rdata <= 32'h0;
                end
            end
        end
    end

endmodule
