#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <iostream>

using namespace std;

struct Color {
  Uint8 r, g, b, a;
};

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h) {
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h, Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

int main() {

  const int width = 500;
  const int height = 300;

  // Init
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Test", 0, 0, width, height, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_bool running = SDL_TRUE;
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;

  float pos = 0;
  float speed = 15;

  // Game Loop
  while (running) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = SDL_FALSE;
        break;
      }
    }

    pos += speed * deltaTime;

    SDL_RenderClear(renderer);

    DrawRect(renderer, 0, 0, width, height, {0, 0, 0, SDL_ALPHA_OPAQUE});
    DrawRect(renderer, pos, 100, 50, 100, {255, 255, 255, SDL_ALPHA_OPAQUE});
    DrawRect(renderer, pos, 250, 100, 25);

    SDL_RenderPresent(renderer);
  }

  // Clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
