#include "g_defs.h"
#include "update.h"
#include "utils/logger.h"
#include "g_sdl.h"

global *g;

void *module_main(void *saved_state) {
  logger_init(DEBUG, NULL);

  // load or create global
  g = saved_state;
  if (g == NULL) {
    LOG("INITIALIZING WENGINE ...\n");
    g = calloc(1, sizeof(*g));
    if (sdl_init(g) != 0)
      return NULL;
    if (vk_init(g) != 0)
        return NULL;
  } else {
    LOG("LOADING GLOBAL ...\n");
    LOG("GLOBAL LOADED\n");
  }

  // set defaults
  set_global_defaults(g);

  // gameloop
  while (!g->game.quit) {

    handle_event(g);

  }

  // game.reload / game.quit
  if (g->game.reload) {
    LOG("WENGINE RELOADING ...\n");
    return g;
  } else {
    LOG("WENGINGE CLOSING ...\n");
    sdl_quit(g);
    return NULL;
  }
}
