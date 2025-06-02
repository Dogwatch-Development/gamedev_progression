#include "game.h"
#include "raylib.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

// TODO: figure out a better way to handle screen dimensions
// Global constants
const int screenWidth = 800;
const int screenHeight = 600;

// InputHandler implementation
void InputHandler::HandleInput(GameState &state) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    state.shuttingDown = true;
  }
}

// PhysicsEngine implementation
void PhysicsEngine::Update(GameState &state, float deltaTime) {
  // Implement physics update
}

// EntityManager implementation
void EntityManager::Update(GameState &state, float deltaTime) {
  // Implement entity update
}

// Renderer implementation
void Renderer::Render(const GameState &state) {
  BeginDrawing();
  ClearBackground(BLACK);

  const char text[] = "EUREKA!";
  int fontSize = 120;
  int textWidth = MeasureText(text, fontSize);
  int centerX = screenWidth / 2;
  int centerY = screenHeight / 2;
  DrawText(text, centerX - (textWidth / 2), centerY - (fontSize / 2), fontSize,
           RAYWHITE);

  EndDrawing();
}

// GameState constructor
GameState::GameState() : shuttingDown(false) {}

// Game constructor
Game::Game()
    : gameState(), inputHandler(), audioManager(), physics(), entityManager(),
      renderer() {}

void Game::HandleInput() { inputHandler.HandleInput(gameState); }

void Game::Update(float deltaTime) {
  if (gameState.shuttingDown) {
    EndGame();
    return;
  }
  physics.Update(gameState, deltaTime);
  entityManager.Update(gameState, deltaTime);
}

void Game::Render() { renderer.Render(gameState); }

void Game::EndGame() { gameState.shuttingDown = true; }
