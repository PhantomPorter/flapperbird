cmake_minimum_required(VERSION 3.15)
project(WiiHomebrewApp C CXX)

# Gather all source files
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.c")

# Add executable
add_executable(WiiHomebrewApp ${SOURCES})

# 1. ADD MANDATORY INITIALIZATION COMPILER FLAGS
target_compile_definitions(WiiHomebrewApp PRIVATE -D_WII_7Z_)

# 2. ASSIGN EXTENDED HEADERS SUBDIRECTORIES
target_include_directories(WiiHomebrewApp SYSTEM PRIVATE 
    /opt/devkitpro/portlibs/wii/include
    /opt/devkitpro/portlibs/wii/include/SDL
    /opt/devkitpro/libogc/include
)

# Helper references
set(WII_PORTLIBS "/opt/devkitpro/portlibs/wii/lib")
set(WII_OGCLIBS  "/opt/devkitpro/libogc/lib/wii")

# 3. INTERLOCK LINKER STACK WITH EXTENDED AUDIO SUBSYSTEMS
target_link_libraries(WiiHomebrewApp 
    PRIVATE 
    -Wl,--start-group
    ${WII_PORTLIBS}/libSDLmain.a
    ${WII_PORTLIBS}/libSDL.a
    ${WII_OGCLIBS}/libfat.a
    ${WII_OGCLIBS}/libwiiuse.a
    ${WII_OGCLIBS}/libbte.a
    -lwiikeyboard
    ${WII_OGCLIBS}/libaesnd.a        # <-- Fixes the AESND_ missing references
    ${WII_OGCLIBS}/libasnd.a         # Standard audio helper routines
    ${WII_OGCLIBS}/libogc.a
    -lm
    -Wl,--end-group
)

# Convert the ELF artifact into a Wii-readable DOL executable
ogc_create_dol(WiiHomebrewApp)
