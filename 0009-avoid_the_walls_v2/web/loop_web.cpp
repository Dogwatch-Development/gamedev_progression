#include "../loop.h"
#include <emscripten/emscripten.h>

void RunPlatformLoop(void (*MainLoop)()) {
  emscripten_set_main_loop(MainLoop, 0, 1);
}
