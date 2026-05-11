# Architecture Documentation

## Overview

This project implements a RISC-V based System-on-Chip (SoC) that replicates the functionality of an ATmega328P microcontroller. The design uses the PicoRV32 RISC-V core and implements a memory-mapped peripheral system compatible with ATmega328P register addresses.

## System Architecture

```text
┌─────────────────────────────────────────────────────────────────────────────┐
│                                SoC Top Level                                │
│                                                                             │
│          ┌────────────┐        ┌────────────┐        ┌────────────┐         │
│          │ JTAG Debug ├───────►│  PicoRV32  │◄───────┤  IRQ Ctrl  │         │
│          └────────────┘        │    Core    │        └────────────┘         │
│                                └─────┬──────┘                               │
│                                      │ Memory Bus                           │
│                                      │                                      │
│   ┌──────────────────────────────────▼────────────────────────────────────┐ │
│   │                              Bus Decoder                              │ │
│   └─┬─────────┬─────────┬─────────────┬─────────┬─────────┬────────┬──────┘ │
│     │         │         │             │         │         │        │        │
│   ┌─▼─┐     ┌─▼─┐   ┌───▼────┐    ┌───▼───┐   ┌─▼──┐    ┌─▼─┐    ┌─▼─┐      │
│   │ROM│     │RAM│   │  GPIO  │    │ Timer │   │UART│    │SPI│    │I2C│      │
│   │   │     │   │   │ B,C,D  │    │       │   │    │    │   │    │   │      │
│   └───┘     └───┘   └────────┘    └───────┘   └────┘    └───┘    └───┘      │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```
## Memory Map

| Address Range        | Size  | Description                    |
|---------------------|-------|--------------------------------|
| 0x00000000-0x0000FFFF| 64KB  | Program ROM (Flash equivalent)  |
| 0x10000000-0x10000FFF| 4KB   | Data RAM (SRAM equivalent)      |
| 0x20000000-0x20000FFF| 4KB   | I/O Space (Peripherals)         |

### I/O Register Map

#### Port B Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000023 | PINB     | Port B Input Pins (read-only)        |
| 0x20000024 | DDRB     | Port B Data Direction Register        |
| 0x20000025 | PORTB    | Port B Data Register                  |

#### Port C Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000026 | PINC     | Port C Input Pins (read-only)        |
| 0x20000027 | DDRC     | Port C Data Direction Register        |
| 0x20000028 | PORTC    | Port C Data Register                  |

#### Port D Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000029 | PIND     | Port D Input Pins (read-only)        |
| 0x2000002A | DDRD     | Port D Data Direction Register        |
| 0x2000002B | PORTD    | Port D Data Register                  |

#### Analog Comparator Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000050 | ACSR     | Analog Comparator Control/Status      |

#### Timer/Counter 0 Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000035 | TIFR0    | Timer Interrupt Flag Register         |
| 0x20000044 | TCCR0A   | Timer Control Register A              |
| 0x20000045 | TCCR0B   | Timer Control Register B              |
| 0x20000046 | TCNT0    | Timer Counter Register                |
| 0x20000047 | OCR0A    | Output Compare Register A             |
| 0x20000048 | OCR0B    | Output Compare Register B             |
| 0x2000006E | TIMSK0   | Timer Interrupt Mask Register         |

#### Watchdog Timer Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000054 | MCUSR    | MCU Status Register (WDRF bit)        |
| 0x20000060 | WDTCSR   | Watchdog Timer Control and Status     |
| 0x20000061 | WDR      | Watchdog Reset (write 0xA5 to reset)  |

#### Timer/Counter 2 Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000037 | TIFR2    | Timer Interrupt Flag Register         |
| 0x20000070 | TIMSK2   | Timer Interrupt Mask Register         |
| 0x200000B0 | TCCR2A   | Timer Control Register A              |
| 0x200000B1 | TCCR2B   | Timer Control Register B              |
| 0x200000B2 | TCNT2    | Timer Counter Register                |
| 0x200000B3 | OCR2A    | Output Compare Register A             |
| 0x200000B4 | OCR2B    | Output Compare Register B             |

