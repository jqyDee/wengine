#include "g_sdl.h"
#include "g_defs.h"
#include "utils/logger.h"
#include <SDL_vulkan.h>
#include <stdio.h>

int sdl_init(global *g) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    sdl_quit(g);
    return 1;
  }
  LOG("SDL INITIALIZED\n");

  if (SDL_Vulkan_LoadLibrary(NULL) != 0)
    sdl_quit(g);
  LOG("LIBRAY SDL_VULKAN LOADED\n");

  g->sdl.window = SDL_CreateWindow(
      "wengine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
      WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
  if (!g->sdl.window) {
    sdl_quit(g);
    return 1;
  }
  LOG("SDL WINDOW CREATED\n");

  LOG("SDL INITIALIZED\n");
  return 0;
}

void sdl_quit(global *g) {
  vk_quit(g);
  if (strcmp(SDL_GetError(), "") != 0) {
    ERR("Error: %s\n", SDL_GetError());
  }
  SDL_DestroyWindow(g->sdl.window);
  LOG("SDL WINDOW DESTROYED\n");
  SDL_Quit();
  LOG("SDL DEINITIALIZED\n");
}
