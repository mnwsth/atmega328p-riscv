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
              ../rtl/peripherals/analog_comparator.v \
              ../rtl/peripherals/timer0.v \
              ../rtl/peripherals/watchdog_timer.v \
              ../rtl/bus/bus_decoder.v \
              ../rtl/soc_top.v

TESTBENCH = tb_soc.v

# Output
VVP = tb_soc.vvp
VCD = tb_soc.vcd

# Firmware
FIRMWARE_MEM = firmware.mem

.PHONY: all sim view clean firmware timer0_unit timer0_sim wdt_unit wdt_sim

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

# Timer0 Unit Test (standalone peripheral test)
timer0_unit:
	$(SIMULATOR) -o timer0_tb.vvp -I../rtl ../rtl/peripherals/timer0.v peripherals/timer0_tb.v
	vvp timer0_tb.vvp

# Timer0 Unit Test with waveform viewing
timer0_unit_view: timer0_unit
ifeq ($(UNAME_S),Darwin)
	@mkdir -p /tmp/vcs_files
	@cp timer0_tb.vcd /tmp/vcs_files/
	@open -a NovyWave &
else
	$(VIEWER) timer0_tb.vcd &
endif

# Timer0 Integration Test (full SoC with timer0_test firmware)
timer0_sim: timer0_firmware
	$(SIMULATOR) -o tb_soc_timer0.vvp -I../rtl $(RTL_SOURCES) tb_soc_timer0.v
	vvp tb_soc_timer0.vvp

# Timer0 test firmware
timer0_firmware:
	@echo "Building timer0 test firmware..."
	cd ../software/firmware && make timer0_test
	cp ../software/firmware/firmware.mem .

# Timer0 Integration Test with waveform viewing
timer0_sim_view: timer0_sim
ifeq ($(UNAME_S),Darwin)
	@mkdir -p /tmp/vcs_files
	@cp tb_soc_timer0.vcd /tmp/vcs_files/
	@open -a NovyWave &
else
	$(VIEWER) tb_soc_timer0.vcd &
endif

# Watchdog Timer Unit Test (standalone peripheral test)
wdt_unit:
	$(SIMULATOR) -o watchdog_timer_tb.vvp -I../rtl ../rtl/peripherals/watchdog_timer.v peripherals/watchdog_timer_tb.v
	vvp watchdog_timer_tb.vvp

# Watchdog Timer Unit Test with waveform viewing
wdt_unit_view: wdt_unit
ifeq ($(UNAME_S),Darwin)
	@mkdir -p /tmp/vcs_files
	@cp watchdog_timer_tb.vcd /tmp/vcs_files/
	@open -a NovyWave &
else
	$(VIEWER) watchdog_timer_tb.vcd &
endif

# Watchdog Timer Integration Test (full SoC with wdt_test firmware)
wdt_sim: wdt_firmware
	$(SIMULATOR) -o tb_soc_wdt.vvp -I../rtl $(RTL_SOURCES) tb_soc_wdt.v
	vvp tb_soc_wdt.vvp

# Watchdog Timer test firmware
wdt_firmware:
	@echo "Building WDT test firmware..."
	cd ../software/firmware && make wdt_test
	cp ../software/firmware/firmware.mem .

# Watchdog Timer Integration Test with waveform viewing
wdt_sim_view: wdt_sim
ifeq ($(UNAME_S),Darwin)
	@mkdir -p /tmp/vcs_files
	@cp tb_soc_wdt.vcd /tmp/vcs_files/
	@open -a NovyWave &
else
	$(VIEWER) tb_soc_wdt.vcd &
endif

clean:
	rm -f $(VVP) $(VCD) $(FIRMWARE_MEM) timer0_tb.vvp timer0_tb.vcd tb_soc_timer0.vvp tb_soc_timer0.vcd watchdog_timer_tb.vvp watchdog_timer_tb.vcd tb_soc_wdt.vvp tb_soc_wdt.vcd
	@echo "To clean VCD files in /tmp/vcs_files, run: rm -rf /tmp/vcs_files"

