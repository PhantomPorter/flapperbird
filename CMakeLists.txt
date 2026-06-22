cmake_minimum_required(VERSION 3.10)
project(WiiHomebrewApp C CXX)

if(NOT DEVKITPRO)
    message(FATAL_ERROR "Please set DEVKITPRO in your environment")
endif()

include(${DEVKITPRO}/cmake/Wii.cmake)

set(WII_PORTLIBS "${DEVKITPRO}/portlibs/wii/lib")
set(WII_OGCLIBS "${DEVKITPRO}/libogc/lib/wii")

include_directories(
    ${DEVKITPRO}/portlibs/wii/include
    ${DEVKITPRO}/portlibs/wii/include/SDL
)

add_executable(WiiHomebrewApp source/main.cpp)

ogc_create_dol(WiiHomebrewApp)

set_target_properties(WiiHomebrewApp PROPERTIES LINKER_LANGUAGE CXX)

find_library(JPEG_LIBRARY NAMES jpeg-turbo jpeg PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)
find_library(PNG_LIBRARY NAMES png PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)
find_library(Z_LIBRARY NAMES z PATHS ${DEVKITPRO}/portlibs/ppc/lib ${DEVKITPRO}/portlibs/wii/lib)

target_link_libraries(WiiHomebrewApp 
    PRIVATE 
    -Wl,--start-group
    ${WII_PORTLIBS}/libSDLmain.a
    
    ${WII_PORTLIBS}/libSDL_ttf.a
    ${DEVKITPRO}/portlibs/ppc/lib/libfreetype.a    
    -lbrotlidec       
    -lbrotlicommon
    -lbz2

    ${WII_PORTLIBS}/libSDL.a
    ${WII_PORTLIBS}/libSDL_image.a
    /opt/devkitpro/portlibs/ppc/lib/libjpeg.a       
    /opt/devkitpro/portlibs/ppc/lib/libpng.a        
    /opt/devkitpro/portlibs/ppc/lib/libz.a          
    
    ${WII_OGCLIBS}/libfat.a
    ${WII_OGCLIBS}/libwiiuse.a
    ${WII_OGCLIBS}/libbte.a
    -lwiikeyboard
    ${WII_OGCLIBS}/libasnd.a         
    ${WII_OGCLIBS}/libaesnd.a         
    ${WII_OGCLIBS}/libogc.a
    -lm
    -lstdc++
    -Wl,--end-group
)
