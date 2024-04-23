#include "render.h"

void render(global *g) {
  SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
  SDL_RenderClear(g->renderer);
  SDL_RenderPresent(g->renderer);
}
