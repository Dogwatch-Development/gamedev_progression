// loop_desktop.cpp

#include "../game.h"
#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)(void *gamePtr), void *gamePtr) {
  while (!WindowShouldClose()) {

    // Check for shutdown
    Game *game = reinterpret_cast<Game *>(gamePtr);
    if (game->gameState.shuttingDown) {
      break;
    }

    MainLoop(game);
  }
}
