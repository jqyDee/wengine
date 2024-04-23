#include <stdbool.h>
#include <stdio.h>

#include "utils/logger.h"
#include <dlfcn.h>

int main(void) {
  logger_init(DEBUG, NULL);
  LOG("HOST STARTED");
  void *state = NULL;

  int cycle = 0;
  while (true && cycle <= 10) {
    void *module = dlopen("./libwengine-module.dylib", RTLD_NOW);
    if (module == NULL) {
      ERR("FAILED TO LOAD WENGINE LIB: (%s)", dlerror());
      cycle++;
      continue;
    } else {
      LOG("WENGINE LIB LOADED");
      cycle = 0;
    }
    typedef void *module_main_func(void *state);
    module_main_func *module_main = dlsym(module, "module_main");
    state = module_main(state);

    dlclose(module);
    LOG("WENGINE LIB CLOSED");
    if (state == NULL) {
      LOG("HOST CLOSING");
      return 0;
    }
  }
  ERR("couldn't load library in %d tries! exiting.", cycle);
  return 0;
}
