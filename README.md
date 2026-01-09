# ATmega328P RISC-V Replica

This project implements a RISC-V based System-on-Chip (SoC) that replicates the functionality of an ATmega328P microcontroller. The initial implementation includes GPIO support to run a simple LED blinking program on an FPGA.

## Project Structure

```
.
├── rtl/                    # RTL source files
│   ├── core/              # RISC-V core (PicoRV32)
│   ├── memory/            # ROM and RAM modules
│   ├── peripherals/       # GPIO and other peripherals
│   ├── bus/               # Bus decoder and memory map
│   └── soc_top.v          # Top-level SoC module
├── testbench/             # Simulation testbenches
├── software/               # Firmware source code
│   └── firmware/          # C programs and build system
├── fpga/                   # FPGA implementation files
│   ├── constraints/       # Pin constraint files
│   └── synthesis/         # Synthesis scripts
└── docs/                   # Documentation

```

## Memory Map

- **0x00000000 - 0x0000FFFF**: Program ROM (64KB)
- **0x10000000 - 0x10000FFF**: Data RAM (4KB)
- **0x20000000 - 0x20000FFF**: I/O Space (Peripherals)
  - **0x20000023**: PINB (GPIO Port B Input)
  - **0x20000024**: DDRB (GPIO Port B Data Direction)
  - **0x20000025**: PORTB (GPIO Port B Output)

## Building the Firmware

1. Install RISC-V toolchain:
   ```bash
   # On Ubuntu/Debian:
   sudo apt-get install gcc-riscv64-unknown-elf
   
   # Or build from source:
   # https://github.com/riscv/riscv-gnu-toolchain
   ```

2. Build the firmware:
   ```bash
   cd software/firmware
   make
   ```

   This will generate `firmware.hex` which can be loaded into the ROM.

## Simulation

1. Install simulation tools:
   ```bash
   # Linux - Verilator (recommended, faster)
   sudo apt-get install verilator gtkwave
   
   # Linux - Or Icarus Verilog (alternative)
   sudo apt-get install iverilog gtkwave
   
   # macOS - Verilator
   brew install verilator
   # macOS - Waveform viewer: Install NovyWave from https://github.com/NovyWave/NovyWave/releases
   ```

2. Build firmware for simulation (uses shorter delays):
   ```bash
   cd software/firmware
   make sim
   ```

3. Run simulation:
   ```bash
   # Using Verilator (recommended)
   cd testbench
   make -f Makefile.verilator sim
   
   # Or using Icarus Verilog
   cd testbench
   make sim
   ```

4. View waveforms:
   ```bash
   make view
   ```
   
   **Note**: 
   - **macOS**: NovyWave is recommended (native app, supports macOS 14+). The Makefile copies VCD files to `/tmp/vcs_files/` for easy access. In NovyWave, click "Load Files" and navigate to `tmp` → `vcs_files` to load the file. Cleanup: `rm -rf /tmp/vcs_files`
   - **Linux**: GTKWave works well and opens directly
   - GTKWave is deprecated and incompatible with macOS 14+

## FPGA Implementation

### For Xilinx FPGAs (Vivado)

1. Create a new Vivado project
2. Add all RTL sources from `rtl/` directory
3. Add constraint file from `fpga/constraints/`
4. Set `firmware.mem` as initialization file for ROM
5. Run synthesis, implementation, and generate bitstream

### For Lattice iCE40 (open-source toolchain)

```bash
cd fpga/synthesis
make bitstream
```

### For Intel/Altera FPGAs (Quartus)

1. Create a new Quartus project
2. Add all RTL sources
3. Add pin assignments from constraints file
4. Set ROM initialization file
5. Compile and program

## Current Status

- ✅ RISC-V core integration (PicoRV32)
- ✅ Memory system (ROM and RAM)
- ✅ GPIO peripheral (Port B)
- ✅ Bus decoder
- ✅ Simple blinky firmware
- ⏳ Full ATmega328P peripheral set (in progress)

## Next Steps

- Add more GPIO ports (Port C, Port D)
- Implement timers
- Add UART support
- Implement ADC
- Add interrupt controller
- Implement EEPROM emulation

## License

This project uses the PicoRV32 core which is released under the ISC license.
Other components are provided as-is for educational purposes.

