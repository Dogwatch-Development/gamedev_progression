// game.h

#pragma once

#include "entity.h"
#include "raylib.h"

// ----------- GameState -----------
// Manages: overall game status flags
// Should Own:
//   - Whether the game is running
//   - Whether the game is over
//   - Whether a shutdown has been requested
// Should Not:
//   - Know about entities
//   - Know about input or rendering
class GameState {
public:
  bool shuttingDown;
  GameState();
};

// ----------- AudioManager -----------
// Manages: sound effects and audio playback
// Should Own:
//   - Loading, playing, stopping sounds
//   - Keeping track of currently loaded sounds
// Should Not:
//   - Know about input, physics, or entities
class AudioManager {
public:
  void PlayBeep(); // Example: play a simple beep sound
  AudioManager();
  ~AudioManager();

private:
  Sound sound;
};

// ----------- EntityManager -----------
// Manages: the collection of game entities
// Should Own:
//   - Storage of all entities (player, enemies, objects)
//   - Creating, deleting, updating entities
// Should Not:
//   - Draw entities
//   - Handle physics directly (physics can modify entities, but not the manager
//   itself)
class EntityManager {
public:
  Player player;
  void Update(GameState &state, float deltaTime);
  EntityManager();

  void SetPlayerMoveDirection(Vector2 direction);
};

// ----------- InputHandler -----------
// Manages: user input handling
// Should Own:
//   - Mapping input keys/buttons to actions
//   - Detecting key presses or mouse input
// Should Not:
//   - Update entities
//   - Play sounds
class InputHandler {
public:
  void HandleInput(GameState &state, EntityManager &entities);
  InputHandler();
};

// ----------- PhysicsEngine -----------
// Manages: movement and physical simulation
// Should Own:
//   - Updating positions, velocities, forces
//   - (Later) Handling collisions, gravity, friction
// Should Not:
//   - Render entities
//   - Handle user input
class PhysicsEngine {
public:
  void Update(GameState &state, EntityManager &entities, float deltaTime);
  PhysicsEngine();
};

// ----------- Renderer -----------
// Manages: drawing to the screen
// Should Own:
//   - Drawing entities, backgrounds, UI
//   - Managing the render order if necessary
// Should Not:
//   - Update game logic or entity states
//   - Handle input or play sounds
class Renderer {
public:
  void Render(const EntityManager &entities);
  Renderer();
};

// ----------- Game -----------
// Manages: top-level orchestration of the game
// Should Own:
//   - Instances of all subsystems (InputHandler, AudioManager, etc.)
//   - Game loop: input -> update -> render
//   - Starting and stopping the game
// Should Not:
//   - Directly update physics, entities, or render details (delegate to
//   subsystems)
class Game {
public:
  GameState gameState;
  InputHandler inputHandler;
  AudioManager audioManager;
  PhysicsEngine physicsEngine;
  EntityManager entityManager;
  Renderer renderer;

  Game();
  void HandleInput();
  void Update(float deltaTime);
  void Render();
  void Run();
};
