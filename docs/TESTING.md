# Test Documentation

This document provides comprehensive documentation of all tests available in the ATmega328P RISC-V Replica project, including how to run them and what they verify.

## Overview

The project includes multiple testbenches to verify different aspects of the SoC:

| Testbench | Type | Simulator | Description |
|-----------|------|-----------|-------------|
| `tb_soc.v` | System Integration | Icarus Verilog | Full SoC test with firmware |
| `tb_soc_verilator.cpp` | System Integration | Verilator | Full SoC test (faster execution) |
| `tb_gpio_portb.v` | Unit Test | Icarus Verilog | GPIO Port B register tests |
| `tb_gpio_portc.v` | Unit Test | Icarus Verilog | GPIO Port C register tests |
| `tb_gpio_portd.v` | Unit Test | Icarus Verilog | GPIO Port D register tests |
| `tb_soc_input.v` | Integration Test | Icarus Verilog | GPIO input functionality |
| `tb_soc_ac.v` | Integration Test | Icarus Verilog | Analog Comparator integration |
| `peripherals/analog_comparator_tb.v` | Unit Test | Icarus Verilog | Analog Comparator peripheral |

---

## Running Tests

### Prerequisites

Before running tests, ensure you have the required tools installed:

**Ubuntu/Debian:**
```bash
sudo apt-get install gcc-riscv64-unknown-elf verilator iverilog
```

**macOS (Homebrew):**
```bash
brew install riscv-gnu-toolchain verilator icarus-verilog
```

### Quick Start

**Run the main system test (Verilator - recommended, faster):**
```bash
cd testbench
make -f Makefile.verilator sim
```

**Run the main system test (Icarus Verilog):**
```bash
cd testbench
make sim
```

**Or from the project root:**
```bash
make sim
```

---

## Test Descriptions

### 1. Main System Testbench (`tb_soc.v`)

**Location:** `testbench/tb_soc.v`

**Purpose:** Verifies the complete SoC running actual firmware.

**What it Tests:**
- CPU boot sequence after reset
- Program ROM initialization and execution
- GPIO Port B, C, and D output functionality
- Bus protocol and memory timing
- CPU trap detection

**Simulation Parameters:**
- Clock: 10MHz (100ns period)
- Reset Duration: 1µs
- Simulation Duration: 10ms (100,000 cycles)

**Expected Output:**
```
Reset released at time 1000
Time 5000: GPIO_PORTB = 0x01, GPIO_DIR_B = 0x01
Time 8000: GPIO_PORTB = 0x00, GPIO_DIR_B = 0x01
...
Simulation complete
```

**How to Run:**
```bash
cd testbench
iverilog -o tb_soc.vvp -I../rtl ../rtl/core/picorv32.v ../rtl/memory/rom.v \
  ../rtl/memory/ram.v ../rtl/peripherals/gpio.v ../rtl/peripherals/analog_comparator.v \
  ../rtl/bus/bus_decoder.v ../rtl/soc_top.v tb_soc.v
vvp tb_soc.vvp
```

Or simply:
```bash
cd testbench && make sim
```

---

### 2. Verilator System Testbench (`tb_soc_verilator.cpp`)

**Location:** `testbench/tb_soc_verilator.cpp`

**Purpose:** High-performance C++ testbench for faster simulation.

**What it Tests:**
- Same functionality as `tb_soc.v`
- GPIO Port B and Port D toggle detection
- Faster execution (1,000,000 iterations)

**Features:**
- VCD waveform generation
- GPIO toggle counting
- Periodic sampling output

**Simulation Parameters:**
- Maximum iterations: 1,000,000 (500,000 clock cycles)
- Early termination: After 10 GPIO Port B toggles

**Expected Output:**
```
Reset released at time 20
Sample at cycle 51: GPIO_PORTB=0x0, GPIO_DIR_B=0x0, GPIO_PORTD=0x0, GPIO_DIR_D=0x0
Time 3000 (cycle 1500): GPIO_PORTB = 0x1, GPIO_DIR_B = 0x1
...
Simulation complete. GPIO Port B toggled 10 times.
GPIO Port D toggled 0 times.
```

**How to Run:**
```bash
cd testbench
make -f Makefile.verilator sim
```

---

### 3. GPIO Port B Unit Test (`tb_gpio_portb.v`)

**Location:** `testbench/tb_gpio_portb.v`

