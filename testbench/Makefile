# Makefile for simulation

SIMULATOR = iverilog
# Use NovyWave on macOS (native, supports macOS 14+), fallback to gtkwave on Linux
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	VIEWER = /Applications/NovyWave.app/Contents/MacOS/novywave
else
	VIEWER = gtkwave
endif

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
ifeq ($(UNAME_S),Darwin)
	@echo "Copying VCD file to /tmp/vcs_files for NovyWave access..."
	@mkdir -p /tmp/vcs_files
	@cp $(VCD) /tmp/vcs_files/tb_soc.vcd
	@echo "Opening NovyWave..."
	@echo "  -> Click 'Load Files' button"
	@echo "  -> Navigate to 'tmp' -> 'vcs_files' directory"
	@echo "  -> Select 'tb_soc.vcd'"
	@open -a NovyWave &
else
	$(VIEWER) $(VCD) &
endif

clean:
	rm -f $(VVP) $(VCD) $(FIRMWARE_MEM)
	@echo "To clean VCD files in /tmp/vcs_files, run: rm -rf /tmp/vcs_files"

