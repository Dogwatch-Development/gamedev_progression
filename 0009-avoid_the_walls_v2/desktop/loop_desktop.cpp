// loop_desktop.cpp

#include "../game.h"
#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)(void *game), void *game) {
  while (!WindowShouldClose()) {
    MainLoop(game);
  }
}