**Purpose:** Comprehensive register-level testing of GPIO Port B.

**Test Cases:**

| Test # | Description | Expected Result |
|--------|-------------|-----------------|
| 1 | Read DDRB after reset | 0x00 |
| 2 | Read PORTB after reset | 0x00 |
| 3 | Read PINB (input register) | Reflects external pin state |
| 4 | Write DDRB = 0xFF (all outputs) | DDRB reads back 0xFF |
| 5 | Write PORTB = 0x55 | PORTB reads back 0x55 |
| 6 | Write PORTB = 0xAA | PORTB reads back 0xAA |
| 7 | Configure mixed I/O (DDRB = 0x0F) | Lower 4 bits are outputs |
| 8 | Verify PINB reads input | Reflects gpio_pin_in_b |
| 9 | Verify Port C and D unaffected | Registers remain 0x00 |
| 10 | Verify PINB is read-only | Writes are ignored |

**How to Run:**
```bash
cd testbench
iverilog -o tb_gpio_portb.vvp -I../rtl ../rtl/peripherals/gpio.v tb_gpio_portb.v
vvp tb_gpio_portb.vvp
```

**Expected Output:**
```
========================================
GPIO Port B Test Suite
========================================

Test 1: Read DDRB after reset
PASS: DDRB = 0x00
...
========================================
ALL TESTS PASSED!
========================================
```

---

### 4. GPIO Port C Unit Test (`tb_gpio_portc.v`)

**Location:** `testbench/tb_gpio_portc.v`

**Purpose:** Comprehensive register-level testing of GPIO Port C.

**Test Cases:**

| Test # | Description | Expected Result |
|--------|-------------|-----------------|
| 1 | Read DDRC after reset | 0x00 |
| 2 | Read PORTC after reset | 0x00 |
| 3 | Read PINC (input register) | Reflects external pin state |
| 4 | Write DDRC = 0xFF (all outputs) | DDRC reads back 0xFF |
| 5 | Write PORTC = 0x55 | PORTC reads back 0x55 |
| 6 | Write PORTC = 0xAA | PORTC reads back 0xAA |
| 7 | Configure mixed I/O (DDRC = 0x0F) | Lower 4 bits are outputs |
| 8 | Verify PINC reads input | Reflects gpio_pin_in_c |
| 9 | Verify Port B and D unaffected | Registers remain 0x00 |
| 10 | Verify PINC is read-only | Writes are ignored |

**How to Run:**
```bash
cd testbench
iverilog -o tb_gpio_portc.vvp -I../rtl ../rtl/peripherals/gpio.v tb_gpio_portc.v
vvp tb_gpio_portc.vvp
```

---

### 5. GPIO Port D Unit Test (`tb_gpio_portd.v`)

**Location:** `testbench/tb_gpio_portd.v`

**Purpose:** Comprehensive register-level testing of GPIO Port D.

**Test Cases:**

| Test # | Description | Expected Result |
|--------|-------------|-----------------|
| 1 | Read DDRD after reset | 0x00 |
| 2 | Read PORTD after reset | 0x00 |
| 3 | Read PIND (input register) | Reflects external pin state |
| 4 | Write DDRD = 0xFF (all outputs) | DDRD reads back 0xFF |
| 5 | Write PORTD = 0x55 | PORTD reads back 0x55 |
| 6 | Write PORTD = 0xAA | PORTD reads back 0xAA |
| 7 | Configure mixed I/O (DDRD = 0x0F) | Lower 4 bits are outputs |
| 8 | Verify PIND reads input | Reflects gpio_pin_in_d |
| 9 | Verify Port B is unaffected | Registers remain 0x00 |
| 10 | Verify PIND is read-only | Writes are ignored |

**How to Run:**
```bash
cd testbench
iverilog -o tb_gpio_portd.vvp -I../rtl ../rtl/peripherals/gpio.v tb_gpio_portd.v
vvp tb_gpio_portd.vvp
```

---

### 6. GPIO Input Test (`tb_soc_input.v`)

**Location:** `testbench/tb_soc_input.v`

**Purpose:** Verifies GPIO input functionality with firmware that reads PINC and writes to PORTB.

**What it Tests:**
- Firmware can read external GPIO inputs
- Input values are correctly propagated to output
- GPIO pin direction configuration works

**Test Sequence:**
1. Drive PC0 Low → Expect PB0 Low
2. Drive PC0 High → Expect PB0 High
3. Drive PC0 Low → Expect PB0 Low

