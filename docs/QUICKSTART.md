# Quick Start Guide

Get up and running with the ATmega328P RISC-V replica in minutes!

## Step 1: Install Prerequisites

### Minimum Requirements
- RISC-V GCC toolchain
- Verilator or Icarus Verilog (for simulation)

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install gcc-riscv64-unknown-elf verilator gtkwave
# Or use Icarus Verilog: sudo apt-get install gcc-riscv64-unknown-elf iverilog gtkwave
```

**macOS (using Homebrew):**
```bash
brew install riscv-gnu-toolchain verilator
# Or use Icarus Verilog: brew install riscv-gnu-toolchain icarus-verilog
```

**Waveform Viewer for macOS:**
- **NovyWave** (recommended): Native macOS app, supports macOS 14+. Download from [GitHub Releases](https://github.com/NovyWave/NovyWave/releases) - install the `.dmg` for your architecture (aarch64 for Apple Silicon, x64 for Intel).
- **GTKWave**: Deprecated and incompatible with macOS 14+. Not recommended for macOS users.

## Step 2: Build the Firmware

**For simulation** (recommended, uses shorter delays):
```bash
cd software/firmware
make sim
```

**For hardware** (standard build):
```bash
cd software/firmware
make
```

This creates `firmware.mem` which will be loaded into the ROM.

## Step 3: Run Simulation

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

Or from the project root (uses Icarus Verilog):
```bash
make sim
```

You should see GPIO output toggling in the console output.

## Step 4: View Waveforms (Optional)

```bash
cd testbench
make view
```

This opens NovyWave (on macOS) or GTKWave (on Linux) where you can see:
- Clock and reset signals
- GPIO port B output (`gpio_pin_out_b`) and direction (`gpio_pin_dir_b`)
- GPIO port D output (`gpio_pin_out_d`) and direction (`gpio_pin_dir_d`)

### Loading Waveforms in NovyWave (macOS)

On macOS, `make view` automatically copies the VCD file to `/tmp/vcs_files/` for easy access. To load the file:

1. In NovyWave, click the **"Load Files"** button in the "Files & Scopes" panel
2. Navigate to **`tmp`** directory (visible in the file picker)
3. Open the **`vcs_files`** subdirectory
4. Select **`tb_soc.vcd`**

**Note**: NovyWave's file picker on macOS doesn't show the `/Users` directory due to macOS security restrictions. The VCD file is automatically copied to `/tmp/vcs_files/` to work around this limitation.

**To clean up VCD files**: Run `rm -rf /tmp/vcs_files` when you're done viewing waveforms.

**Note**: On macOS, NovyWave is the recommended waveform viewer as it's native and supports macOS 14+. The generated `.vcd` files can also be opened in other VCD-compatible viewers.

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
- Add more GPIO ports (Port C)
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