#### SPI Registers

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x2000004C | SPCR     | SPI Control Register                  |
| 0x2000004D | SPSR     | SPI Status Register                   |
| 0x2000004E | SPDR     | SPI Data Register                     |

## Components

### 1. RISC-V Core (PicoRV32)

- **Source**: Open-source PicoRV32 from YosysHQ
- **ISA**: RV32I (32-bit RISC-V base instruction set)
- **Features**:
  - Single-cycle execution for most instructions
  - Memory-mapped I/O interface
  - Configurable features (counters, interrupts, etc.)

### 2. Bus Decoder

- **Purpose**: Routes memory accesses to appropriate peripherals
- **Functionality**:
  - Address decoding based on memory map
  - Byte-to-word address conversion
  - Read data multiplexing
  - Ready signal generation

### 3. Program ROM

- **Size**: 64KB (16K 32-bit words)
- **Implementation**: FPGA block RAM
- **Initialization**: Loaded from `firmware.mem` file
- **Interface**: Synchronous read with valid signal

### 4. Data RAM

- **Size**: 4KB (1K 32-bit words)
- **Implementation**: FPGA block RAM
- **Features**:
  - Byte-write enable support
  - Read-after-write capability
  - Synchronous read/write

### 5. GPIO Peripheral (Port B, Port C, and Port D)

- **Port B Registers**: PINB, DDRB, PORTB (matching ATmega328P)
- **Port C Registers**: PINC, DDRC, PORTC (matching ATmega328P)
- **Port D Registers**: PIND, DDRD, PORTD (matching ATmega328P)
- **Functionality**:
  - 23 bidirectional I/O pins total (8 + 8 + 7)
  - Configurable direction per pin
  - Read actual pin state (PINB, PINC, PIND)
  - Drive output pins (PORTB, PORTC, PORTD)
  - Independent operation of Port B, Port C, and Port D

### 6. Analog Comparator

- **Register**: ACSR (Analog Comparator Control and Status Register)
- **Functionality**:
  - Compares voltages on AIN0 and AIN1 pins
  - Output (ACO) set when AIN0 > AIN1
  - Interrupt generation on Toggle, Rising Edge, or Falling Edge
  - Interrupt flag (ACI) and enable (ACIE) support
  - Disable capability (ACD) to save power
- **Implementation Notes**:
  - Race condition in interrupt logic has been fixed
  - Clear operations take absolute precedence over interrupt detection
  - Synchronous edge detection eliminates timing issues

### 7. Timer/Counter 0

- **Registers**: TCCR0A, TCCR0B, TCNT0, OCR0A, OCR0B, TIMSK0, TIFR0
- **Functionality**:
  - 8-bit counter with multiple operating modes
  - Prescaler options: 1, 8, 64, 256, 1024, external clock (T0 pin)
  - Waveform Generation Modes:
    - Normal mode (count 0x00 to 0xFF)
    - CTC mode (Clear Timer on Compare Match)
    - Fast PWM mode
    - Phase Correct PWM mode
  - Two Output Compare units (OC0A, OC0B) with configurable behavior:
    - Toggle, Clear, or Set on compare match
    - PWM output generation
  - Interrupt sources:
    - Timer Overflow (TOV0)
    - Compare Match A (OCF0A)
    - Compare Match B (OCF0B)
  - Force Output Compare capability (FOC0A, FOC0B)
- **External Pins**:
  - T0: External clock input
  - OC0A: Output Compare A output
  - OC0B: Output Compare B output
- **Implementation Notes**:
  - Full ATmega328P register compatibility
  - All prescaler modes implemented including external clock
  - 27 comprehensive unit tests verify all functionality

### 8. Watchdog Timer

- **Registers**: MCUSR (WDRF bit), WDTCSR (WDIF, WDIE, WDP3, WDCE, WDE, WDP[2:0])
- **Functionality**:
  - Separate 128kHz oscillator simulation (128 system clock divider)
  - 8 programmable timeout periods (16ms to 8s equivalent)
  - Three operating modes:
    - Interrupt only mode (WDIE=1, WDE=0)
    - System Reset mode (WDIE=0, WDE=1)
    - Interrupt and System Reset mode (WDIE=1, WDE=1)
  - WDR (Watchdog Reset) command via magic value 0xA5
  - Timed sequence for safe WDE/WDP bit changes (32 cycle window)
  - WDCE (Watchdog Change Enable) auto-clear
  - WDRF (Watchdog Reset Flag) in MCUSR
