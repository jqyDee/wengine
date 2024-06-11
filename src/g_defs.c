#include "g_defs.h"
#include "vulkan/vulkan_core.h"

// ---------------------
// defaults
// ---------------------
int set_global_defaults(global *g) {
  // game
  g->game.reload = false;
  g->game.quit = false;

  // vk
  g->vk.pdevice = VK_NULL_HANDLE;
  g->vk.queues.family_indices.graphics_family = NO_INDEX;
  return 0;
}