**Requirements:**
- Requires `gpio_test_input.c` firmware compiled and loaded

**How to Run:**
```bash
cd testbench
# First, build the input test firmware:
# cd ../software/firmware && make gpio_test_input
# cp firmware.mem ../../testbench/

iverilog -o tb_soc_input.vvp -I../rtl ../rtl/core/picorv32.v ../rtl/memory/rom.v \
  ../rtl/memory/ram.v ../rtl/peripherals/gpio.v ../rtl/peripherals/analog_comparator.v \
  ../rtl/bus/bus_decoder.v ../rtl/soc_top.v tb_soc_input.v
vvp tb_soc_input.vvp
```

---

### 7. Analog Comparator Integration Test (`tb_soc_ac.v`)

**Location:** `testbench/tb_soc_ac.v`

**Purpose:** Tests the Analog Comparator peripheral integration with the SoC.

**Test Cases:**

| Test | Description | Verification |
|------|-------------|--------------|
| 1 | AIN0 > AIN1 | ACO = 1 |
| 2 | AIN0 < AIN1 | ACO = 0 |
| 3 | IRQ generation | Force ACIE=1, verify IRQ triggers on toggle |
| 4 | Bus aliasing check | Verify address decoding is correct |

**What it Tests:**
- Analog comparator output (ACO) responds to input changes
- IRQ signal propagates to CPU
- Address decoding prevents bus aliasing between AC and GPIO

**How to Run:**
```bash
cd testbench
iverilog -o tb_soc_ac.vvp -I../rtl ../rtl/core/picorv32.v ../rtl/memory/rom.v \
  ../rtl/memory/ram.v ../rtl/peripherals/gpio.v ../rtl/peripherals/analog_comparator.v \
  ../rtl/bus/bus_decoder.v ../rtl/soc_top.v tb_soc_ac.v
vvp tb_soc_ac.vvp
```

**Expected Output:**
```
Reset released at time 1000
Starting Analog Comparator Integration Test
AIN0=1, AIN1=0. Checking internal ACO...
PASS: Internal ACO is 1
AIN0=0, AIN1=1. Checking internal ACO...
PASS: Internal ACO is 0
...
Integration Test Complete
```

---

### 8. Analog Comparator Unit Test (`peripherals/analog_comparator_tb.v`)

**Location:** `testbench/peripherals/analog_comparator_tb.v`

**Purpose:** Comprehensive unit testing of the Analog Comparator peripheral.

**Test Cases:**

| Test # | Description | Verification |
|--------|-------------|--------------|
| 0 | Reset Values | ACSR = 0x00 after reset |
| 1 | Basic Comparison (AIN0 > AIN1) | ACO = 1 |
| 2 | Basic Comparison (AIN0 < AIN1) | ACO = 0 |
| 3 | Interrupt - Toggle Mode | IRQ triggers on ACO toggle |
| 4 | Interrupt - Rising Edge Mode | IRQ only triggers on rising edge |
| 5 | Disable Comparator | ACO = 0 when ACD = 1 |
| 6 | Bit Read/Write | ACBG, ACIE, ACIC, ACIS bits work correctly |
| 7 | Read-Only Bits | ACO cannot be written |

**ACSR Register Bits:**
- Bit 7: ACD (Analog Comparator Disable)
- Bit 6: ACBG (Analog Comparator Bandgap Select)
- Bit 5: ACO (Analog Comparator Output, read-only)
- Bit 4: ACI (Analog Comparator Interrupt Flag)
- Bit 3: ACIE (Analog Comparator Interrupt Enable)
- Bit 2: ACIC (Analog Comparator Input Capture)
- Bits 1:0: ACIS (Analog Comparator Interrupt Select)

**How to Run:**
```bash
cd testbench/peripherals
iverilog -o analog_comparator_tb.vvp -I../../rtl ../../rtl/peripherals/analog_comparator.v analog_comparator_tb.v
vvp analog_comparator_tb.vvp
```

**Expected Output:**
```
Test 0: Reset Values
Pass: Reset value is 0
Test 1: Basic Comparison (AIN0 > AIN1)
Pass: ACO is 1
Test 2: Basic Comparison (AIN0 < AIN1)
Pass: ACO is 0
Test 3: Interrupt Generation (Toggle Mode)
Pass: IRQ triggered on toggle
Pass: IRQ cleared
...
All tests completed
```

