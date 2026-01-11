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
| `tb_bus_decoder.v` | Unit Test | Icarus Verilog | Bus decoder address routing |
| `tb_ram.v` | Unit Test | Icarus Verilog | RAM read/write operations |
| `peripherals/analog_comparator_tb.v` | Unit Test | Icarus Verilog | Analog Comparator peripheral |
| `peripherals/timer0_tb.v` | Unit Test | Icarus Verilog | Timer0 peripheral (27 tests) |
| `tb_soc_timer0.v` | Integration Test | Icarus Verilog | Timer0 SoC integration |
| `peripherals/watchdog_timer_tb.v` | Unit Test | Icarus Verilog | Watchdog Timer peripheral (26 tests) |
| `tb_soc_wdt.v` | Integration Test | Icarus Verilog | Watchdog Timer SoC integration |

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

### 9. Timer0 Unit Test (`peripherals/timer0_tb.v`)

**Location:** `testbench/peripherals/timer0_tb.v`

**Purpose:** Comprehensive unit testing of the Timer/Counter 0 peripheral.

**Test Cases (27 tests):**

| Test # | Description | Verification |
|--------|-------------|--------------|
| 1 | Reset Values | All registers = 0x00 after reset |
| 2 | Register Read/Write | TCCR0A, TCCR0B, TCNT0, OCR0A/B, TIMSK0 |
| 3 | Timer Stopped (CS=000) | TCNT0 doesn't increment |
| 4 | Normal Mode, Prescaler /1 | TCNT0 counts at clock rate |
| 5 | Normal Mode Overflow | TOV0 flag and IRQ set at 0xFF→0x00 |
| 6 | Prescaler /8 | TCNT0 counts at clk/8 |
| 7 | Prescaler /64 | TCNT0 counts at clk/64 |
| 8 | CTC Mode | Counter clears at OCR0A match |
| 9 | Compare Match A Detection | OCF0A flag and IRQ set |
| 10 | Compare Match B Detection | OCF0B flag and IRQ set |
| 11 | OC0A Toggle Mode | OC0A toggles on compare match |
| 12 | OC0A Clear Mode | OC0A clears on compare match |
| 13 | OC0A Set Mode | OC0A sets on compare match |
| 14 | Fast PWM Mode | TOV0 set at TOP |
| 15 | Fast PWM with OCR0A as TOP | Counter wraps at OCR0A |
| 16 | External Clock Falling Edge | TCNT0 increments on T0 falling edge |
| 17 | External Clock Rising Edge | TCNT0 increments on T0 rising edge |
| 18 | Interrupt Mask/Enable | IRQ only when TOIE0=1 |
| 19 | Write TCNT0 While Running | Direct counter update works |
| 20 | Force Output Compare | FOC0A toggles OC0A in non-PWM mode |
| 21 | Multiple Flags Set | All flags can be set simultaneously |
| 22 | OC0B Toggle Mode | OC0B toggles on compare match |
| 23 | Phase Correct PWM Mode | TOV0 set at BOTTOM |
| 24 | Prescaler /256 | TCNT0 counts at clk/256 |
| 25 | Prescaler /1024 | TCNT0 counts at clk/1024 |
| 26 | CTC Mode - TOV0 not set (OCR0A < 0xFF) | TOV0 remains cleared |
| 27 | CTC Mode - TOV0 set (OCR0A = 0xFF) | TOV0 set when TOP=MAX |

**Timer0 Register Bits:**

**TCCR0A:**
- Bits 7:6: COM0A (Compare Output Mode A)
- Bits 5:4: COM0B (Compare Output Mode B)
- Bits 1:0: WGM0[1:0] (Waveform Generation Mode)

**TCCR0B:**
- Bit 7: FOC0A (Force Output Compare A)
- Bit 6: FOC0B (Force Output Compare B)
- Bit 3: WGM02 (Waveform Generation Mode bit 2)
- Bits 2:0: CS0 (Clock Select)

**How to Run:**
```bash
cd testbench
make timer0_unit
```

**Expected Output:**
```
=== Test 1: Reset Values ===
PASS: TCCR0A reset = 00000000
PASS: TCCR0B reset = 00000000
...
=== Test 25: Prescaler /1024 ===
PASS: TCNT0 with /1024 = 10

=== Test 26: CTC Mode - TOV0 not set (OCR0A < 0xFF) ===
PASS: TOV0 not set in CTC mode (OCR0A < 0xFF)

=== Test 27: CTC Mode - TOV0 set (OCR0A = 0xFF) ===
PASS: TOV0 set in CTC mode (OCR0A = 0xFF)
========================================
Test Summary: 27 tests, 0 errors
ALL TESTS PASSED!
========================================
```

---

### 10. Timer0 Integration Test (`tb_soc_timer0.v`)

**Location:** `testbench/tb_soc_timer0.v`

**Purpose:** Tests Timer0 integration with the full SoC using test firmware.

**What it Tests:**
- Timer0 register access from CPU
- Timer counting with prescaler
- Overflow detection
- Compare match detection
- CTC mode operation

**Requirements:**
- Requires `timer0_test.c` firmware compiled and loaded

**How to Run:**
```bash
cd testbench
make timer0_sim
```

