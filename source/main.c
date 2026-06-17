#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

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

    Uint32 sky_blue = SDL_MapRGB(screen->format, 135, 206, 235);
    Uint32 bird_yellow = SDL_MapRGB(screen->format, 255, 225, 0);

    float bird_y = SCREEN_HEIGHT / 2.0f;
    float velocity = 0.0f;
    const float gravity = 0.2f;
    const float flap_strength = -5.0f;

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
                if (event.jbutton.button == 0) {
                    velocity = flap_strength; 
                }
                if (event.jbutton.button == 3) {
                    is_running = false;
                }
            }
        }

        velocity += gravity;
        bird_y += velocity;

        if (bird_y > SCREEN_HEIGHT - 32) {
            bird_y = SCREEN_HEIGHT - 32;
            velocity = 0;
        }
        if (bird_y < 0) {
            bird_y = 0;
            velocity = 0;
        }

        SDL_FillRect(screen, NULL, sky_blue);

        SDL_Rect bird_rect = {150, (int)bird_y, 32, 32};
        SDL_FillRect(screen, &bird_rect, bird_yellow);

        SDL_Flip(screen);
        SDL_Delay(16); 
    }

    if (SDL_JoystickOpened(0)) {
        SDL_JoystickClose(wii_remote);
    }

    SDL_Quit();
    return 0;
}
