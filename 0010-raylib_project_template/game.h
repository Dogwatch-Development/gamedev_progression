// game.h

#pragma once

#include "raylib.h"

class Game {
public:
  Game();
  void HandleInput();
  void Update(float deltaTime);
  void Render();
  void Run();
  bool shutdownRequested;
};
