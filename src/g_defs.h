#ifndef G_DEFS_H
#define G_DEFS_H

#include "SDL_video.h"
#include "player.h"
#include "vulkan/vulkan_core.h"
#include <SDL.h>
#include <stdbool.h>

#define NO_INDEX 999

#define APPLICATION_NAME "test\0"
#define APPLICATION_VERSION 1
#define ENGINE_NAME "WENIGNE\n"
#define ENGINE_VERSION 1

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

typedef struct g_s {
  // game state
  struct game_s {
    bool quit;
    bool reload;
  } game;

  // SDL
  struct sdl_s {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
  } sdl;

  // Vulkan
  struct vk_s {
    VkInstance inst;
    VkPhysicalDevice pdevice;
    VkDevice ldevice;
    struct extensions_s {
        u32 count;
        const char **names;
    } extensions;
    struct layers_s {
      u32 count;
      const char **names;
    } layers;
    struct queue_s {
      VkQueue graphics_queue;
      struct family_indicies_s {
        u32 graphics_family;
      } family_indices;
    } queues;
  } vk;

  // player
  player_t player;
} global;

int set_global_defaults(global *g);

#endif
