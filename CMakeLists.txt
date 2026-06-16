cmake_minimum_required(VERSION 3.15)
project(WiiHomebrewApp C CXX)

set(CMAKE_EXE_LINKER_FLAGS "-mrvl -mcpu=750 -meabi -mhard-float -Wl,-Map,${PROJECT_NAME}.map")

# Locate all project source files
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")

# Create the standard ELF executable
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Link mandatory Wii system libraries
target_link_libraries(${PROJECT_NAME}.elf 
    PRIVATE 
    wiiuse 
    bte 
    ogc 
    m
)

# Custom Command: Convert the compiled ELF file to a Wii hardware-readable DOL file
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND /opt/devkitpro/tools/bin/elf2dol $<TARGET_FILE:${PROJECT_NAME}.elf> ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dol
    COMMENT "Converting ELF build artifact to Wii DOL format..."
)
