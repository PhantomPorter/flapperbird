# Wii Cross-Compilation Toolchain Configuration
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "powerpc")

# Define devkitPro Environment Paths
set(DEVKITPRO "/opt/devkitpro" CACHE PATH "devkitPro root directory")
set(DEVKITPPC "${DEVKITPRO}/devkitPPC" CACHE PATH "devkitPPC directory")

# Specify Cross-Compilers
set(CMAKE_C_COMPILER "${DEVKITPPC}/bin/powerpc-eabi-gcc")
set(CMAKE_CXX_COMPILER "${DEVKITPPC}/bin/powerpc-eabi-g++")
set(CMAKE_ASM_COMPILER "${DEVKITPPC}/bin/powerpc-eabi-as")

# Compiler Flags required for the Wii (Gekko Architecture)
set(WII_FLAGS "-mrvl -mcpu=750 -meabi -mhard-float")
set(CMAKE_C_FLAGS "${WII_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${WII_FLAGS}" CACHE STRING "" FORCE)

# System Include Directories
include_directories(
    "${DEVKITPRO}/libogc/include"
    "${DEVKITPRO}/portlibs/wii/include"
)

# Search Paths for Libraries
set(CMAKE_FIND_ROOT_PATH "${DEVKITPPC}" "${DEVKITPRO}/libogc" "${DEVKITPRO}/portlibs/wii")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
