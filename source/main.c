#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    16

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Joystick* wii_remote = NULL;
    if (SDL_NumJoysticks() > 0) {
        wii_remote = SDL_JoystickOpen(0);
        if (wii_remote) {
            printf("Connected to Controller: %s\n", SDL_JoystickName(0));
        }
    }

    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (screen == NULL) {
        SDL_Quit();
        return -1;
    }

    int flags = IMG_INIT_JPG;
    if ((IMG_Init(flags) & flags) != flags) {
        printf("IMG_Init: Failed to init required image support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }

    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;
    loadedImage = IMG_Load("back.jpg");

    if (loadedImage != NULL) {
        // Convert to the display format for fast blitting
        optimizedImage = SDL_DisplayFormat(loadedImage);
        // Free the original loaded surface
        SDL_FreeSurface(loadedImage);
    } else {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        // Fallback: If image fails to load, game will just use standard FillRect colors
    }

    Uint32 sky_blue = SDL_MapRGB(screen->format, 135, 206, 235);
    Uint32 bird_yellow = SDL_MapRGB(screen->format, 255, 225, 0);
    Uint32 pipe_green = SDL_MapRGB(screen->format, 34, 177, 76);

    float bird_y = SCREEN_HEIGHT / 2.0f;
    float velocity = 0.0f;
    float pspeed = 2.0f;
    const float gravity = 0.2f;
    const float flap_strength = -5.0f;
    float pipe_x = SCREEN_WIDTH;
    int pipe_gap_y = rand() % (SCREEN_HEIGHT - 200) + 100;

    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
            
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
                    velocity = flap_strength; 
                }
            }

            if (event.type == SDL_JOYBUTTONDOWN) {
                if (event.jbutton.button == 0) { // Wiimote 'A' Button
                    velocity = flap_strength; 
                }
                if (event.jbutton.button == 3) { // Wiimote 'Home' Button
                    is_running = false;
                }
            }
        }

        velocity += gravity;
        bird_y += velocity;
        pipe_x -= pspeed; 

        // Reset pipe if it goes offscreen
        if (pipe_x < -50) {
            pipe_x = SCREEN_WIDTH;
            pipe_gap_y = rand() % (SCREEN_HEIGHT - 200) + 100;
        }

        if (bird_y > SCREEN_HEIGHT - 32) {
            bird_y = SCREEN_HEIGHT - 32;
            velocity = 0;
        }
        if (bird_y < 0) {
            bird_y = 0;
            velocity = 0;
        }

        if (optimizedImage != NULL) {
            SDL_BlitSurface(optimizedImage, NULL, screen, NULL);
        } else {
            SDL_FillRect(screen, NULL, sky_blue); // Fallback background color
        }

        SDL_Rect bird_rect = {150, (int)bird_y, 32, 32};
        SDL_FillRect(screen, &bird_rect, bird_yellow);

        SDL_Rect pipe_rect_top = { (int)pipe_x, 0, 50, pipe_gap_y - 75 };
        SDL_Rect pipe_rect_bottom = { (int)pipe_x, pipe_gap_y + 75, 50, SCREEN_HEIGHT - (pipe_gap_y + 75) };
        SDL_FillRect(screen, &pipe_rect_top, pipe_green);
        SDL_FillRect(screen, &pipe_rect_bottom, pipe_green);

        SDL_Flip(screen);
        SDL_Delay(16); 
    }

    if (optimizedImage != NULL) {
        SDL_FreeSurface(optimizedImage);
    }
    
    IMG_Quit();

    if (wii_remote && SDL_JoystickOpened(0)) {
        SDL_JoystickClose(wii_remote);
    }

    SDL_Quit();
    return 0;
}
