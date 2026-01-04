# Makefile for FPGA synthesis using Yosys and nextpnr
# This is a basic example - adjust for your specific FPGA

# FPGA device (example for Lattice iCE40)
DEVICE = hx8k
PACKAGE = ct256

# Project name
PROJECT = soc_top

# Source files
RTL_SOURCES = ../../rtl/core/picorv32.v \
              ../../rtl/memory/rom.v \
              ../../rtl/memory/ram.v \
              ../../rtl/peripherals/gpio.v \
              ../../rtl/bus/bus_decoder.v \
              ../../rtl/soc_top.v

# Constraints
CONSTRAINTS = ../constraints/arty_a7.xdc

# Output files
JSON = $(PROJECT).json
ASC = $(PROJECT).asc
BIN = $(PROJECT).bin

.PHONY: all synth place route bitstream clean

all: bitstream

synth: $(JSON)

$(JSON): $(RTL_SOURCES)
	yosys -p "read_verilog -I../../rtl/bus $(RTL_SOURCES); \
	          synth_ice40 -top soc_top -json $@"

place: $(ASC)

$(ASC): $(JSON) $(CONSTRAINTS)
	nextpnr-ice40 --$(DEVICE) --package $(PACKAGE) --json $< --asc $@

route: $(ASC)

bitstream: $(BIN)

$(BIN): $(ASC)
	icepack $< $@

clean:
	rm -f $(JSON) $(ASC) $(BIN)

# Note: For Xilinx FPGAs, use Vivado instead
# For Intel/Altera FPGAs, use Quartus

