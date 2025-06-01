#include "../loop.h"
#include <emscripten/emscripten.h>

// not immediately obvious whats going on here (wasn't to me anyways)
// the first argument is a function pointer,
// the second is a pointer to the game state,
void RunPlatformLoop(void (*MainLoop)(void *game), void *game) {
  // now inside of the function here we put them together
  emscripten_set_main_loop_arg(MainLoop, game, 0, 1);
}
