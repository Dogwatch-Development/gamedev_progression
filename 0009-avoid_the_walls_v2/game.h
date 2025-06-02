#pragma once

#include "raylib.h"

class GameState {
public:
  bool shuttingDown;
  GameState();
};

class InputHandler {
public:
  void HandleInput(GameState &state);
};

class AudioManager {
public:
  // Add audio management methods as needed
};

class PhysicsEngine {
public:
  void Update(GameState &state, float deltaTime);
};

class EntityManager {
public:
  void Update(GameState &state, float deltaTime);
};

class Renderer {
public:
  void Render(const GameState &state);
};

class Game {
public:
  GameState gameState;
  InputHandler inputHandler;
  AudioManager audioManager;
  PhysicsEngine physics;
  EntityManager entityManager;
  Renderer renderer;

  Game();
  void HandleInput();
  void Update(float deltaTime);
  void Render();
  void EndGame();
};
