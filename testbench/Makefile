# Makefile for simulation

SIMULATOR = iverilog
VIEWER = gtkwave

# Source files
RTL_SOURCES = ../rtl/core/picorv32.v \
              ../rtl/memory/rom.v \
              ../rtl/memory/ram.v \
              ../rtl/peripherals/gpio.v \
              ../rtl/bus/bus_decoder.v \
              ../rtl/soc_top.v

TESTBENCH = tb_soc.v

# Output
VVP = tb_soc.vvp
VCD = tb_soc.vcd

# Firmware
FIRMWARE_MEM = firmware.mem

.PHONY: all sim view clean firmware

all: firmware sim

firmware:
	@echo "Building firmware..."; \
	cd ../software/firmware && make && \
	cp firmware.mem ../../testbench/

../software/firmware/firmware.mem:
	@echo "Building firmware..."; \
	cd ../software/firmware && make

$(FIRMWARE_MEM): ../software/firmware/firmware.mem
	@echo "Copying firmware.mem..."; \
	cp $< $@

sim: $(VVP) $(FIRMWARE_MEM)
	vvp $(VVP)

$(VVP): $(RTL_SOURCES) $(TESTBENCH)
	$(SIMULATOR) -o $(VVP) -I../rtl $(RTL_SOURCES) $(TESTBENCH)

view: $(VCD)
	$(VIEWER) $(VCD) &

clean:
	rm -f $(VVP) $(VCD) $(FIRMWARE_MEM)

