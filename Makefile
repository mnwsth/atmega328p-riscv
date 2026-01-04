# Top-level Makefile for the project

.PHONY: all firmware sim clean help

all: help

firmware:
	@echo "Building firmware..."
	cd software/firmware && make

sim: firmware
	@echo "Running simulation..."
	@cp software/firmware/firmware.mem testbench/
	cd testbench && make sim

view:
	cd testbench && make view

clean:
	@echo "Cleaning..."
	cd software/firmware && make clean
	cd testbench && make clean

help:
	@echo "ATmega328P RISC-V Replica - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  firmware  - Build the firmware"
	@echo "  sim       - Run simulation"
	@echo "  view      - View simulation waveforms"
	@echo "  clean     - Clean all build artifacts"
	@echo "  help      - Show this help message"

