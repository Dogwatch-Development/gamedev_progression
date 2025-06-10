// entity.h

#pragma once

#include "raylib.h"

// ----------- Entity -----------
// Base class for all game entities
// Manages: position, size, and collision detection
// Should Own:
//   - Position and size of the entity
//   - Bounding box for collision detection
// Should Not:
//   - Handle rendering directly (should use Renderer)
class Entity {
public:
  Vector2 position; // Position of the entity
  Rectangle bounds; // Bounding box for collision detection

  Entity(float x, float y, float width, float height);

  virtual void
  Update(float deltaTime) = 0;   // Pure virtual function for updating
  virtual void Draw() const = 0; // Pure virtual function for drawing
};

// ----------- Player -----------
// Manages: player entity
// Should Own:
//   - Player position, size, speed
//   - Player input handling
// Should Not:
//   - Handle rendering directly (should use Renderer)
class Player : public Entity {
public:
  float speed;
  Vector2 moveDir;

  Player(float x, float y, float width, float height, float speed);

  void
  Update(float deltaTime) override; // Update player position based on input
  void Draw() const override;       // Draw the player
};