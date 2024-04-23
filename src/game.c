#include "g_defs.h"
#include "render.h"
#include "update.h"
#include "utils/logger.h"
#include <SDL.h>

static global *g;

void *module_main(void *saved_state) {
  logger_init(DEBUG, NULL);

  // load or create global
  g = saved_state;
  if (g == NULL) {
    LOG("INITIALIZING WENGINE ...");
    g = calloc(1, sizeof(*g));
    init_sdl(g);
  } else {
    LOG("LOADING GLOBAL ...");
    LOG("GLOBAL LOADED");
  }

  // set defaults
  set_global_defaults(g);

  // gameloop
  while (!g->quit) {
    handle_event(g);

    render(g);
  }

  // reload / quit
  if (g->reload) {
    LOG("WENGINE RELOADING ...");
    return g;
  } else {
    LOG("WENGINGE CLOSING ...");
    deinit_sdl(g);
    return NULL;
  }
}
