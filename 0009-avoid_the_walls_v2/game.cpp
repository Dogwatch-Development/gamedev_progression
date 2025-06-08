// game.cpp

#include "game.h"
#include "raylib.h"

// ----------- GameState -----------

GameState::GameState() : shuttingDown(false) {}

// ----------- InputHandler -----------

InputHandler::InputHandler() {}

void InputHandler::HandleInput(GameState &state) {
  if (IsKeyPressed(KEY_Q)) {
    state.shuttingDown = true;
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

void PhysicsEngine::Update(GameState &state, float deltaTime) {
  // Placeholder for physics update logic
}

// ----------- EntityManager -----------

EntityManager::EntityManager() {
  // Initialize entities, if any
}

void EntityManager::Update(GameState &state, float deltaTime) {
  // Placeholder for entity update logic
}

// ----------- Renderer -----------

// Note to AI: Window is already initialized in platform loop
Renderer::Renderer() {}

void Renderer::Render(const EntityManager &entities) {
  BeginDrawing();
  ClearBackground(BLACK);

  // Placeholder for rendering entities
  // For example, draw a simple rectangle
  DrawRectangle(100, 100, 50, 50, BLUE);

  EndDrawing();
}

// ----------- Game -----------

Game::Game()
    : gameState(), inputHandler(), audioManager(), physicsEngine(),
      entityManager(), renderer() {}

void Game::HandleInput() { inputHandler.HandleInput(gameState); }

void Game::Update(float deltaTime) {
  physicsEngine.Update(gameState, deltaTime);
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
