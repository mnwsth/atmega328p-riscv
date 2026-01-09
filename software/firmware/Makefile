# Makefile for building firmware

# Toolchain prefix (adjust if your RISC-V toolchain uses a different prefix)
# Try riscv64-unknown-elf- first, fall back to riscv32-unknown-elf-
RISCV_PREFIX ?= $(shell which riscv64-unknown-elf-gcc > /dev/null 2>&1 && echo "riscv64-unknown-elf-" || echo "riscv32-unknown-elf-")

# Compiler and tools
CC = $(RISCV_PREFIX)gcc
AS = $(RISCV_PREFIX)as
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump
SIZE = $(RISCV_PREFIX)size

# Compiler flags
CFLAGS = -march=rv32i -mabi=ilp32 -Wall -Wextra -O2 -ffreestanding -nostdlib
ASFLAGS = -march=rv32i -mabi=ilp32
# Linker flags - pass emulation to linker via -Wl
LDFLAGS = -T linker.ld -nostdlib -nostartfiles -Wl,-melf32lriscv

# Source files (Port B is the default firmware)
SOURCES = blinky_portb.c
ASM_SOURCES = start.S
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Simulation source files for Port B (shorter delay)
SIM_SOURCES = blinky_portb_sim.c
SIM_OBJECTS = $(SIM_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Alternate firmware sources for Port D
PORTD_SOURCES = blinky_portd.c
PORTD_OBJECTS = $(PORTD_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Simulation sources for Port D
PORTD_SIM_SOURCES = blinky_portd_sim.c
PORTD_SIM_OBJECTS = $(PORTD_SIM_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Alternate firmware sources for Port C
PORTC_SOURCES = blinky_portc.c
PORTC_OBJECTS = $(PORTC_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Simulation sources for Port C
PORTC_SIM_SOURCES = blinky_portc_sim.c
PORTC_SIM_OBJECTS = $(PORTC_SIM_SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

# Output files
TARGET = firmware
ELF = $(TARGET).elf
HEX = $(TARGET).hex
BIN = $(TARGET).bin
MEM = $(TARGET).mem
DUMP = $(TARGET).dump

.PHONY: all sim portd portd_sim portc portc_sim clean size dump

all: $(HEX) $(BIN) $(MEM) size

# Simulation build (uses blinky_portb_sim.c with shorter delay)
sim: $(SIM_OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $(ELF) $(SIM_OBJECTS)
	$(OBJDUMP) -d $(ELF) > $(DUMP)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
	$(OBJCOPY) -O binary $(ELF) $(BIN)
	python3 ../../software/tools/hex2mem.py $(HEX) $(MEM)
	$(SIZE) $(ELF)

# Hardware build using Port D firmware
portd: $(PORTD_OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $(ELF) $(PORTD_OBJECTS)
	$(OBJDUMP) -d $(ELF) > $(DUMP)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
	$(OBJCOPY) -O binary $(ELF) $(BIN)
	python3 ../../software/tools/hex2mem.py $(HEX) $(MEM)
	$(SIZE) $(ELF)

# Simulation build using Port D firmware
portd_sim: $(PORTD_SIM_OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $(ELF) $(PORTD_SIM_OBJECTS)
	$(OBJDUMP) -d $(ELF) > $(DUMP)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
	$(OBJCOPY) -O binary $(ELF) $(BIN)
	python3 ../../software/tools/hex2mem.py $(HEX) $(MEM)
	$(SIZE) $(ELF)

$(ELF): $(OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)
	$(OBJDUMP) -d $@ > $(DUMP)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(MEM): $(HEX)
	python3 ../../software/tools/hex2mem.py $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $<

size: $(ELF)
	$(SIZE) $<

dump: $(DUMP)
	@cat $(DUMP)

clean:
	rm -f $(OBJECTS) $(ELF) $(HEX) $(BIN) $(MEM) $(DUMP)

