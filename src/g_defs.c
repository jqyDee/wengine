#include "g_defs.h"
#include "utils/logger.h"

// ---------------------
// defaults
// ---------------------
void set_global_defaults(global *g) {
    g->reload = false;
    g->quit = false;
}

// ---------------------
// SDL
// ---------------------
int init_sdl(global *g) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    deinit_sdl(g);
    return 1;
  }
  LOG("SDL INITIALIZED");

  g->window =
      SDL_CreateWindow("wengine", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!g->window) {
    deinit_sdl(g);
    return 1;
  }
  LOG("SDL WINDOW CREATED");

  g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);
  if (!g->renderer) {
    deinit_sdl(g);
    return 1;
  }
  LOG("SDL RENDERER CREATED");

  LOG("WENGINE INITIALIZED");
  return 0;
}

void deinit_sdl(global *g) {
  if (strcmp(SDL_GetError(), "") != 0) {
    ERR("Error: %s\n", SDL_GetError());
  }
  SDL_DestroyWindow(g->window);
  LOG("SDL WINDOW DESTROYED");
  SDL_DestroyRenderer(g->renderer);
  LOG("SDL RENDERER DESTROYED");
  SDL_Quit();
  LOG("SDL DEINITIALIZED");
}
