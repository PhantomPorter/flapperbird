#include "back_jpg.h"
#include "font_ttf.h"
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <gccore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 16
bool check_collision(SDL_Rect *rectA, SDL_Rect *rectB) {
  int leftA = rectA->x;
  int rightA = rectA->x + rectA->w;
  int topA = rectA->y;
  int bottomA = rectA->y + rectA->h;

  int leftB = rectB->x;
  int rightB = rectB->x + rectB->w;
  int topB = rectB->y;
  int bottomB = rectB->y + rectB->h;

  if (bottomA <= topB)
    return false;
  if (topA >= bottomB)
    return false;
  if (rightA <= leftB)
    return false;
  if (leftA >= rightB)
    return false;

  return true;
}

int main(int argc, char *argv[]) {
  SYS_STDIO_Report(true);
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL initialization failed: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Joystick *wii_remote = NULL;
  if (SDL_NumJoysticks() > 0) {
    wii_remote = SDL_JoystickOpen(0);
    if (wii_remote) {
      printf("Connected to Controller: %s\n", SDL_JoystickName(0));
    }
  }

  SDL_Surface *screen =
      SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
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
  SDL_RWops *rw = SDL_RWFromMem(back_jpg, back_jpg_len);
  if (rw != NULL) {
    loadedImage = IMG_Load_RW(rw, 1);
  }

  if (loadedImage != NULL) {
    // Convert to the display format for fast blitting
    optimizedImage = SDL_DisplayFormat(loadedImage);
    // Free the original loaded surface
    SDL_FreeSurface(loadedImage);
  } else {
    printf("IMG_Load Error: %s\n", IMG_GetError());
    // Fallback: If image fails to load, game will just use standard FillRect
    // colors
  }

  if (TTF_Init() == -1) {
    printf("TTF_Init Error: %s\n", TTF_GetError());
  }

  SDL_RWops *font_rw = SDL_RWFromMem(font_ttf, font_ttf_len);
  TTF_Font *font = TTF_OpenFontRW(font_rw, 1, 24);

  if (font == NULL) {
    printf("TTF_OpenFontRW Error: %s\n", TTF_GetError());
  }

  SDL_Color textColor = {255, 255, 255};

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
  bool is_playing = true;

  SDL_Event event;

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      // Wiimote 'Home' Button
      if (event.jbutton.button == 3 || event.jbutton.button == 11) {
        is_running = false;
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          is_running = false;
        }
        if (event.key.keysym.sym == SDLK_SPACE ||
            event.key.keysym.sym == SDLK_RETURN) {
          velocity = flap_strength;
        }
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          is_running = false;
        }
        if (event.key.keysym.sym == SDLK_SPACE ||
            event.key.keysym.sym == SDLK_RETURN) {
          if (is_playing) {
            velocity = flap_strength;
          } else {

            bird_y = SCREEN_HEIGHT / 2.0f;
            velocity = 0;
            pipe_x = SCREEN_WIDTH;
            is_playing = true;
          }
        }
      }
      if (event.type == SDL_JOYBUTTONDOWN) {
        if (event.jbutton.button == 0) { // Wiimote 'A' Button
          if (is_playing) {
            velocity = flap_strength;
          } else {

            bird_y = SCREEN_HEIGHT / 2.0f;
            velocity = 0;
            pipe_x = SCREEN_WIDTH;
            is_playing = true;
          }
        }
      }
    }
    if (is_playing) {
      velocity += gravity;
      bird_y += velocity;
      pipe_x -= pspeed;

      if (pipe_x < -50) {
        pipe_x = SCREEN_WIDTH;
        pipe_gap_y = rand() % (SCREEN_HEIGHT - 200) + 100;
      }
      if (bird_y > SCREEN_HEIGHT - 32) {
        bird_y = SCREEN_HEIGHT - 32;
        velocity = 0;
        is_playing = false;
      }
      if (bird_y < 0) {
        bird_y = 0;
        velocity = 0;
        is_playing = false;
      }
    }
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

    SDL_Rect pipe_rect_top = {(int)pipe_x, 0, 50, pipe_gap_y - 75};
    SDL_Rect pipe_rect_bottom = {(int)pipe_x, pipe_gap_y + 75, 50,
                                 SCREEN_HEIGHT - (pipe_gap_y + 75)};
    SDL_FillRect(screen, &pipe_rect_top, pipe_green);
    SDL_FillRect(screen, &pipe_rect_bottom, pipe_green);

    if (is_playing) {
      if (check_collision(&bird_rect, &pipe_rect_top) ||
          check_collision(&bird_rect, &pipe_rect_bottom)) {
        is_playing = false;
      }
    }

    if (!is_playing) {
      SDL_Surface *textSurface =
          TTF_RenderText_Blended(font, "GAME OVER", textColor);
      if (textSurface != NULL) {
        SDL_Rect textOffset;
        textOffset.x = (SCREEN_WIDTH - textSurface->w) / 2;
        textOffset.y = 200;
        SDL_BlitSurface(textSurface, NULL, screen, &textOffset);
        SDL_FreeSurface(textSurface);
      }

      SDL_Surface *subSurface =
          TTF_RenderText_Blended(font, "Press A / Space to Restart", textColor);
      if (subSurface != NULL) {
        SDL_Rect subOffset;
        subOffset.x = (SCREEN_WIDTH - subSurface->w) / 2;
        subOffset.y = 250;
        SDL_BlitSurface(subSurface, NULL, screen, &subOffset);
        SDL_FreeSurface(subSurface);
      }
    }
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

  if (font != NULL) {
    TTF_CloseFont(font);
  }
  TTF_Quit();

  SDL_Quit();
  SDL_Quit();
  return 0;
}
