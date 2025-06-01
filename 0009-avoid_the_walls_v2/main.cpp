#include "loop.h"
#include "raylib.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

// Global constants
const int screenWidth = 800;
const int screenHeight = 600;

class GameState {
public:
  // shuttingDown, flipped when the player expresses desire to quit
  bool shuttingDown = false;
};

class InputHandler {
public:
  void HandleInput(GameState &state) {
    // Escape to quit
    if (IsKeyPressed(KEY_ESCAPE)) {
      state.shuttingDown = true;
    }
  }
};

class AudioManager {
public:
  // Add audio management methods as needed
};

class PhysicsEngine {
public:
  void Update(GameState &state, float deltaTime) {
    // Implement physics update
  }
};

class EntityManager {
public:
  void Update(GameState &state, float deltaTime) {
    // Implement entity update
  }
};

class Renderer {
public:
  void Render(const GameState &state) {
    BeginDrawing();
    ClearBackground(BLACK);

    const char text[] = "EUREKA!";
    int fontSize = 120;
    int textWidth = MeasureText(text, fontSize);
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    DrawText(text, centerX - (textWidth / 2), centerY - (fontSize / 2),
             fontSize, RAYWHITE);

    EndDrawing();
  }
};

class Game {
public:
  GameState gameState;
  InputHandler inputHandler;
  AudioManager audioManager;
  PhysicsEngine physics;
  EntityManager entityManager;
  Renderer renderer;

  void HandleInput() { inputHandler.HandleInput(gameState); }

  void Update(float deltaTime) {
    if (gameState.shuttingDown) {
      EndGame();
      return;
    }
    physics.Update(gameState, deltaTime);
    entityManager.Update(gameState, deltaTime);
  }

  void Render() { renderer.Render(gameState); }

  void EndGame() { gameState.shuttingDown = true; }
};

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