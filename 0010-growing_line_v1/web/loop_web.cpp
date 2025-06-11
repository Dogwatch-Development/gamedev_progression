// loop_web.cpp

#include "../constants.h"
#include "../game.h"
#include "../loop.h"
#include "raylib.h"
#include <emscripten/emscripten.h>

static void (*RealMainLoop)(void *gamePtr) = nullptr;
static void *RealGamePtr = nullptr;

static void WrappedMainLoop(void *gamePtr) {

  Game *game = reinterpret_cast<Game *>(gamePtr);

  if (game->shutdownRequested) {
    emscripten_cancel_main_loop();
  }

  RealMainLoop(gamePtr);
}

void RunPlatformLoop(void (*MainLoop)(void *gamePtr), void *gamePtr) {
  InitWindow(screenWidth, screenHeight, "Avoid the Walls V2");
  SetExitKey(0);
  SetTargetFPS(60);

  RealMainLoop = MainLoop;
  RealGamePtr = gamePtr;

  emscripten_set_main_loop_arg(WrappedMainLoop, RealGamePtr, 0, 1);
}