# Pin constraints for Digilent Arty A7-35T FPGA board
# Matching soc_top.v port names

# =============================================================================
# Clock and Reset
# =============================================================================

# Clock - 100MHz on-board oscillator
set_property PACKAGE_PIN E3 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports clk]
create_clock -period 10.000 -name sys_clk [get_ports clk]

# Reset - Button BTN0 (directly active-low as design expects)
set_property PACKAGE_PIN D9 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]

# =============================================================================
# GPIO Port B - Primary test port (directly active LEDs & switches)
# =============================================================================

# Port B Outputs - Connected to LEDs LD0-LD3
set_property PACKAGE_PIN H5 [get_ports {gpio_pin_out_b[0]}]
set_property PACKAGE_PIN J5 [get_ports {gpio_pin_out_b[1]}]
set_property PACKAGE_PIN T9 [get_ports {gpio_pin_out_b[2]}]
set_property PACKAGE_PIN T10 [get_ports {gpio_pin_out_b[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_b[*]}]

# Port B Outputs - Connected to RGB LED 0 (accent LEDs)
set_property PACKAGE_PIN E1 [get_ports {gpio_pin_out_b[4]}]
set_property PACKAGE_PIN F6 [get_ports {gpio_pin_out_b[5]}]
set_property PACKAGE_PIN G6 [get_ports {gpio_pin_out_b[6]}]
set_property PACKAGE_PIN G4 [get_ports {gpio_pin_out_b[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_b[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_b[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_b[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_b[7]}]

# Port B Inputs - Connected to Switches SW0-SW3
set_property PACKAGE_PIN A8 [get_ports {gpio_pin_in_b[0]}]
set_property PACKAGE_PIN C11 [get_ports {gpio_pin_in_b[1]}]
set_property PACKAGE_PIN C10 [get_ports {gpio_pin_in_b[2]}]
set_property PACKAGE_PIN A10 [get_ports {gpio_pin_in_b[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_b[*]}]

# Port B Inputs - Connected to Buttons BTN1-BTN3 (active low)
set_property PACKAGE_PIN C9 [get_ports {gpio_pin_in_b[4]}]
set_property PACKAGE_PIN B9 [get_ports {gpio_pin_in_b[5]}]
set_property PACKAGE_PIN B8 [get_ports {gpio_pin_in_b[6]}]
set_property PACKAGE_PIN A7 [get_ports {gpio_pin_in_b[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_b[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_b[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_b[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_b[7]}]

# Port B Direction - directly active Pmod Header JA (directly active directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly directly
set_property PACKAGE_PIN G13 [get_ports {gpio_pin_dir_b[0]}]
set_property PACKAGE_PIN B11 [get_ports {gpio_pin_dir_b[1]}]
set_property PACKAGE_PIN A11 [get_ports {gpio_pin_dir_b[2]}]
set_property PACKAGE_PIN D12 [get_ports {gpio_pin_dir_b[3]}]
set_property PACKAGE_PIN D13 [get_ports {gpio_pin_dir_b[4]}]
set_property PACKAGE_PIN B18 [get_ports {gpio_pin_dir_b[5]}]
set_property PACKAGE_PIN A18 [get_ports {gpio_pin_dir_b[6]}]
set_property PACKAGE_PIN K16 [get_ports {gpio_pin_dir_b[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_dir_b[*]}]

# =============================================================================
# GPIO Port C - Pmod Header JB
# =============================================================================

set_property PACKAGE_PIN E15 [get_ports {gpio_pin_out_c[0]}]
set_property PACKAGE_PIN E16 [get_ports {gpio_pin_out_c[1]}]
set_property PACKAGE_PIN D15 [get_ports {gpio_pin_out_c[2]}]
set_property PACKAGE_PIN C15 [get_ports {gpio_pin_out_c[3]}]
set_property PACKAGE_PIN J17 [get_ports {gpio_pin_out_c[4]}]
set_property PACKAGE_PIN J18 [get_ports {gpio_pin_out_c[5]}]
set_property PACKAGE_PIN K15 [get_ports {gpio_pin_out_c[6]}]
set_property PACKAGE_PIN J15 [get_ports {gpio_pin_out_c[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_c[*]}]

set_property PACKAGE_PIN U12 [get_ports {gpio_pin_in_c[0]}]
set_property PACKAGE_PIN V12 [get_ports {gpio_pin_in_c[1]}]
set_property PACKAGE_PIN V10 [get_ports {gpio_pin_in_c[2]}]
set_property PACKAGE_PIN V11 [get_ports {gpio_pin_in_c[3]}]
set_property PACKAGE_PIN U14 [get_ports {gpio_pin_in_c[4]}]
set_property PACKAGE_PIN V14 [get_ports {gpio_pin_in_c[5]}]
set_property PACKAGE_PIN T13 [get_ports {gpio_pin_in_c[6]}]
set_property PACKAGE_PIN U13 [get_ports {gpio_pin_in_c[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_c[*]}]

set_property PACKAGE_PIN U11 [get_ports {gpio_pin_dir_c[0]}]
set_property PACKAGE_PIN V16 [get_ports {gpio_pin_dir_c[1]}]
set_property PACKAGE_PIN M13 [get_ports {gpio_pin_dir_c[2]}]
set_property PACKAGE_PIN R10 [get_ports {gpio_pin_dir_c[3]}]
set_property PACKAGE_PIN R11 [get_ports {gpio_pin_dir_c[4]}]
set_property PACKAGE_PIN R13 [get_ports {gpio_pin_dir_c[5]}]
set_property PACKAGE_PIN R15 [get_ports {gpio_pin_dir_c[6]}]
set_property PACKAGE_PIN P15 [get_ports {gpio_pin_dir_c[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_dir_c[*]}]

# =============================================================================
# GPIO Port D - Pmod Header JC
# =============================================================================

set_property PACKAGE_PIN U18 [get_ports {gpio_pin_out_d[0]}]
set_property PACKAGE_PIN R17 [get_ports {gpio_pin_out_d[1]}]
set_property PACKAGE_PIN P17 [get_ports {gpio_pin_out_d[2]}]
set_property PACKAGE_PIN R18 [get_ports {gpio_pin_out_d[3]}]
set_property PACKAGE_PIN T18 [get_ports {gpio_pin_out_d[4]}]
set_property PACKAGE_PIN U17 [get_ports {gpio_pin_out_d[5]}]
set_property PACKAGE_PIN N14 [get_ports {gpio_pin_out_d[6]}]
set_property PACKAGE_PIN P14 [get_ports {gpio_pin_out_d[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_out_d[*]}]

set_property PACKAGE_PIN P16 [get_ports {gpio_pin_in_d[0]}]
set_property PACKAGE_PIN N17 [get_ports {gpio_pin_in_d[1]}]
set_property PACKAGE_PIN M17 [get_ports {gpio_pin_in_d[2]}]
set_property PACKAGE_PIN M18 [get_ports {gpio_pin_in_d[3]}]
set_property PACKAGE_PIN L17 [get_ports {gpio_pin_in_d[4]}]
set_property PACKAGE_PIN L18 [get_ports {gpio_pin_in_d[5]}]
set_property PACKAGE_PIN K17 [get_ports {gpio_pin_in_d[6]}]
set_property PACKAGE_PIN K18 [get_ports {gpio_pin_in_d[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_in_d[*]}]

set_property PACKAGE_PIN H17 [get_ports {gpio_pin_dir_d[0]}]
set_property PACKAGE_PIN H18 [get_ports {gpio_pin_dir_d[1]}]
set_property PACKAGE_PIN G17 [get_ports {gpio_pin_dir_d[2]}]
set_property PACKAGE_PIN G18 [get_ports {gpio_pin_dir_d[3]}]
set_property PACKAGE_PIN F17 [get_ports {gpio_pin_dir_d[4]}]
set_property PACKAGE_PIN F18 [get_ports {gpio_pin_dir_d[5]}]
set_property PACKAGE_PIN E17 [get_ports {gpio_pin_dir_d[6]}]
set_property PACKAGE_PIN E18 [get_ports {gpio_pin_dir_d[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio_pin_dir_d[*]}]

# =============================================================================
# Analog Comparator - Directly to Pmod Header JD (directly directly directly directly
# =============================================================================

set_property PACKAGE_PIN D4 [get_ports ain0]
set_property PACKAGE_PIN D3 [get_ports ain1]
set_property IOSTANDARD LVCMOS33 [get_ports ain0]
set_property IOSTANDARD LVCMOS33 [get_ports ain1]

# =============================================================================
# Timer0 External Pins
# =============================================================================

set_property PACKAGE_PIN F4 [get_ports t0_pin]
set_property PACKAGE_PIN F3 [get_ports oc0a]
set_property PACKAGE_PIN E2 [get_ports oc0b]
set_property IOSTANDARD LVCMOS33 [get_ports t0_pin]
set_property IOSTANDARD LVCMOS33 [get_ports oc0a]
set_property IOSTANDARD LVCMOS33 [get_ports oc0b]

# =============================================================================
# Timer2 External Pins
# =============================================================================

set_property PACKAGE_PIN D2 [get_ports oc2a]
set_property PACKAGE_PIN H2 [get_ports oc2b]
set_property IOSTANDARD LVCMOS33 [get_ports oc2a]
set_property IOSTANDARD LVCMOS33 [get_ports oc2b]

# =============================================================================
# Watchdog Timer
# =============================================================================

set_property PACKAGE_PIN G2 [get_ports wdt_reset_req]
set_property IOSTANDARD LVCMOS33 [get_ports wdt_reset_req]

# =============================================================================
# SPI Pins - Directly to ChipKit/Arduino Header
# =============================================================================

set_property PACKAGE_PIN F16 [get_ports spi_sck]
set_property PACKAGE_PIN D18 [get_ports spi_mosi]
set_property PACKAGE_PIN E18 [get_ports spi_miso]
set_property PACKAGE_PIN G16 [get_ports spi_ss_n]
set_property IOSTANDARD LVCMOS33 [get_ports spi_sck]
set_property IOSTANDARD LVCMOS33 [get_ports spi_mosi]
set_property IOSTANDARD LVCMOS33 [get_ports spi_miso]
set_property IOSTANDARD LVCMOS33 [get_ports spi_ss_n]

# SPI Slave mode additional pins
set_property PACKAGE_PIN H16 [get_ports spi_miso_out]
set_property PACKAGE_PIN H14 [get_ports spi_miso_oe]
set_property PACKAGE_PIN G14 [get_ports spi_mosi_in]
set_property PACKAGE_PIN D17 [get_ports spi_sck_in]
set_property IOSTANDARD LVCMOS33 [get_ports spi_miso_out]
set_property IOSTANDARD LVCMOS33 [get_ports spi_miso_oe]
set_property IOSTANDARD LVCMOS33 [get_ports spi_mosi_in]
set_property IOSTANDARD LVCMOS33 [get_ports spi_sck_in]

# =============================================================================
# Configuration and Bitstream Settings
# =============================================================================

set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]