- **WDTCSR Register Bits**:
  - Bit 7: WDIF (Watchdog Interrupt Flag, write-1-to-clear)
  - Bit 6: WDIE (Watchdog Interrupt Enable)
  - Bit 5: WDP3 (Watchdog Prescaler bit 3)
  - Bit 4: WDCE (Watchdog Change Enable)
  - Bit 3: WDE (Watchdog Enable)
  - Bits 2:0: WDP[2:0] (Watchdog Prescaler bits)
- **Timeout Periods** (WDP[3:0]):
  - 0000: 2K cycles (~16ms at 128kHz)
  - 0001: 4K cycles (~32ms)
  - 0010: 8K cycles (~64ms)
  - 0011: 16K cycles (~125ms)
  - 0100: 32K cycles (~250ms)
  - 0101: 64K cycles (~500ms)
  - 0110: 128K cycles (~1s)
  - 0111: 256K cycles (~2s)
  - 1000: 512K cycles (~4s)
  - 1001: 1M cycles (~8s)
- **Implementation Notes**:
  - 21-bit counter and timeout registers to support full 2^20 cycle timeout (WDP=1001)
  - 26 comprehensive unit tests verify all functionality
  - Full integration test with C firmware
  - Timed sequence window extended to 32 cycles for RISC-V compatibility

### 9. Timer/Counter 2

- **Registers**: TCCR2A, TCCR2B, TCNT2, OCR2A, OCR2B, TIMSK2, TIFR2
- **Functionality**:
  - 8-bit counter with multiple operating modes
  - Timer2-specific prescaler options: 1, 8, 32, 64, 128, 256, 1024
  - Waveform Generation Modes:
    - Normal mode (count 0x00 to 0xFF)
    - CTC mode (Clear Timer on Compare Match)
    - Fast PWM mode
    - Phase Correct PWM mode
  - Two Output Compare units (OC2A, OC2B) with configurable behavior:
    - Toggle, Clear, or Set on compare match
    - PWM output generation
  - Interrupt sources:
    - Timer Overflow (TOV2)
    - Compare Match A (OCF2A)
    - Compare Match B (OCF2B)
  - Force Output Compare capability (FOC2A, FOC2B)
- **External Pins**:
  - OC2A: Output Compare A output
  - OC2B: Output Compare B output
- **Implementation Notes**:
  - Full ATmega328P register compatibility
  - Timer2-specific prescaler (includes /32 and /128 options not in Timer0)
  - 33 comprehensive unit tests verify all functionality

### 10. SPI (Serial Peripheral Interface)

- **Registers**: SPCR, SPSR, SPDR
- **Functionality**:
  - Master and Slave modes
  - All 4 SPI modes supported (CPOL/CPHA combinations):
    - Mode 0: CPOL=0, CPHA=0 (sample on rising, shift on falling)
    - Mode 1: CPOL=0, CPHA=1 (sample on falling, shift on rising)
    - Mode 2: CPOL=1, CPHA=0 (sample on falling, shift on rising)
    - Mode 3: CPOL=1, CPHA=1 (sample on rising, shift on falling)
  - 8 clock rate options via SPI2X, SPR1, SPR0:
    - fosc/2, /4, /8, /16, /32, /64, /128
  - MSB or LSB first data order (DORD bit)
  - Interrupt on transfer complete (SPIF flag)
  - Write collision detection (WCOL flag)
- **SPCR Register Bits**:
  - Bit 7: SPIE (SPI Interrupt Enable)
  - Bit 6: SPE (SPI Enable)
  - Bit 5: DORD (Data Order, 0=MSB first, 1=LSB first)
  - Bit 4: MSTR (Master/Slave Select)
  - Bit 3: CPOL (Clock Polarity)
  - Bit 2: CPHA (Clock Phase)
  - Bits 1:0: SPR (SPI Clock Rate Select)
