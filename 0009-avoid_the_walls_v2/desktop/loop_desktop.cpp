#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)()) {
  while (!WindowShouldClose()) {
    MainLoop();
  }
}
