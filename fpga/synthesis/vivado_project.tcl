# Vivado TCL script to create project for ATmega328P RISC-V Replica
# Usage: vivado -mode batch -source vivado_project.tcl

# Project settings
set project_name "atmega328p_riscv"
set project_dir "./vivado_project"
set part_name "xc7a35ticsg324-1L"  # Arty A7-35T, adjust for your board

# Create project
create_project $project_name $project_dir -part $part_name -force

# Add RTL sources
add_files -fileset sources_1 {
    ../../rtl/core/picorv32.v
    ../../rtl/memory/rom.v
    ../../rtl/memory/ram.v
    ../../rtl/peripherals/gpio.v
    ../../rtl/bus/bus_decoder.v
    ../../rtl/soc_top.v
}

# Add constraints
add_files -fileset constrs_1 {
    ../constraints/arty_a7.xdc
}

# Set top module
set_property top soc_top [current_fileset]

# Set ROM initialization file
# Note: You need to build firmware first and copy firmware.mem to this directory
set_property INIT_FILE "../../software/firmware/firmware.mem" [get_cells -hierarchical -filter {NAME =~ "*program_rom*mem*"}]

# Update compile order
update_compile_order -fileset sources_1

# Run synthesis
launch_runs synth_1 -jobs 4
wait_on_run synth_1

# Run implementation
launch_runs impl_1 -jobs 4
wait_on_run impl_1

# Generate bitstream
launch_runs impl_1 -to_step write_bitstream -jobs 4
wait_on_run impl_1

puts "Project created and bitstream generated successfully!"