- **SPSR Register Bits**:
  - Bit 7: SPIF (SPI Interrupt Flag, read-only)
  - Bit 6: WCOL (Write Collision Flag, read-only)
  - Bit 0: SPI2X (Double SPI Speed)
- **External Pins**:
  - SCK: Serial Clock (output in Master mode)
  - MOSI: Master Out Slave In (output in Master, input in Slave)
  - MISO: Master In Slave Out (input in Master, output in Slave)
  - SS_n: Slave Select (active low, input in Slave mode)
- **Implementation Notes**:
  - Proper bidirectional data handling for Master/Slave modes
  - Slave mode uses separate mosi_in (receive) and miso_out/miso_oe (transmit) signals
  - SPIF cleared by reading SPSR then SPDR sequence
  - 33 comprehensive unit tests + 8 integration tests

## Bus Protocol

The system uses PicoRV32's memory bus protocol with **word-aligned addressing**:

- **Address**: 32-bit address (bits [1:0] always 00 for word alignment)
- **Write Data**: 32-bit word with 4-bit byte enables (`wstrb`)
- **Read Data**: 32-bit word
- **Control Signals**:
  - `mem_valid`: Request valid
  - `mem_ready`: Response ready
  - `mem_wstrb`: Write strobe (byte enables, one-hot for byte access)

### Word-Aligned GPIO Register Mapping

Since PicoRV32 sends word-aligned addresses, GPIO registers are grouped into 32-bit words:

| Word Address | Byte 0 | Byte 1 | Byte 2 | Byte 3 |
|--------------|--------|--------|--------|--------|
| 0x20000020   | -      | -      | -      | PINB   |
| 0x20000024   | DDRB   | PORTB  | PINC   | DDRC   |
| 0x20000028   | PORTC  | PIND   | DDRD   | PORTD  |

When accessing a byte-addressed register (e.g., PORTB at 0x25):
- CPU sends word address 0x24 with `wstrb=0010`
- GPIO module extracts data from bits [15:8] of the data bus

## Clock and Reset

- **Clock**: External clock input (typically 10-100MHz)
- **Reset**: Active-low asynchronous reset
- **Reset Behavior**: All registers cleared, CPU starts at address 0x00000000

## GPIO Pin Behavior

### Port B
- **DDRB = 0**: Pin configured as input
  - `gpio_pin_out_b` is driven but pin is high-impedance
  - Reading `PINB` returns actual pin state
- **DDRB = 1**: Pin configured as output
  - `gpio_pin_out_b` drives the physical pin
  - Reading `PINB` may return driven value (implementation dependent)

### Port C
- **DDRC = 0**: Pin configured as input
  - `gpio_pin_out_c` is driven but pin is high-impedance
  - Reading `PINC` returns actual pin state
- **DDRC = 1**: Pin configured as output
  - `gpio_pin_out_c` drives the physical pin
  - Reading `PINC` may return driven value (implementation dependent)

### Port D
- **DDRD = 0**: Pin configured as input
  - `gpio_pin_out_d` is driven but pin is high-impedance
  - Reading `PIND` returns actual pin state
- **DDRD = 1**: Pin configured as output
  - `gpio_pin_out_d` drives the physical pin
  - Reading `PIND` may return driven value (implementation dependent)

## Future Enhancements

- Timer/Counter 1 (16-bit timer with input capture)
- USART (Serial communication)
- I2C/TWI (Two-Wire Interface)
- ADC (Analog-to-Digital Converter)
- Interrupt controller
- EEPROM emulation
- Power management (sleep modes)
- Brown-out detection

## Design Notes

1. **Endianness**: The system uses little-endian byte ordering (standard for RISC-V)
2. **Address Alignment**: PicoRV32 uses word-aligned addresses with byte strobes for sub-word access
3. **Memory Timing**: All memories respond in a single cycle (combinational read, registered write)
4. **GPIO Timing**: GPIO registers are synchronous, responding in one clock cycle
5. **Byte Strobes**: `wstrb[3:0]` indicates which byte lane within a word is being accessed

