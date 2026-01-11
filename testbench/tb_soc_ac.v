`timescale 1ns / 1ps

module tb_soc_ac;

    // Clock and reset
    reg clk;
    reg rst_n;
    
    // GPIO Port B signals
    reg [7:0] gpio_pin_in_b;
    wire [7:0] gpio_pin_out_b;
    wire [7:0] gpio_pin_dir_b;
    
    // GPIO Port D signals
    reg [7:0] gpio_pin_in_d;
    wire [7:0] gpio_pin_out_d;
    wire [7:0] gpio_pin_dir_d;

    // GPIO Port C signals
    reg [7:0] gpio_pin_in_c;
    wire [7:0] gpio_pin_out_c;
    wire [7:0] gpio_pin_dir_c;

    // Analog Comparator signals
    reg ain0;
    reg ain1;
    
    // Instantiate DUT
    soc_top dut (
        .clk(clk),
        .rst_n(rst_n),
        .gpio_pin_in_b(gpio_pin_in_b),
        .gpio_pin_out_b(gpio_pin_out_b),
        .gpio_pin_dir_b(gpio_pin_dir_b),
        .gpio_pin_in_d(gpio_pin_in_d),
        .gpio_pin_out_d(gpio_pin_out_d),
        .gpio_pin_dir_d(gpio_pin_dir_d),
        .gpio_pin_in_c(gpio_pin_in_c),
        .gpio_pin_out_c(gpio_pin_out_c),
        .gpio_pin_dir_c(gpio_pin_dir_c),
        .ain0(ain0),
        .ain1(ain1)
    );
    
    // Clock generation (10MHz = 100ns period)
    initial begin
        clk = 0;
        forever #50 clk = ~clk;
    end
    
    // Reset generation
    initial begin
        rst_n = 0;
        gpio_pin_in_b = 8'h00;
        gpio_pin_in_d = 8'h00;
        gpio_pin_in_c = 8'h00;
        ain0 = 0;
        ain1 = 0;
        #1000;
        rst_n = 1;
        $display("Reset released at time %0t", $time);
    end
    
    // AC Test Sequence
    initial begin
        // Wait for reset
        #2000;
        
        $display("Starting Analog Comparator Integration Test");
        
        // Test 1: AIN0 > AIN1
        ain0 = 1;
        ain1 = 0;
        #200; // Wait for sync
        $display("AIN0=1, AIN1=0. Checking internal ACO...");
        if (dut.ac_inst.aco === 1'b1) 
            $display("PASS: Internal ACO is 1");
        else 
            $display("FAIL: Internal ACO is %b", dut.ac_inst.aco);
            
        // Test 2: AIN0 < AIN1
        ain0 = 0;
        ain1 = 1;
        #200;
        $display("AIN0=0, AIN1=1. Checking internal ACO...");
        if (dut.ac_inst.aco === 1'b0) 
            $display("PASS: Internal ACO is 0");
        else 
            $display("FAIL: Internal ACO is %b", dut.ac_inst.aco);
            
        // Test 3: Check IRQ connection
        // Since firmware is not enabling interrupts, we can't check CPU reaction easily.
        // But we can check if the IRQ signal from AC is connected to CPU.
        // We can force ACIE in the AC instance to 1 to see if IRQ propagates.
        
        $display("Forcing ACIE=1 to check IRQ propagation...");
        // Force write to ACSR (or just force the register value)
        // Since we can't easily drive bus from here without interfering with CPU,
        // we will force the internal register.
        force dut.ac_inst.acie = 1'b1;
        force dut.ac_inst.acis = 2'b00; // Toggle mode
        
        // Toggle inputs
        ain0 = 1;
        ain1 = 0;
        #200;
        
        if (dut.ac_inst.irq === 1'b1)
            $display("PASS: AC IRQ generated");
        else
            $display("FAIL: AC IRQ not generated");
            
        // Check connection to CPU
        // CPU irq input is a wire, we can check it.
        // In soc_top: .irq({31'b0, ac_irq})
        // In cpu instance: .irq(32'h00000000) -> wait, I connected it in soc_top.v
        // Let's check dut.cpu.irq
        if (dut.cpu.irq[0] === 1'b1)
            $display("PASS: CPU IRQ[0] is high");
        else
            $display("FAIL: CPU IRQ[0] is %b", dut.cpu.irq[0]);
            
        // Test 4: Bus Aliasing Check
        $display("Test 4: Bus Aliasing Check");
        // Write to ACSR and check if GPIO registers are affected
        // ACSR is at 0x20000050
        // GPIO Port B DDR is at 0x20000024
        
        // We can't easily write to bus from here as we are outside the CPU.
        // However, we can check if writing to GPIO (via firmware/CPU simulation) affects ACSR.
        // Since we are not running firmware here, we can only check default values or force signals.
        
        // Let's verify that the address decoding logic in bus_decoder is correct by inspection or by 
        // simulating bus transactions if we had a bus master BFM.
        // Since we don't, we will rely on the fact that we verified bus_decoder in unit tests (implicitly via soc_test).
        
        // But wait, we can check if the ACSR register changes when we toggle GPIO pins? No, that's external.
        // We can check if `dut.decoder.ac_sel` is strictly decoding 0x50.
        // We can force cpu_mem_addr to 0x20000024 and check ac_sel.
        
        force dut.cpu_mem_valid = 1'b1;
        force dut.cpu_mem_addr = 32'h20000024; // GPIO DDRB
        #10;
        if (dut.decoder.ac_sel === 1'b1) $display("FAIL: AC selected at GPIO address!");
        else $display("PASS: AC not selected at GPIO address");
        
        force dut.cpu_mem_addr = 32'h20000050; // ACSR
        #10;
        if (dut.decoder.ac_sel === 1'b1) $display("PASS: AC selected at ACSR address");
        else $display("FAIL: AC not selected at ACSR address");
        
        force dut.cpu_mem_addr = 32'h20000054; // Near ACSR
        #10;
        if (dut.decoder.ac_sel === 1'b1) $display("FAIL: AC selected at 0x54!");
        else $display("PASS: AC not selected at 0x54");
        
        release dut.cpu_mem_valid;
        release dut.cpu_mem_addr;

        $display("Integration Test Complete");
        $finish;
    end
    
    // Dump waves
    initial begin
        $dumpfile("tb_soc_ac.vcd");
        $dumpvars(0, tb_soc_ac);
    end

endmodule
