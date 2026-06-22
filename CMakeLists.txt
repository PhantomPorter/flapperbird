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
find_library(JPEG_LIBRARY NAMES jpeg-turbo jpeg PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)
find_library(PNG_LIBRARY NAMES png PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)
find_library(Z_LIBRARY NAMES z PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)

target_link_libraries(WiiHomebrewApp 
    PRIVATE 
    -Wl,--start-group
    ${WII_PORTLIBS}/libSDLmain.a
    
    /opt/devkitpro/portlibs/wii/lib/libSDL_ttf.a
    
    ${WII_PORTLIBS}/libSDL.a
    
    ${WII_PORTLIBS}/libSDL_image.a
    ${JPEG_LIBRARY}        
    ${PNG_LIBRARY}         
    ${Z_LIBRARY}           
    
    /opt/devkitpro/portlibs/ppc/lib/libfreetype.a    
    -lbrotlidec 
    -lbrotlicommon
    -lbz2

    ${WII_OGCLIBS}/libfat.a
    ${WII_OGCLIBS}/libwiiuse.a
    ${WII_OGCLIBS}/libbte.a
    -lwiikeyboard
    ${WII_OGCLIBS}/libaesnd.a        
    ${WII_OGCLIBS}/libasnd.a         
    ${WII_OGCLIBS}/libogc.a
    -lm
    -Wl,--end-group
)





# Convert the ELF artifact into a Wii-readable DOL executable
ogc_create_dol(WiiHomebrewApp)
