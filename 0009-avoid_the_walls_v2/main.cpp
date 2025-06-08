// main.cpp

#include "game.h"
#include "loop.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

void MainLoop(void *gamePtr) {
  Game *game = reinterpret_cast<Game *>(gamePtr);

  assert(game && "gamePtr is null in MainLoop");

  game->Run();
}

int main() {
  // Seed random number generator
  std::srand(std::time(nullptr));

  // Create game instance
  Game game;

  // Run the main loop (platform handles window, etc)
  RunPlatformLoop(MainLoop, &game);

  return 0;
}