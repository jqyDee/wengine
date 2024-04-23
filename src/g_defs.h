#ifndef G_DEFS_H
#define G_DEFS_H

#include "player.h"
#include <SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct g_s {
  // game state
  bool quit;
  bool reload;

  // SDL
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Event event;

  // player
  player_t player;
} global;

void deinit_sdl(global *g);
int init_sdl(global *g);

void set_global_defaults(global *g);

#endif
