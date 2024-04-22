#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <utils.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static struct {
  bool quit;
  bool reload;
  SDL_Renderer *renderer;
  SDL_Window *window;
} *state;

static void handle_event(SDL_Event event) {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      state->quit = true;
    case SDL_KEYDOWN:
      switch (event.key.keysym.scancode) {
      case SDL_SCANCODE_F5:
        state->quit = true;
        state->reload = true;
      default:
        break;
      }
    default:
      break;
    }
  }
}

static void render() {
  SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 255);
  SDL_RenderClear(state->renderer);
  SDL_RenderPresent(state->renderer);
}

void *module_main(void *saved_state) {
  state = saved_state;
  if (state == NULL) {
    LOG("no prev state");
    state = calloc(1, sizeof(*state));
  } else {
    LOG("prev state loaded");
  }

  // INITIALIZATION
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    goto CLEANUP;

  state->window = SDL_CreateWindow("Flopsy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!state->window)
    goto CLEANUP;

  state->renderer =
      SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
  if (!state->renderer)
    goto CLEANUP;

  state->quit = false;
  SDL_Event event = event;
  while (!state->quit) {
    handle_event(event);

    render();
  }

// CLEANUP
CLEANUP:;
  if (strcmp(SDL_GetError(), "") != 0) {
    ERR("Error: %s\n", SDL_GetError());
  }
  SDL_DestroyWindow(state->window);
  SDL_DestroyRenderer(state->renderer);
  SDL_Quit();
  LOG("Cleanup successfull!\n");
  if (state->reload)
    return state;
  else
    return NULL;
}
