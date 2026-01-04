// Verilator testbench for SoC
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vsoc_top.h"
#include <iostream>

vluint64_t main_time = 0;

double sc_time_stamp() {
    return main_time;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    
    Vsoc_top* top = new Vsoc_top;
    VerilatedVcdC* tfp = new VerilatedVcdC;
    
    top->trace(tfp, 99);
    tfp->open("tb_soc.vcd");
    
    // Initialize
    top->clk = 0;
    top->rst_n = 0;
    top->gpio_pin_in = 0;
    
    // Reset
    for (int i = 0; i < 20; i++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(main_time++);
    }
    
    top->rst_n = 1;
    std::cout << "Reset released at time " << main_time << std::endl;
    
    // Monitor GPIO
    vluint8_t prev_gpio_out = 0;
    vluint8_t prev_gpio_dir = 0;
    int toggle_count = 0;
    int sample_count = 0;
    
    // Run simulation - enough cycles to see multiple toggles
    // With delay(1000) in firmware, each toggle takes ~2000-3000 cycles
    // So 1,000,000 iterations = 500,000 cycles should show many toggles
    for (int i = 0; i < 1000000; i++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(main_time++);
        
        // Sample GPIO every 1000 cycles to see if anything changes
        if (i % 2000 == 0 && i > 100) {
            sample_count++;
            if (sample_count <= 10) {
                std::cout << "Sample at cycle " << (main_time/2) 
                          << ": GPIO_PORTB=0x" << std::hex << (int)top->gpio_pin_out
                          << ", GPIO_DIR=0x" << (int)top->gpio_pin_dir << std::dec << std::endl;
            }
        }
        
        // Monitor GPIO changes
        if (top->gpio_pin_out != prev_gpio_out || top->gpio_pin_dir != prev_gpio_dir) {
            std::cout << "Time " << main_time << " (cycle " << (main_time/2) << "): GPIO_PORTB = 0x" 
                      << std::hex << (int)top->gpio_pin_out 
                      << ", GPIO_DIR = 0x" << (int)top->gpio_pin_dir 
                      << std::dec << std::endl;
            prev_gpio_out = top->gpio_pin_out;
            prev_gpio_dir = top->gpio_pin_dir;
            toggle_count++;
            if (toggle_count >= 10) break; // Stop after 10 toggles
        }
    }
    
    std::cout << "Simulation complete. GPIO toggled " << toggle_count << " times." << std::endl;
    
    tfp->close();
    delete top;
    delete tfp;
    
    return 0;
}

