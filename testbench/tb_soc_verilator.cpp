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
    top->gpio_pin_in_b = 0;
    top->gpio_pin_in_d = 0;
    
    // Reset
    for (int i = 0; i < 20; i++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(main_time++);
    }
    
    top->rst_n = 1;
    std::cout << "Reset released at time " << main_time << std::endl;
    
    // Monitor GPIO Port B
    vluint8_t prev_gpio_out_b = 0;
    vluint8_t prev_gpio_dir_b = 0;
    int toggle_count_b = 0;
    
    // Monitor GPIO Port D
    vluint8_t prev_gpio_out_d = 0;
    vluint8_t prev_gpio_dir_d = 0;
    int toggle_count_d = 0;
    
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
                          << ": GPIO_PORTB=0x" << std::hex << (int)top->gpio_pin_out_b
                          << ", GPIO_DIR_B=0x" << (int)top->gpio_pin_dir_b
                          << ", GPIO_PORTD=0x" << (int)top->gpio_pin_out_d
                          << ", GPIO_DIR_D=0x" << (int)top->gpio_pin_dir_d << std::dec << std::endl;
            }
        }
        
        // Monitor GPIO Port B changes
        if (top->gpio_pin_out_b != prev_gpio_out_b || top->gpio_pin_dir_b != prev_gpio_dir_b) {
            std::cout << "Time " << main_time << " (cycle " << (main_time/2) << "): GPIO_PORTB = 0x" 
                      << std::hex << (int)top->gpio_pin_out_b 
                      << ", GPIO_DIR_B = 0x" << (int)top->gpio_pin_dir_b 
                      << std::dec << std::endl;
            prev_gpio_out_b = top->gpio_pin_out_b;
            prev_gpio_dir_b = top->gpio_pin_dir_b;
            toggle_count_b++;
        }
        
        // Monitor GPIO Port D changes
        if (top->gpio_pin_out_d != prev_gpio_out_d || top->gpio_pin_dir_d != prev_gpio_dir_d) {
            std::cout << "Time " << main_time << " (cycle " << (main_time/2) << "): GPIO_PORTD = 0x" 
                      << std::hex << (int)top->gpio_pin_out_d 
                      << ", GPIO_DIR_D = 0x" << (int)top->gpio_pin_dir_d 
                      << std::dec << std::endl;
            prev_gpio_out_d = top->gpio_pin_out_d;
            prev_gpio_dir_d = top->gpio_pin_dir_d;
            toggle_count_d++;
        }
        
        if (toggle_count_b >= 10 && toggle_count_d >= 0) break; // Stop after 10 Port B toggles
    }
    
    std::cout << "Simulation complete. GPIO Port B toggled " << toggle_count_b << " times." << std::endl;
    std::cout << "GPIO Port D toggled " << toggle_count_d << " times." << std::endl;
    
    tfp->close();
    delete top;
    delete tfp;
    
    return 0;
}

