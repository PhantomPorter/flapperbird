# Flappy Bird for the final release of the Nintendo Revolution

A Flappy bird clone for the Nintendo Wii

<img width="5570" height="3420" alt="Screenshot_20260622_204509" src="https://github.com/user-attachments/assets/9e38e712-080e-4121-a543-bc72d456ec24" />
Video: https://github.com/user-attachments/assets/a20eb82a-2450-4bdd-9296-70b1277c9081


# How it works
This game has Music, score, and random offsetted pipes. Using SDL to simplify the implemtation of these game elements.


# How to play
Play here: https://github.com/PhantomPorter/flapperbird/releases/tag/v1.0.0

Download the files from the releases and either

Boot in [dolphin-emulator](https://dolphin-emu.org/download/) by opening the boot.dol in the same folder as the .elf
<img width="1158" height="468" alt="image" src="https://github.com/user-attachments/assets/a07d95f7-dbeb-4f30-86d0-753a15db699a" />

<img width="717" height="133" alt="image" src="https://github.com/user-attachments/assets/0304fe71-28ad-48eb-a6e8-b9fd019ae365" />

OR

If you have a Homebrewed Wii Put the files in the apps folder like below in a usb/sdcard and launch from the homebrew channel.

SD CARD ROOT/apps/WiiHomebrewApp/boot.dol & WiiHomebrewApp.elf

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
