# Build Instructions

This document provides detailed instructions for building and testing the ATmega328P RISC-V replica.

## Prerequisites

### Software Tools

1. **RISC-V Toolchain**
   - **Linux**: `sudo apt-get install gcc-riscv64-unknown-elf`
   - **macOS**: `brew install riscv-gnu-toolchain`
   - Or build from source: https://github.com/riscv/riscv-gnu-toolchain

2. **Simulation Tools**
   - **Verilator** (recommended, faster simulation)
     - Linux: `sudo apt-get install verilator`
     - macOS: `brew install verilator`
   - **Icarus Verilog** (alternative simulator)
     - Linux: `sudo apt-get install iverilog`
     - macOS: `brew install icarus-verilog`
   - **Waveform Viewer** (optional, for viewing VCD files)
     - **Linux**: `sudo apt-get install gtkwave`
     - **macOS**: 
       - **NovyWave** (recommended): Native macOS app, supports macOS 14+. Download from [GitHub Releases](https://github.com/NovyWave/NovyWave/releases) - install the `.dmg` for your architecture (aarch64 for Apple Silicon, x64 for Intel).
       - **GTKWave** (alternative): ⚠️ Deprecated and incompatible with macOS 14+. If you need gtkwave on macOS, you may need to build from source or use Linux.
     - Other alternatives: WaveTrace, Sigrok/PulseView, or other VCD viewers

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

   **For simulation** (shorter delays, faster to run):
   ```bash
   make sim
   ```
   This builds with `blinky_sim.c` which uses shorter delays suitable for simulation.

3. Check the size:
   ```bash
   make size
   ```

## Running Simulation

1. Build the firmware first (see above)
   - For simulation, use `make sim` in the firmware directory to build with shorter delays
   - For hardware, use `make` (standard build)

2. Run the simulation:

   **Using Verilator (recommended, faster):**
   ```bash
   cd testbench
   make -f Makefile.verilator sim
   ```

   **Using Icarus Verilog:**
   ```bash
   cd testbench
   make sim
   ```

   Or from the root directory (uses Icarus Verilog by default):
   ```bash
   make sim
   ```

3. View waveforms (optional):
   ```bash
   make view
   ```

   This opens NovyWave (on macOS) or GTKWave (on Linux) with the simulation results. On macOS, NovyWave is recommended as it's native and supports macOS 14+.

   **Loading files in NovyWave (macOS)**: The Makefile automatically copies the VCD file to `/tmp/vcs_files/`. In NovyWave:
   - Click "Load Files" button
   - Navigate to `tmp` → `vcs_files` directory
   - Select `tb_soc.vcd`
   
   **Note**: NovyWave's file picker doesn't show `/Users` directory on macOS. The file is copied to `/tmp/vcs_files/` to work around this limitation.
   
   **Cleanup**: To remove VCD files from `/tmp/vcs_files/`, run: `rm -rf /tmp/vcs_files`

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
   # Linux
   sudo apt-get install yosys fpga-icestorm
   
   # macOS
   brew install yosys icestorm
   
   # nextpnr (both platforms)
   # Build from source: https://github.com/YosysHQ/nextpnr
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

- **iverilog not found**: Install Icarus Verilog using the platform-specific instructions above
- **verilator not found**: Install Verilator using the platform-specific instructions above
- **ROM not initialized**: Ensure `firmware.mem` exists in `testbench/` directory
- **No waveforms**: Check that VCD file is generated and your waveform viewer can open it
- **Waveform viewer not found**:
  - **macOS**: Install NovyWave from [GitHub Releases](https://github.com/NovyWave/NovyWave/releases) (recommended) or try building gtkwave from source
  - **Linux**: Install gtkwave with `sudo apt-get install gtkwave`

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

