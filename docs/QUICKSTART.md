# Quick Start Guide

Get up and running with the ATmega328P RISC-V replica in minutes!

## Step 1: Install Prerequisites

### Minimum Requirements
- RISC-V GCC toolchain
- Icarus Verilog (for simulation)

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install gcc-riscv64-unknown-elf iverilog gtkwave
```

**macOS (using Homebrew):**
```bash
brew install riscv-gnu-toolchain iverilog gtkwave
```

## Step 2: Build the Firmware

```bash
cd software/firmware
make
```

This creates `firmware.mem` which will be loaded into the ROM.

## Step 3: Run Simulation

From the project root:
```bash
make sim
```

Or manually:
```bash
cd testbench
make sim
```

You should see GPIO output toggling in the console output.

## Step 4: View Waveforms (Optional)

```bash
cd testbench
make view
```

This opens GTKWave where you can see:
- Clock and reset signals
- GPIO port B output (`gpio_pin_out`)
- GPIO direction register (`gpio_pin_dir`)

## Step 5: Program FPGA (Hardware)

### For Xilinx FPGAs (e.g., Arty A7)

1. Build firmware (Step 2)
2. Open Vivado
3. Create new project or use the TCL script:
   ```bash
   cd fpga/synthesis
   vivado -mode batch -source vivado_project.tcl
   ```
4. Program the FPGA with the generated bitstream
5. Connect an LED to the GPIO pin (PB0) - it should blink!

### Pin Connections (Arty A7 example)
- `gpio_pin_out[0]` → LED0 (H5)
- `gpio_pin_out[1]` → LED1 (J5)
- Clock: 100MHz on-board oscillator
- Reset: Button D9 (active low)

## What's Next?

- Modify `software/firmware/blinky.c` to change the blink pattern
- Add more GPIO ports (Port C, Port D)
- Implement additional peripherals (UART, timers)
- See `BUILD.md` for detailed instructions

## Troubleshooting

**"riscv32-unknown-elf-gcc: command not found"**
- Install RISC-V toolchain (see Step 1)
- Or set `RISCV_PREFIX` in the Makefile to match your toolchain

**"firmware.mem not found"**
- Make sure you built the firmware first (Step 2)
- Check that `hex2mem.py` ran successfully

**Simulation shows no activity**
- Verify firmware.mem exists in testbench directory
- Check that ROM module is reading the file correctly
- Look for error messages in simulation output

