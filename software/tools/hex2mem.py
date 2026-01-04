#!/usr/bin/env python3
"""
Convert Intel HEX file to Verilog memory initialization format
for use with $readmemh in simulation
"""

import sys
import re

def parse_hex_line(line):
    """Parse a line from Intel HEX file"""
    line = line.strip()
    if not line.startswith(':'):
        return None
    
    # Remove ':'
    data = line[1:]
    
    # Extract fields
    byte_count = int(data[0:2], 16)
    address = int(data[2:6], 16)
    record_type = int(data[6:8], 16)
    hex_data = data[8:8+byte_count*2]
    
    # Convert hex data to bytes
    bytes_data = []
    for i in range(0, len(hex_data), 2):
        bytes_data.append(int(hex_data[i:i+2], 16))
    
    return {
        'type': record_type,
        'address': address,
        'data': bytes_data
    }

def hex_to_mem(hex_file, mem_file, word_size=4):
    """Convert Intel HEX to memory file"""
    memory = {}
    
    # Read HEX file
    with open(hex_file, 'r') as f:
        for line in f:
            record = parse_hex_line(line)
            if record is None:
                continue
            
            if record['type'] == 0:  # Data record
                addr = record['address']
                for i, byte_val in enumerate(record['data']):
                    memory[addr + i] = byte_val
            elif record['type'] == 1:  # End of file
                break
    
    # Find max address
    if not memory:
        print("Warning: No data found in HEX file")
        return
    
    max_addr = max(memory.keys())
    # Round up to word boundary
    max_word_addr = (max_addr + word_size - 1) // word_size
    
    # Write memory file (little-endian words)
    with open(mem_file, 'w') as f:
        for word_addr in range(max_word_addr + 1):
            byte_addr = word_addr * word_size
            word = 0
            for i in range(word_size):
                byte_val = memory.get(byte_addr + i, 0)
                word |= (byte_val << (i * 8))
            f.write(f"{word:08x}\n")

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: hex2mem.py <input.hex> <output.mem>")
        sys.exit(1)
    
    hex_file = sys.argv[1]
    mem_file = sys.argv[2]
    
    hex_to_mem(hex_file, mem_file)
    print(f"Converted {hex_file} to {mem_file}")

