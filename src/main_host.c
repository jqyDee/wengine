#include <stdbool.h>
#include <stdio.h>

#include <dlfcn.h>
#include <utils.h>

int main(void) {
  logger_init(DEBUG, NULL);
  void *state = NULL;

  int cycle = 0;
  while (true && cycle <= 10) {
    void *module = dlopen("./libsdl2-hotreload-module.dylib", RTLD_NOW);
    if (module == NULL) {
      ERR("Failed to load library: (%s)", dlerror());
      cycle++;
      continue;
    } else {
      cycle = 0;
    }
    typedef void *module_main_func(void *state);
    module_main_func *module_main = dlsym(module, "module_main");
    state = module_main(state);

    dlclose(module);
    if (state == NULL)
      return 0;
  }
  ERR("couldn't load library in %d tries! exiting.", cycle);
  return 0;
}
