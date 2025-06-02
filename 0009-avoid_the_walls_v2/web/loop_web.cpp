// loop_web.cpp

#include "../game.h"
#include "../loop.h"
#include <emscripten/emscripten.h>

void RunPlatformLoop(void (*MainLoop)(void *game), void *game) {
  emscripten_set_main_loop_arg(MainLoop, game, 0, 1);
}
