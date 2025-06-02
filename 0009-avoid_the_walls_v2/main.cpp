// main.cpp

#include "constants.h"
#include "game.h"
#include "loop.h"
#include "raylib.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

void MainLoop(void *gamePtr) {
  Game *game = reinterpret_cast<Game *>(gamePtr);

  assert(game && "gamePtr is null in MainLoop");

  game->HandleInput();
  game->Update(GetFrameTime());
  game->Render();
}

int main() {
  // Initialization
  InitWindow(screenWidth, screenHeight, "Avoid the Walls V2");
  SetTargetFPS(60);

  // Seed random number generator
  std::srand(std::time(nullptr));

  // Create game instance
  Game game;

  // Run the main loop
  RunPlatformLoop(MainLoop, &game);

  // Return to OS
  return 0;
}