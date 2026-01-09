# Architecture Documentation

## Overview

This project implements a RISC-V based System-on-Chip (SoC) that replicates the functionality of an ATmega328P microcontroller. The design uses the PicoRV32 RISC-V core and implements a memory-mapped peripheral system compatible with ATmega328P register addresses.

## System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    SoC Top Level                         │
│                                                          │
│  ┌──────────┐                                           │
│  │ PicoRV32 │                                           │
│  │   Core   │                                           │
│  └────┬─────┘                                           │
│       │ Memory Bus                                      │
│       │                                                 │
│  ┌────▼──────────────────────────────────────┐         │
│  │         Bus Decoder                        │         │
│  └────┬──────────┬──────────┬───────────────┘         │
│       │          │          │                          │
│  ┌────▼──┐  ┌───▼───┐  ┌───▼────┐                     │
│  │  ROM  │  │  RAM  │  │  GPIO  │                     │
│  │ 64KB  │  │  4KB  │  │Port B,C,D│                     │
│  └───────┘  └───────┘  └─────────┘                     │
└─────────────────────────────────────────────────────────┘
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

- Timer/Counter peripherals
- UART (Serial communication)
- SPI interface
- I2C (TWI) interface
- ADC (Analog-to-Digital Converter)
- Interrupt controller
- EEPROM emulation

## Design Notes

1. **Endianness**: The system uses little-endian byte ordering (standard for RISC-V)
2. **Address Alignment**: PicoRV32 uses word-aligned addresses with byte strobes for sub-word access
3. **Memory Timing**: All memories respond in a single cycle (combinational read, registered write)
4. **GPIO Timing**: GPIO registers are synchronous, responding in one clock cycle
5. **Byte Strobes**: `wstrb[3:0]` indicates which byte lane within a word is being accessed

