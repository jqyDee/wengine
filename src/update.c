#include "g_defs.h"
#include "update.h"

void handle_event(global *g) {
  while (SDL_PollEvent(&g->sdl.event)) {
    switch (g->sdl.event.type) {
    case SDL_QUIT:
      g->game.quit = true;
    case SDL_KEYDOWN:
      switch (g->sdl.event.key.keysym.scancode) {
      case SDL_SCANCODE_F5:
        g->game.quit = true;
        g->game.reload = true;
      default:
        break;
      }
    default:
      break;
    }
  }
}
