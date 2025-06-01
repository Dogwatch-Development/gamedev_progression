#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)(void *game), void *game) {
  while (!WindowShouldClose()) {
    MainLoop(game);
  }
}
