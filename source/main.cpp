#include "back_jpg.h"
#include "font_ttf.h"

extern "C" {
#include "music_ogg.h"
}
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <asndlib.h>
#include <gccore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

static unsigned char *aligned_music_buffer = NULL;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 16

bool check_collision(SDL_Rect a, SDL_Rect b) {
  if (a.x + a.w <= b.x)
    return false;
  if (a.x >= b.x + b.w)
    return false;
  if (a.y + a.h <= b.y)
    return false;
  if (a.y >= b.y + b.h)
    return false;
  return true;
}

int main(int argc, char *argv[]) {
  SYS_STDIO_Report(true);

  ASND_Init();
  ASND_Pause(0);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL initialization failed: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Joystick *wii_remote = NULL;
  if (SDL_NumJoysticks() > 0) {
    wii_remote = SDL_JoystickOpen(0);
  }

  SDL_Surface *screen =
      SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if (screen == NULL) {
    SDL_Quit();
    return -1;
  }

  if (TTF_Init() == -1) {
    printf("TTF_Init Error: %s\n", TTF_GetError());
  }

  SDL_RWops *font_rw = SDL_RWFromMem(font_ttf, font_ttf_len);
  TTF_Font *font = TTF_OpenFontRW(font_rw, 0, 24);

  int flags = IMG_INIT_JPG;
  IMG_Init(flags);

  SDL_Surface *loadedImage = NULL;
  SDL_Surface *optimizedImage = NULL;
  SDL_RWops *rw = SDL_RWFromMem(back_jpg, back_jpg_len);
  if (rw != NULL)
    loadedImage = IMG_Load_RW(rw, 1);
  if (loadedImage != NULL) {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
  }

 aligned_music_buffer = (unsigned char*)memalign(32, music_wav_len);
    if (aligned_music_buffer != NULL) {
        memcpy(aligned_music_buffer, music_wav, music_wav_len);
        
       
        for (unsigned int i = 0; i < music_wav_len; i += 2) {
            unsigned char temp = aligned_music_buffer[i];
            aligned_music_buffer[i] = aligned_music_buffer[i + 1];
            aligned_music_buffer[i + 1] = temp;
        }

        DCFlushRange((void *)aligned_music_buffer, music_wav_len);
        
        ASND_SetInfiniteVoice(0, VOICE_MONO_16BIT, 22050, 0, (void *)(aligned_music_buffer + 44), (music_wav_len - 44), 200, 200);
    }


  Uint32 sky_blue = SDL_MapRGB(screen->format, 135, 206, 235);
  Uint32 bird_yellow = SDL_MapRGB(screen->format, 255, 225, 0);
  Uint32 pipe_green = SDL_MapRGB(screen->format, 34, 177, 76);
  SDL_Color textColor = {255, 255, 255};

  float bird_y = SCREEN_HEIGHT / 2.0f;
  float velocity = 0.0f;
  float pspeed = 2.0f;
  const float gravity = 0.2f;
  const float flap_strength = -5.0f;
  float pipe_x = SCREEN_WIDTH;
  int pipe_gap_y = rand() % (SCREEN_HEIGHT - 200) + 100;

  int score = 0;
  bool score_counted = false;
  bool is_playing = true;
  bool is_running = true;
  SDL_Event event;

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        is_running = false;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          is_running = false;
        if (event.key.keysym.sym == SDLK_SPACE ||
            event.key.keysym.sym == SDLK_RETURN) {
          if (is_playing) {
            velocity = flap_strength;
          } else {
            ASND_StopVoice(0);
            if (aligned_music_buffer != NULL) {
              DCFlushRange((void *)aligned_music_buffer, music_wav_len);
              ASND_SetInfiniteVoice(0, VOICE_MONO_16BIT, 22050, 0,
                                    (void *)(aligned_music_buffer + 44),
                                    (music_wav_len - 44), 200, 200);
            }

            bird_y = SCREEN_HEIGHT / 2.0f;
            velocity = 0;
            pipe_x = SCREEN_WIDTH;
            score = 0;
            score_counted = false;
            is_playing = true;
          }
        }
      }

      if (event.type == SDL_JOYBUTTONDOWN) {
        if (event.jbutton.button == 0 || event.jbutton.button == 1) {
          if (is_playing) {
            velocity = flap_strength;
          } else {

            ASND_StopVoice(0);
            if (aligned_music_buffer != NULL) {
              DCFlushRange((void *)aligned_music_buffer, music_wav_len);
              ASND_SetInfiniteVoice(0, VOICE_MONO_16BIT, 22050, 0,
                                    (void *)(aligned_music_buffer + 44),
                                    (music_wav_len - 44), 200, 200);
            }

            bird_y = SCREEN_HEIGHT / 2.0f;
            velocity = 0;
            pipe_x = SCREEN_WIDTH;
            score = 0;
            score_counted = false;
            is_playing = true;
          }
        }
        if (event.jbutton.button == 3 || event.jbutton.button == 11)
          is_running = false;
      }
    }

    if (is_playing) {
      velocity += gravity;
      bird_y += velocity;
      pipe_x -= pspeed;

      if (pipe_x < -50) {
        pipe_x = SCREEN_WIDTH;
        pipe_gap_y = rand() % (SCREEN_HEIGHT - 200) + 100;
        score_counted = false;
      }

      if (pipe_x + 50 < 150 && !score_counted) {
        score++;
        score_counted = true;
      }

      if (bird_y > SCREEN_HEIGHT - 32 || bird_y < 0) {
        ASND_StopVoice(0);
        is_playing = false;
      }

      SDL_Rect bird_rect = {150, (int)bird_y, 32, 32};
      SDL_Rect pipe_rect_top = {(int)pipe_x, 0, 50, pipe_gap_y - 75};
      SDL_Rect pipe_rect_bottom = {(int)pipe_x, pipe_gap_y + 75, 50,
                                   SCREEN_HEIGHT - (pipe_gap_y + 75)};

      if (check_collision(bird_rect, pipe_rect_top) ||
          check_collision(bird_rect, pipe_rect_bottom)) {
        ASND_StopVoice(0);
        is_playing = false;
      }
    }

    if (optimizedImage != NULL) {
      SDL_BlitSurface(optimizedImage, NULL, screen, NULL);
    } else {
      SDL_FillRect(screen, NULL, sky_blue);
    }

    SDL_Rect bird_rect = {150, (int)bird_y, 32, 32};
    SDL_FillRect(screen, &bird_rect, bird_yellow);

    SDL_Rect pipe_rect_top = {(int)pipe_x, 0, 50, pipe_gap_y - 75};
    SDL_Rect pipe_rect_bottom = {(int)pipe_x, pipe_gap_y + 75, 50,
                                 SCREEN_HEIGHT - (pipe_gap_y + 75)};
    SDL_FillRect(screen, &pipe_rect_top, pipe_green);
    SDL_FillRect(screen, &pipe_rect_bottom, pipe_green);

    if (font != NULL) {
      char scoreText[32];
      snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
      SDL_Surface *scoreSurface =
          TTF_RenderText_Blended(font, scoreText, textColor);
      if (scoreSurface != NULL) {
        SDL_Rect scoreOffset = {20, 20, 0, 0};
        SDL_BlitSurface(scoreSurface, NULL, screen, &scoreOffset);
        SDL_FreeSurface(scoreSurface);
      }
    }

    if (!is_playing && font != NULL) {
      SDL_Surface *textSurface =
          TTF_RenderText_Blended(font, "GAME OVER", textColor);
      if (textSurface != NULL) {
        SDL_Rect textOffset = {(Sint16)((SCREEN_WIDTH - textSurface->w) / 2),
                               200, 0, 0};
        SDL_BlitSurface(textSurface, NULL, screen, &textOffset);
        SDL_FreeSurface(textSurface);
      }

      SDL_Surface *subSurface =
          TTF_RenderText_Blended(font, "Press A / Space to Restart", textColor);
      if (subSurface != NULL) {
        SDL_Rect subOffset = {(Sint16)((SCREEN_WIDTH - subSurface->w) / 2), 250,
                              0, 0};
        SDL_BlitSurface(subSurface, NULL, screen, &subOffset);
        SDL_FreeSurface(subSurface);
      }
    }

    SDL_Flip(screen);
    SDL_Delay(16);
  }

  ASND_StopVoice(0);
  ASND_End();

  if (aligned_music_buffer != NULL) {
    free(aligned_music_buffer); 
  }

  if (optimizedImage != NULL)
    SDL_FreeSurface(optimizedImage);
  if (wii_remote && SDL_JoystickOpened(0))
    SDL_JoystickClose(wii_remote);
  if (font != NULL)
    TTF_CloseFont(font);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
