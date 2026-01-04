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
│  │ 64KB  │  │  4KB  │  │ Port B │                     │
│  └───────┘  └───────┘  └─────────┘                     │
└─────────────────────────────────────────────────────────┘
```

## Memory Map

| Address Range        | Size  | Description                    |
|---------------------|-------|--------------------------------|
| 0x00000000-0x0000FFFF| 64KB  | Program ROM (Flash equivalent)  |
| 0x10000000-0x10000FFF| 4KB   | Data RAM (SRAM equivalent)      |
| 0x20000000-0x20000FFF| 4KB   | I/O Space (Peripherals)         |

### I/O Register Map (Port B)

| Address    | Register | Description                          |
|------------|----------|--------------------------------------|
| 0x20000023 | PINB     | Port B Input Pins (read-only)        |
| 0x20000024 | DDRB     | Port B Data Direction Register        |
| 0x20000025 | PORTB    | Port B Data Register                  |

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

### 5. GPIO Peripheral (Port B)

- **Registers**: PINB, DDRB, PORTB (matching ATmega328P)
- **Functionality**:
  - 8-bit bidirectional I/O port
  - Configurable direction per pin
  - Read actual pin state (PINB)
  - Drive output pins (PORTB)

## Bus Protocol

The system uses a simple memory-mapped bus protocol:

- **Address**: 32-bit byte address
- **Write Data**: 32-bit word with 4-bit byte enables
- **Read Data**: 32-bit word
- **Control Signals**:
  - `mem_valid`: Request valid
  - `mem_ready`: Response ready
  - `mem_wstrb`: Write strobe (byte enables)

## Clock and Reset

- **Clock**: External clock input (typically 10-100MHz)
- **Reset**: Active-low asynchronous reset
- **Reset Behavior**: All registers cleared, CPU starts at address 0x00000000

## GPIO Pin Behavior

- **DDRB = 0**: Pin configured as input
  - `gpio_pin_out` is driven but pin is high-impedance
  - Reading `PINB` returns actual pin state
- **DDRB = 1**: Pin configured as output
  - `gpio_pin_out` drives the physical pin
  - Reading `PINB` may return driven value (implementation dependent)

## Future Enhancements

- Additional GPIO ports (Port C, Port D)
- Timer/Counter peripherals
- UART (Serial communication)
- SPI interface
- I2C (TWI) interface
- ADC (Analog-to-Digital Converter)
- Interrupt controller
- EEPROM emulation

## Design Notes

1. **Endianness**: The system uses little-endian byte ordering (standard for RISC-V)
2. **Address Alignment**: All memory accesses are word-aligned (4-byte boundaries)
3. **Memory Timing**: All memories respond in a single cycle (combinational read, registered write)
4. **GPIO Timing**: GPIO registers are synchronous, responding in one clock cycle

