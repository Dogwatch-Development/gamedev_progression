// game.cpp

#include "game.h"
#include "entity.h"
#include "raylib.h"
#include <cmath>  // Include for ceilf usage
#include <cstdio> // Include for snprintf usage

// ----------- GameState -----------

GameState::GameState()
    : gameOver(false), shutdownRequested(false), resetRequested(false),
      elapsedTime(0.0f), countdownActive(true), countdownTime(3.0f) {}

// ----------- InputHandler -----------

InputHandler::InputHandler() {}

void InputHandler::HandleInput(GameState &state, EntityManager &entities) {
  Vector2 direction = entities.player.moveDir;
  if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
    direction = {0, -1};
  } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
    direction = {0, 1};
  } else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
    direction = {-1, 0};
  } else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
    direction = {1, 0};
  }
  entities.SetPlayerMoveDirection(direction);

  if (IsKeyPressed(KEY_Q)) {
    state.shutdownRequested = true;
  }
  if (IsKeyPressed(KEY_R)) {
    state.resetRequested = true;
  }
};

// ----------- AudioManager -----------

AudioManager::AudioManager() {
  InitAudioDevice();             // Initialize audio device
  sound = LoadSound("beep.wav"); // Load a beep sound
}

void AudioManager::PlayBeep() {
  PlaySound(sound); // Load and play a beep sound
}

AudioManager::~AudioManager() {
  UnloadSound(sound); // Unload the sound
  CloseAudioDevice(); // Close the audio device
}

// ----------- PhysicsEngine -----------

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::Update(GameState &state, EntityManager &entities,
                           float deltaTime) {
  // Move the player continuously in its current direction
  // and check for collision with the screen edges
  Player &player = entities.player;

  // Move player
  player.position.x += player.moveDir.x * player.speed * deltaTime;
  player.position.y += player.moveDir.y * player.speed * deltaTime;
  player.bounds.x = player.position.x;
  player.bounds.y = player.position.y;

  // Get screen size (from raylib)
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Check for collision with screen edges
  if (player.position.x < 0 ||
      player.position.x + player.bounds.width > screenW ||
      player.position.y < 0 ||
      player.position.y + player.bounds.height > screenH) {
    state.gameOver = true; // Game over if player hits the edge
  }
}

// ----------- EntityManager -----------

EntityManager::EntityManager()
    : player(100, 100, 50, 50, 200.0f) // Initialize member player
{}

void EntityManager::Update(GameState &state, float deltaTime) {
  player.Update(deltaTime);
}

void EntityManager::SetPlayerMoveDirection(Vector2 direction) {
  if (direction.x != player.moveDir.x || direction.y != player.moveDir.y) {
    player.speed += 20.0f; // Increase speed on every turn
  }
  player.moveDir = direction;
}

void EntityManager::ResetPlayer() {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();
  player.position = {(float)screenW / 2 - player.bounds.width / 2,
                     (float)screenH / 2 - player.bounds.height / 2};
  // Pick a random direction: 0=up, 1=down, 2=left, 3=right
  int dir = GetRandomValue(0, 3);
  switch (dir) {
  case 0:
    player.moveDir = {0, -1};
    break;
  case 1:
    player.moveDir = {0, 1};
    break;
  case 2:
    player.moveDir = {-1, 0};
    break;
  case 3:
    player.moveDir = {1, 0};
    break;
  }
  player.speed = 200.0f; // Reset speed to default
  player.bounds.x = player.position.x;
  player.bounds.y = player.position.y;
}

// ----------- Renderer -----------

// Note to AI: Window is already initialized in platform loop
Renderer::Renderer() {}

void Renderer::Render(const EntityManager &entities, const GameState &state) {
  BeginDrawing();
  ClearBackground(BLACK);

  // HUD: Speed (mph) and Timer
  float mph = entities.player.speed *
              0.0621371f; // 1 px/sec = 0.0621371 mph (arbitrary scale)
  char hud[64];
  snprintf(hud, sizeof(hud), "Speed: %.1f mph   Time: %.2f s", mph,
           state.elapsedTime);
  DrawText(hud, 20, 20, 20, WHITE);

  if (state.countdownActive) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int number = (int)ceilf(state.countdownTime);
    if (number > 0) {
      char numStr[16];
      snprintf(numStr, sizeof(numStr), "%d", number);
      int fontSize = 120;
      DrawText(numStr, screenW / 2 - MeasureText(numStr, fontSize) / 2,
               screenH / 2 - fontSize / 2, fontSize, YELLOW);
    }
    EndDrawing();
    return;
  }

  if (state.gameOver) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    const char *msg = "GAME OVER";
    const char *prompt = "Press R to Restart";
    int fontSize = 40;
    int promptSize = 20;
    DrawText(msg, screenW / 2 - MeasureText(msg, fontSize) / 2,
             screenH / 2 - fontSize, fontSize, RED);
    DrawText(prompt, screenW / 2 - MeasureText(prompt, promptSize) / 2,
             screenH / 2 + 10, promptSize, WHITE);
  } else {
    entities.player.Draw(); // Draw the player entity
  }

  EndDrawing();
}

// ----------- Game -----------

Game::Game()
    : gameState(), inputHandler(), audioManager(), physicsEngine(),
      entityManager(), renderer() {}

void Game::HandleInput() { inputHandler.HandleInput(gameState, entityManager); }

void Game::Update(float deltaTime) {
  if (gameState.resetRequested) {
    entityManager.ResetPlayer();
    gameState.shutdownRequested = false;
    gameState.resetRequested = false;
    gameState.gameOver = false;
    gameState.elapsedTime = 0.0f; // Reset timer
    gameState.countdownActive = true;
    gameState.countdownTime = 3.0f;
  }
  if (gameState.countdownActive) {
    gameState.countdownTime -= deltaTime;
    if (gameState.countdownTime <= 0.0f) {
      gameState.countdownActive = false;
      gameState.countdownTime = 0.0f;
    }
    // Don't update timer or physics/entities during countdown
    return;
  }
  if (!gameState.gameOver) {
    gameState.elapsedTime += deltaTime;
    physicsEngine.Update(gameState, entityManager, deltaTime);
    entityManager.Update(gameState, deltaTime);
  }
}

void Game::Render() { renderer.Render(entityManager, gameState); }

void Game::Run() {
  // Ensure player is centered after window is created
  entityManager.ResetPlayer();
  while (!gameState.shutdownRequested) {
    HandleInput();
    Update(GetFrameTime());
    Render();
  }
}