---

## Viewing Waveforms

All testbenches generate VCD (Value Change Dump) files for waveform viewing.

### Linux (GTKWave)

```bash
cd testbench
make view
# Or directly:
gtkwave tb_soc.vcd &
```

### macOS (NovyWave - Recommended)

1. Install NovyWave from [GitHub Releases](https://github.com/NovyWave/NovyWave/releases)
2. Run the simulation to generate VCD files
3. Use the Makefile target:
   ```bash
   cd testbench
   make view
   ```
4. In NovyWave:
   - Click "Load Files"
   - Navigate to `tmp` → `vcs_files`
   - Select `tb_soc.vcd`

**Note:** NovyWave's file picker doesn't show `/Users` on macOS. The Makefile copies VCD files to `/tmp/vcs_files/` to work around this.

**Cleanup:** `rm -rf /tmp/vcs_files`

---

## Test File Structure

```
testbench/
├── Makefile                      # Icarus Verilog simulation
├── Makefile.verilator            # Verilator simulation
├── tb_soc.v                      # Main SoC testbench (iverilog)
├── tb_soc_verilator.cpp          # Main SoC testbench (Verilator)
├── tb_gpio_portb.v               # GPIO Port B unit tests
├── tb_gpio_portc.v               # GPIO Port C unit tests
├── tb_gpio_portd.v               # GPIO Port D unit tests
├── tb_soc_input.v                # GPIO input integration test
├── tb_soc_ac.v                   # Analog Comparator integration test
├── peripherals/
│   └── analog_comparator_tb.v    # Analog Comparator unit test
└── obj_dir/                      # Verilator build output
```

---

## Memory Map Reference

Tests use the following memory-mapped register addresses:

| Address | Register | Description |
|---------|----------|-------------|
| 0x20000023 | PINB | Port B Input Pins |
| 0x20000024 | DDRB | Port B Data Direction |
| 0x20000025 | PORTB | Port B Data Register |
| 0x20000026 | PINC | Port C Input Pins |
| 0x20000027 | DDRC | Port C Data Direction |
| 0x20000028 | PORTC | Port C Data Register |
| 0x20000029 | PIND | Port D Input Pins |
| 0x2000002A | DDRD | Port D Data Direction |
| 0x2000002B | PORTD | Port D Data Register |
| 0x20000050 | ACSR | Analog Comparator Control/Status |

---

## Troubleshooting

### "No module named gpio/analog_comparator"
Ensure you're running from the `testbench/` directory and include paths are correct.

### "firmware.mem not found"
Build the firmware first:
```bash
cd software/firmware && make sim
cp firmware.mem ../../testbench/
```

### Tests show no GPIO activity
- Verify firmware.mem is in the testbench directory
- Check that firmware uses simulation-friendly delays (`blinky_*_sim.c`)
- Ensure ROM module is reading the file correctly

### Verilator compilation errors
- Ensure Verilator 5.x is installed
- Check for missing include files
- Try `make -f Makefile.verilator clean sim`

---

## Adding New Tests

To add a new testbench:

1. Create a new `.v` file in `testbench/`
2. Include the memory map: `` `include "../rtl/bus/memory_map.vh" ``
3. Use the word-aligned bus protocol for register access
4. Add `$dumpfile` and `$dumpvars` for waveform generation
5. Use `$display` for test output

Example template:
```verilog
`timescale 1ns / 1ps
`include "../rtl/bus/memory_map.vh"

module tb_my_test;
    // Clock and reset
    reg clk;
    reg rst_n;
    
    // DUT instance
    // ...
    
    // Clock generation
    initial begin
        clk = 0;
        forever #50 clk = ~clk;
    end
    
    // Test sequence
    initial begin
        $dumpfile("tb_my_test.vcd");
        $dumpvars(0, tb_my_test);
        
        rst_n = 0;
        #1000;
        rst_n = 1;
        
        // Your tests here
        
        $finish;
    end
endmodule
```

---

## Continuous Integration

For CI/CD pipelines, tests can be run with:

```bash
# Run all tests and check for failures
cd testbench
make sim 2>&1 | tee sim.log
grep -q "ERROR" sim.log && exit 1 || exit 0
```

Or for Verilator:
```bash
cd testbench
make -f Makefile.verilator sim 2>&1 | tee sim.log
grep -q "ERROR" sim.log && exit 1 || exit 0
```
