// loop_desktop.cpp

#include "../constants.h"
#include "../game.h"
#include "../loop.h"
#include "raylib.h"

void RunPlatformLoop(void (*MainLoop)(void *gamePtr), void *gamePtr) {
  InitWindow(screenWidth, screenHeight, gameTitle);
  SetExitKey(0); // Disable default ESC behavior
  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    Game *game = reinterpret_cast<Game *>(gamePtr);

    if (game->gameState.shuttingDown) {
      break;
    }

    MainLoop(game);
  }

  CloseWindow();
}
