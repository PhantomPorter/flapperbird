# Flappy Bird for the final release of the Nintendo Revolution

A Flappy bird clone for the Nintendo Wii

<img width="5570" height="3420" alt="Screenshot_20260622_204509" src="https://github.com/user-attachments/assets/9e38e712-080e-4121-a543-bc72d456ec24" />


# How it works
This game has Music, score, and random offsetted pipes. Using SDL to simplify the implemtation of these game elements.


# How to play
Play here: https://github.com/PhantomPorter/flapperbird/releases/tag/v1.0.0
Download the files and either

Boot in dolphin by opening the boot.dol in the same folder as the .elf

OR

Put the files in the apps folder in a usb/sdcard and launch from the homebrew channel.

Controls: A/B/Shake to flap, 2 to quit

# Credits
Devkitpro's SDL 1.2 port for Wii
The music used is Akumajou Densetsu - Beginning

# Dependencies

devkitPPC
wii-cmake
libogc
wii-sdl
wii-sdl_image
wii-sdl_ttf
ppc-freetype
ppc-libjpeg-turbo
ppc-libpng
ppc-zlib
ppc-bzip2
ppc-brotli

Install command for everything:
dkp-pacman -S devkitPPC wii-cmake libogc wii-sdl wii-sdl_image wii-sdl_ttf ppc-freetype ppc-libjpeg-turbo ppc-libpng ppc-bzip2 ppc-brotli

Config: env DEVKITPRO="/opt/devkitpro" DEVKITPPC="/opt/devkitpro/devkitPPC" /opt/devkitpro/portlibs/wii/bin/powerpc-eabi-cmake -B build -S .
Build: cmake --build build
