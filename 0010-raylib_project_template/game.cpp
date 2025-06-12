// game.cpp

#include "game.h"
#include "raylib.h"

Game::Game() {}

void Game::HandleInput() {}

void Game::Update(float deltaTime) {}

void Game::Render() {}

void Game::Run() {
  HandleInput();
  Update(GetFrameTime());
  Render();
}
