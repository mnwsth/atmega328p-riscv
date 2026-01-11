`timescale 1ns / 1ps

module analog_comparator_tb;

    // Inputs
    reg clk;
    reg rst_n;
    reg ain0;
    reg ain1;
    reg mem_valid;
    reg [31:0] mem_addr;
    reg [31:0] mem_wdata;
    reg [3:0] mem_wstrb;

    // Outputs
    wire [31:0] mem_rdata;
    wire mem_ready;
    wire irq;

    // Instantiate the Unit Under Test (UUT)
    analog_comparator uut (
        .clk(clk),
        .rst_n(rst_n),
        .ain0(ain0),
        .ain1(ain1),
        .mem_valid(mem_valid),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_wstrb(mem_wstrb),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready),
        .irq(irq)
    );

    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // Test sequence
    initial begin
        // Initialize Inputs
        rst_n = 0;
        ain0 = 0;
        ain1 = 0;
        mem_valid = 0;
        mem_addr = 0;
        mem_wdata = 0;
        mem_wstrb = 0;

        // Wait for global reset
        #100;
        rst_n = 1;
        #20;

        // Test 0: Reset Values
        $display("Test 0: Reset Values");
        read_register(32'h20000050);
        if (mem_rdata !== 32'h00000000) $display("Error: Reset value should be 0, got %h", mem_rdata);
        else $display("Pass: Reset value is 0");
        
        // Test 1: Basic Comparison (AIN0 > AIN1)
        $display("Test 1: Basic Comparison (AIN0 > AIN1)");
        ain0 = 1;
        ain1 = 0;
        #50;
        // Read ACSR
        read_register(32'h20000050);
        $display("Debug: ACO=%b, Raw=%b, Sync2=%b", mem_rdata[5], uut.ac_out_raw, uut.ac_out_sync_2);
        if (mem_rdata[5] !== 1'b1) $display("Error: ACO should be 1");
        else $display("Pass: ACO is 1");

        // Test 2: Basic Comparison (AIN0 < AIN1)
        $display("Test 2: Basic Comparison (AIN0 < AIN1)");
        ain0 = 0;
        ain1 = 1;
        #50;
        // Read ACSR
        read_register(32'h20000050);
        $display("Debug: ACO=%b, Raw=%b, Sync2=%b", mem_rdata[5], uut.ac_out_raw, uut.ac_out_sync_2);
        if (mem_rdata[5] !== 1'b0) $display("Error: ACO should be 0");
        else $display("Pass: ACO is 0");

        // Test 3: Interrupt Generation (Toggle Mode - Default)
        $display("Test 3: Interrupt Generation (Toggle Mode)");
        // Enable Interrupt (ACIE = 1)
        write_register(32'h20000050, 8'h08); // ACIE=1, ACIS=00 (Toggle)
        
        // Toggle Output (0 -> 1)
        ain0 = 1;
        ain1 = 0;
        #50;
        $display("Debug: IRQ=%b, ACI=%b, Prev=%b, Sync2=%b", irq, uut.aci, uut.ac_out_prev, uut.ac_out_sync_2);
        if (irq !== 1'b1) $display("Error: IRQ should be 1 on toggle");
        else $display("Pass: IRQ triggered on toggle");
        
        // Clear Interrupt (Write 1 to ACI)
        write_register(32'h20000050, 8'h18); // ACI=1 (to clear), ACIE=1
        #20;
        $display("Debug: IRQ=%b, ACI=%b", irq, uut.aci);
        if (irq !== 1'b0) $display("Error: IRQ should be cleared");
        else $display("Pass: IRQ cleared");

        // Test 4: Interrupt Generation (Rising Edge)
        $display("Test 4: Interrupt Generation (Rising Edge)");
        // Set Rising Edge Mode (ACIS=11)
        write_register(32'h20000050, 8'h0B); // ACIE=1, ACIS=11
        
        // Ensure IRQ is cleared before starting
        if (irq) begin
             $display("Warning: IRQ still set, attempting to clear...");
             write_register(32'h20000050, 8'h1B); // Clear ACI, Keep ACIE=1, ACIS=11
             #20;
        end

        // Falling Edge (1 -> 0)
        ain0 = 0;
        ain1 = 1;
        #50;
        if (irq === 1'b1) $display("Error: IRQ should NOT trigger on falling edge");
        else $display("Pass: IRQ did not trigger on falling edge");
        
        // Rising Edge (0 -> 1)
        ain0 = 1;
        ain1 = 0;
        #50;
        if (irq !== 1'b1) $display("Error: IRQ should trigger on rising edge");
        else $display("Pass: IRQ triggered on rising edge");

        // Test 5: Disable Comparator
        $display("Test 5: Disable Comparator");
        write_register(32'h20000050, 8'h80); // ACD=1
        #50;
        // Change inputs, output should not change (or be 0 depending on implementation)
        ain0 = 0;
        ain1 = 1;
        #50;
        read_register(32'h20000050);
        // In our implementation, disabled output is 0
        if (mem_rdata[5] !== 1'b0) $display("Error: ACO should be 0 when disabled");
        else $display("Pass: ACO is 0 when disabled");

        // Test 6: Bit Read/Write Verification
        $display("Test 6: Bit Read/Write Verification");
        // Write pattern to ACBG, ACIE, ACIC, ACIS
        // Pattern: ACBG=1, ACIE=0, ACIC=1, ACIS=10 -> 0x46 (ACD=0)
        write_register(32'h20000050, 8'h46);
        read_register(32'h20000050);
        if (mem_rdata[6] !== 1'b1) $display("Error: ACBG should be 1");
        if (mem_rdata[3] !== 1'b0) $display("Error: ACIE should be 0");
        if (mem_rdata[2] !== 1'b1) $display("Error: ACIC should be 1");
        if (mem_rdata[1:0] !== 2'b10) $display("Error: ACIS should be 10");
        $display("Pass: Bit Read/Write Verification 1");

        // Write inverse pattern
        // Pattern: ACBG=0, ACIE=1, ACIC=0, ACIS=01 -> 0x09 (ACD=0)
        write_register(32'h20000050, 8'h09);
        read_register(32'h20000050);
        if (mem_rdata[6] !== 1'b0) $display("Error: ACBG should be 0");
        if (mem_rdata[3] !== 1'b1) $display("Error: ACIE should be 1");
        if (mem_rdata[2] !== 1'b0) $display("Error: ACIC should be 0");
        if (mem_rdata[1:0] !== 2'b01) $display("Error: ACIS should be 01");
        $display("Pass: Bit Read/Write Verification 2");

        // Test 7: Read-Only Bits (ACO)
        $display("Test 7: Read-Only Bits (ACO)");
        // Enable comparator, set output to 1
        ain0 = 1; ain1 = 0;
        write_register(32'h20000050, 8'h00); // Enable
        #20;
        // Try to write 0 to ACO (bit 5)
        write_register(32'h20000050, 8'h00); // Write 0 to bit 5 (implicit)
        read_register(32'h20000050);
        if (mem_rdata[5] !== 1'b1) $display("Error: ACO should remain 1");
        
        // Try to write 1 to ACO (bit 5) while output is 0
        ain0 = 0; ain1 = 1;
        #20;
        write_register(32'h20000050, 8'h20); // Write 1 to bit 5
        read_register(32'h20000050);
        if (mem_rdata[5] !== 1'b0) $display("Error: ACO should remain 0");
        $display("Pass: ACO is read-only");

        $display("All tests completed");
        $finish;
    end

    // Task to write to register
    task write_register;
        input [31:0] addr;
        input [7:0] data;
        begin
            @(posedge clk);
            mem_valid = 1;
            mem_addr = addr;
            mem_wdata = {24'h0, data};
            mem_wstrb = 4'h1; // Byte write
            wait(mem_ready);
            @(posedge clk);
            mem_valid = 0;
            mem_wstrb = 0;
        end
    endtask

    // Task to read from register
    task read_register;
        input [31:0] addr;
        begin
            @(posedge clk);
            mem_valid = 1;
            mem_addr = addr;
            mem_wstrb = 0; // Read
            wait(mem_ready);
            @(posedge clk);
            mem_valid = 0;
        end
    endtask

endmodule