**Expected Output:**
```
Timer0 Integration Test Started
Time 1000: TEST 0 PASSED - Register access
Time 2000: TEST 1 PASSED - Normal mode counting
Time 3000: TEST 2 PASSED - Overflow detection
...
*** ALL TESTS PASSED! ***
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

### 11. Watchdog Timer Unit Test (`peripherals/watchdog_timer_tb.v`)

**Location:** `testbench/peripherals/watchdog_timer_tb.v`

**Purpose:** Comprehensive unit testing of the Watchdog Timer peripheral.

**Test Cases (26 tests):**

| Test # | Description | Verification |
|--------|-------------|--------------|
| 1 | Reset Values | WDTCSR = 0x00, MCUSR = 0x00 after reset |
| 2 | WDTCSR Read/Write | WDIE bit can be set |
| 3 | WDTCSR WDE | WDE bit can be set |
| 4 | MCUSR Read | MCUSR returns correct value |
| 5 | Prescaler WDP3 | WDP3 bit can be set via timed sequence |
| 6 | Prescaler WDP[2:0] | WDP[2:0] bits can be set via timed sequence |
| 7 | IRQ before timeout | IRQ is 0 before timeout occurs |
| 8 | WDIF set on timeout | WDIF flag set when timer overflows |
| 9 | IRQ after timeout | IRQ asserted when WDIF is set |
| 10 | No reset in INT mode | Reset request not generated in interrupt mode |
| 11 | IRQ cleared | WDIF clears when written with 1 |
| 12 | Reset request | Reset request generated in system reset mode |
| 13 | WDRF set | WDRF flag set on watchdog reset |
| 14 | WDRF cleared | WDRF flag can be cleared by writing 0 |
| 15 | INT+RST: IRQ on 1st | First timeout generates interrupt in combined mode |
| 16 | WDIE auto-cleared | WDIE auto-clears after first timeout in combined mode |
| 17 | No reset on 1st | No reset on first timeout in combined mode |
| 18 | Reset on 2nd | Reset generated on second timeout in combined mode |
| 19 | WDR no IRQ at 1.5x | WDR command prevents timeout |
| 20 | WDR + full timeout | IRQ fires after WDR + full timeout period |
| 21 | WDE not cleared w/o seq | WDE cannot be cleared outside timed sequence |
| 22 | WDE cleared with seq | WDE can be cleared during timed sequence |
| 23 | WDCE auto-cleared | WDCE auto-clears after 32 cycles |

**WDTCSR Register Bits:**
- Bit 7: WDIF (Watchdog Interrupt Flag, write-1-to-clear)
- Bit 6: WDIE (Watchdog Interrupt Enable)
- Bit 5: WDP3 (Watchdog Prescaler bit 3)
- Bit 4: WDCE (Watchdog Change Enable)
- Bit 3: WDE (Watchdog Enable)
- Bits 2:0: WDP[2:0] (Watchdog Prescaler bits)

**MCUSR Register Bits:**
- Bit 3: WDRF (Watchdog Reset Flag)

**How to Run:**
```bash
cd testbench
make wdt_unit
```

**Expected Output:**
```
========================================
Watchdog Timer Unit Tests
========================================
Test 1: Reset Values
  PASS: TCSR reset value
  PASS: CUSR reset value
...
Test 10: WDCE Auto-clear
  PASS: DCE auto-cleared

========================================
Test Summary
========================================
Total Tests:  26
Passed:       26
Failed:       0
========================================
ALL TESTS PASSED!
```

---

### 12. Watchdog Timer Integration Test (`tb_soc_wdt.v`)

**Location:** `testbench/tb_soc_wdt.v`

**Purpose:** Tests Watchdog Timer integration with the full SoC using test firmware.

**What it Tests:**
- WDT register access from CPU
- WDR command functionality
- Interrupt generation on timeout
- WDIF flag detection
- Timed sequence for WDE/WDP changes

**Requirements:**
- Requires `wdt_test.c` firmware compiled and loaded

**How to Run:**
```bash
cd testbench
make wdt_sim
```

**Expected Output:**
```
========================================
Watchdog Timer Integration Test
========================================
Starting SoC with WDT test firmware...
[174] GPIO_B changed: 0x00 -> 0x01
[393283] GPIO_B changed: 0x01 -> 0x03
[655487] GPIO_B changed: 0x03 -> 0x07
...
[659452] *** TEST PASSED ***
[659452] GPIO_B changed: 0xbf -> 0xaa

========================================
Integration Test Summary
========================================
Total cycles: 2000000
RESULT: PASSED
```

**GPIO_B Test Stage Indicators:**
- 0x01: WDT register access OK
- 0x03: WDR command works
- 0x07: WDT interrupt fires
- 0x0F: WDIF flag set correctly
- 0xAA: All tests passed
- 0x55: Test failed

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
├── tb_soc_timer0.v               # Timer0 integration test
├── tb_soc_wdt.v                  # Watchdog Timer integration test
├── tb_bus_decoder.v              # Bus decoder unit tests
├── tb_ram.v                      # RAM unit tests
├── peripherals/
│   ├── analog_comparator_tb.v    # Analog Comparator unit test
│   ├── timer0_tb.v               # Timer0 unit test (27 tests)
│   └── watchdog_timer_tb.v       # Watchdog Timer unit test (26 tests)
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
| 0x20000035 | TIFR0 | Timer0 Interrupt Flag Register |
| 0x20000044 | TCCR0A | Timer0 Control Register A |
| 0x20000045 | TCCR0B | Timer0 Control Register B |
| 0x20000046 | TCNT0 | Timer0 Counter Register |
| 0x20000047 | OCR0A | Timer0 Output Compare A |
| 0x20000048 | OCR0B | Timer0 Output Compare B |
| 0x20000050 | ACSR | Analog Comparator Control/Status |
| 0x2000006E | TIMSK0 | Timer0 Interrupt Mask Register |
| 0x20000054 | MCUSR | MCU Status Register (WDRF bit) |
| 0x20000060 | WDTCSR | Watchdog Timer Control/Status |
| 0x20000061 | WDR | Watchdog Reset (write 0xA5) |

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
