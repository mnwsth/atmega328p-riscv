# Build Instructions

This document provides detailed instructions for building and testing the ATmega328P RISC-V replica.

## Prerequisites

### Software Tools

1. **RISC-V Toolchain**
   - Install GCC for RISC-V: `sudo apt-get install gcc-riscv64-unknown-elf`
   - Or build from source: https://github.com/riscv/riscv-gnu-toolchain

2. **Simulation Tools**
   - Icarus Verilog: `sudo apt-get install iverilog`
   - GTKWave (optional, for waveform viewing): `sudo apt-get install gtkwave`

3. **FPGA Tools** (choose based on your FPGA)
   - **Xilinx**: Vivado (free WebPack version)
   - **Lattice**: Yosys + nextpnr + icepack
   - **Intel/Altera**: Quartus Prime Lite

## Building the Firmware

1. Navigate to the firmware directory:
   ```bash
   cd software/firmware
   ```

2. Build the firmware:
   ```bash
   make
   ```

   This will generate:
   - `firmware.elf` - ELF executable
   - `firmware.hex` - Intel HEX format
   - `firmware.bin` - Binary format
   - `firmware.mem` - Memory initialization file for Verilog

3. Check the size:
   ```bash
   make size
   ```

## Running Simulation

1. Build the firmware first (see above)

2. Run the simulation:
   ```bash
   cd testbench
   make sim
   ```

   Or from the root directory:
   ```bash
   make sim
   ```

3. View waveforms (optional):
   ```bash
   make view
   ```

   This opens GTKWave with the simulation results.

## FPGA Implementation

### For Xilinx FPGAs (Vivado)

1. **Using GUI:**
   - Open Vivado
   - Create new project
   - Add all RTL files from `rtl/` directory
   - Add constraint file from `fpga/constraints/arty_a7.xdc`
   - Set `soc_top` as top module
   - Set ROM initialization: Right-click on ROM instance → Properties → Memory → INIT File → Browse to `software/firmware/firmware.mem`
   - Run synthesis, implementation, and generate bitstream

2. **Using TCL Script:**
   ```bash
   cd fpga/synthesis
   vivado -mode batch -source vivado_project.tcl
   ```

3. **Program FPGA:**
   - Connect your FPGA board
   - Open Hardware Manager in Vivado
   - Program device with generated `.bit` file

### For Lattice iCE40 (Open Source)

1. Install tools:
   ```bash
   # Yosys
   sudo apt-get install yosys
   
   # nextpnr
   # Build from source: https://github.com/YosysHQ/nextpnr
   
   # icepack
   sudo apt-get install fpga-icestorm
   ```

2. Build:
   ```bash
   cd fpga/synthesis
   make bitstream
   ```

3. Program:
   ```bash
   iceprog firmware.bin
   ```

### For Intel/Altera (Quartus)

1. Create new project in Quartus
2. Add all RTL files
3. Add pin assignments from constraints file
4. Set ROM initialization file
5. Compile project
6. Program FPGA using Quartus Programmer

## Troubleshooting

### Firmware Build Issues

- **Toolchain not found**: Ensure RISC-V GCC is in your PATH
- **Linker errors**: Check that `linker.ld` is correct and all object files are generated

### Simulation Issues

- **ROM not initialized**: Ensure `firmware.mem` exists in `testbench/` directory
- **No waveforms**: Check that VCD file is generated and GTKWave can open it

### FPGA Issues

- **Synthesis errors**: Check for syntax errors in RTL files
- **Timing violations**: May need to adjust clock constraints or add pipeline stages
- **ROM not loading**: Verify INIT_FILE path is correct and firmware.mem exists

## Next Steps

Once you have a working implementation:

1. Test with the blinky program
2. Add more peripherals (UART, timers, etc.)
3. Optimize for your specific FPGA
4. Add more complex firmware examples

