# Pin constraints for Digilent Arty A7-35T FPGA board
# Adjust pin numbers for your specific board

# Clock - 100MHz on-board oscillator
set_property PACKAGE_PIN E3 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports clk]
create_clock -period 10.000 -name sys_clk [get_ports clk]

# Reset - Button (active low)
set_property PACKAGE_PIN D9 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]

# GPIO Port B - Connected to LEDs
# LED0 = PB0
set_property PACKAGE_PIN H5 [get_ports {gpio_pin_out[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out[0]}]

# LED1 = PB1
set_property PACKAGE_PIN J5 [get_ports {gpio_pin_out[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out[1]}]

# LED2 = PB2
set_property PACKAGE_PIN T9 [get_ports {gpio_pin_out[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out[2]}]

# LED3 = PB3
set_property PACKAGE_PIN T10 [get_ports {gpio_pin_out[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out[3]}]

# GPIO inputs (if needed) - Connected to switches
# SW0 = PB4
set_property PACKAGE_PIN A8 [get_ports {gpio_pin_in[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in[4]}]

# SW1 = PB5
set_property PACKAGE_PIN C11 [get_ports {gpio_pin_in[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in[5]}]

# Note: Adjust pin assignments based on your specific FPGA board

