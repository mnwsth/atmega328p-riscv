# Simulation Guide

## Simulation Duration

The simulation duration depends on which testbench and firmware you use:

### Current Testbench Settings

**Verilator Testbench:**
- Maximum: 100,000 clock cycles (50,000 iterations × 2)
- Stops early after 10 GPIO toggles are detected
- Typical runtime: **1-5 seconds** (depending on CPU)

**Icarus Verilog Testbench:**
- Fixed duration: 100ms at 10MHz = 1,000,000 clock cycles
- Runtime: **Several seconds to minutes** (depending on simulator speed)

### Firmware Delay Impact

The original `blinky.c` uses a delay of 1,000,000 × 100 = **100 million cycles** per toggle. This is:
- **Too long for simulation** - would require hours/days to see one toggle
- **Appropriate for hardware** - creates visible LED blinking at typical clock speeds

For simulation, use `blinky_sim.c` which uses a delay of only **1,000 cycles** per toggle.

## What the Simulation Verifies

Once completed, the simulation will tell you:

### 1. **Basic Functionality**
- ✅ CPU boots correctly after reset
- ✅ Program ROM loads and executes instructions
- ✅ CPU can read from and write to memory
- ✅ Stack pointer is initialized correctly

### 2. **GPIO Peripheral**
- ✅ GPIO Port B registers are accessible at correct addresses (0x20000023-0x20000025)
- ✅ GPIO Port C registers are accessible at correct addresses (0x20000026-0x20000028)
- ✅ GPIO Port D registers are accessible at correct addresses (0x20000029-0x2000002B)
- ✅ DDR registers can be written (configures pin direction)
- ✅ PORT registers can be written (drives output pins)
- ✅ PIN registers reflect pin state (for inputs)
- ✅ GPIO pins change state when PORT registers are written
- ✅ All three ports (B, C, D) operate independently
- ✅ Word-aligned bus protocol with byte strobes works correctly

### 3. **Memory System**
- ✅ Program ROM responds to instruction fetches
- ✅ Data RAM can be read and written
- ✅ Bus decoder routes addresses correctly
- ✅ Memory timing is correct (ready signals work)

### 4. **Program Execution**
- ✅ Startup code (`start.S`) runs and sets up stack
- ✅ C code executes correctly
- ✅ Function calls work (delay function)
- ✅ Loops execute properly
- ✅ Bitwise operations work (XOR for toggling)

### 5. **Expected Output**

When simulation completes successfully, you should see:

```
Reset released at time 20
Time 5000 (cycle 2500): GPIO_PORTB = 0x1, GPIO_DIR = 0x1
Time 8000 (cycle 4000): GPIO_PORTB = 0x0, GPIO_DIR = 0x1
Time 11000 (cycle 5500): GPIO_PORTB = 0x1, GPIO_DIR = 0x1
Time 14000 (cycle 7000): GPIO_PORTB = 0x0, GPIO_DIR = 0x1
...
Simulation complete. GPIO toggled 10 times.
```

This confirms:
- GPIO pin 0 (PB0) is configured as output (GPIO_DIR_B = 0x1)
- PORTB bit 0 toggles between 0 and 1
- The blinky program is running correctly
- Port D is available for use (though not used by blinky program)

### 6. **Waveform Analysis**

The generated `tb_soc.vcd` file can be viewed in a waveform viewer to see:
- Clock and reset signals
- CPU memory bus transactions
- GPIO register writes (DDRB, PORTB, DDRD, PORTD)
- GPIO pin output changes (Port B and Port D)
- Memory read/write operations
- Instruction fetch cycles

**Waveform Viewer Options**:
- **Linux**: Use GTKWave (`sudo apt-get install gtkwave`) - opens directly with `make view`
- **macOS**: Use NovyWave (recommended, native macOS app, supports macOS 14+) - download from [GitHub Releases](https://github.com/NovyWave/NovyWave/releases)
  - The Makefile automatically copies VCD files to `/tmp/vcs_files/` for NovyWave access
  - In NovyWave: Click "Load Files" → Navigate to `tmp` → `vcs_files` → Select `tb_soc.vcd`
  - **Note**: NovyWave's file picker doesn't show `/Users` directory on macOS due to security restrictions
  - **Cleanup**: Run `rm -rf /tmp/vcs_files` to remove VCD files when done
- **Other platforms**: Any VCD-compatible viewer (WaveTrace, Sigrok/PulseView, etc.)

**Note**: GTKWave is deprecated and discontinued upstream (as of October 2025) and incompatible with macOS 14+. NovyWave is the recommended alternative for macOS users.

## Running Simulation with Optimized Firmware

To see toggles in reasonable time:

1. **Build simulation firmware:**
   ```bash
   cd software/firmware
   # Edit Makefile to use blinky_sim.c instead of blinky.c
   # Or manually: riscv64-unknown-elf-gcc ... blinky_sim.c ...
   ```

2. **Run simulation:**
   ```bash
   cd testbench
   make -f Makefile.verilator sim
   ```

## Troubleshooting

**No GPIO toggles observed:**
- Check that firmware.mem was generated correctly
- Verify ROM is initialized with firmware
- Check that delay is short enough for simulation duration
- Look for CPU trap signals (indicates errors)

**Simulation runs but shows no activity:**
- Check reset is released
- Verify clock is toggling
- Look at CPU memory bus - should see instruction fetches
- Check that program counter is incrementing

**GPIO_DIR stays at 0:**
- DDRB/DDRD register write may have failed
- Check bus decoder routes I/O addresses correctly
- Verify GPIO module receives write transactions
- Ensure correct port signals are being monitored (gpio_pin_dir_b vs gpio_pin_dir_d)

