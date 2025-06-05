// loop_desktop.cpp

#include "../game.h"
#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)(void *gamePtr), void *gamePtr) {
  while (!WindowShouldClose()) {

    Game *game = reinterpret_cast<Game *>(gamePtr);

    // Check for shutdown
    if (game->gameState.shuttingDown) {
      TraceLog(LOG_INFO, "[desktop] Shutting down game, exiting main loop.");
      break;
    }

    MainLoop(game);
  }
}
