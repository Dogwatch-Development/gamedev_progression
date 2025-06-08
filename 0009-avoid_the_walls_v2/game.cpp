// game.cpp

#include "game.h"
#include "entity.h"
#include "raylib.h"

// ----------- GameState -----------

GameState::GameState() : shuttingDown(false) {}

// ----------- InputHandler -----------

InputHandler::InputHandler() {}

void InputHandler::HandleInput(GameState &state, EntityManager &entities) {
  // Only allow direction change if not reversing
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
    state.shuttingDown = true;
  }
  if (IsKeyPressed(KEY_R)) {
    // Reset player to center and moving right
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    entities.player.position = {
        (float)screenW / 2 - entities.player.bounds.width / 2,
        (float)screenH / 2 - entities.player.bounds.height / 2};
    entities.player.moveDir = {1, 0};
    state.shuttingDown = false;
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
    state.shuttingDown = true; // End the game if player hits the wall
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
  player.moveDir = direction;
}

// ----------- Renderer -----------

// Note to AI: Window is already initialized in platform loop
Renderer::Renderer() {}

void Renderer::Render(const EntityManager &entities) {
  BeginDrawing();
  ClearBackground(BLACK);

  entities.player.Draw(); // Draw the player entity

  EndDrawing();
}

// ----------- Game -----------

Game::Game()
    : gameState(), inputHandler(), audioManager(), physicsEngine(),
      entityManager(), renderer() {}

void Game::HandleInput() { inputHandler.HandleInput(gameState, entityManager); }

void Game::Update(float deltaTime) {
  physicsEngine.Update(gameState, entityManager, deltaTime);
  entityManager.Update(gameState, deltaTime);
}

void Game::Render() { renderer.Render(entityManager); }

void Game::Run() {
  while (!gameState.shuttingDown) {
    HandleInput();
    Update(GetFrameTime());
    Render();
  }
}
