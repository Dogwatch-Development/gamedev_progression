// loop_web.cpp

#include "../game.h"
#include "../loop.h"
#include <emscripten/emscripten.h>

static void (*RealMainLoop)(void *gamePtr) = nullptr;
static void *RealGamePtr = nullptr;

static void WrappedMainLoop(void *gamePtr) {
  // Check for shutdown
  Game *game = reinterpret_cast<Game *>(gamePtr);
  if (game->gameState.shuttingDown) {
    // If the game is shutting down, cancel the main loop
    emscripten_cancel_main_loop();
  }

  // Call user's main loop
  RealMainLoop(gamePtr);
}

void RunPlatformLoop(void (*MainLoop)(void *gamePtr), void *gamePtr) {
  RealMainLoop = MainLoop;
  RealGamePtr = gamePtr;

  emscripten_set_main_loop_arg(WrappedMainLoop, RealGamePtr, 0, 1);
}