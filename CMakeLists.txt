cmake_minimum_required(VERSION 3.15)
project(WiiHomebrewApp C CXX)

# Gather all source files
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")

# Add executable
add_executable(WiiHomebrewApp ${SOURCES})

# Bind standard Wii system libraries 
target_link_libraries(WiiHomebrewApp 
    PRIVATE 
    wiiuse 
    bte 
    ogc 
    m
)

# Convert the ELF artifact into a Wii-readable DOL executable
ogc_create_dol(WiiHomebrewApp)
