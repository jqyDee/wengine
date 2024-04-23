#include "g_defs.h"
#include "update.h"

void handle_event(global *g) {
  while (SDL_PollEvent(&g->event)) {
    switch (g->event.type) {
    case SDL_QUIT:
      g->quit = true;
    case SDL_KEYDOWN:
      switch (g->event.key.keysym.scancode) {
      case SDL_SCANCODE_F5:
        g->quit = true;
        g->reload = true;
      default:
        break;
      }
    default:
      break;
    }
  }
}
