#include "entity.h"
#include "raylib.h"
#include <cmath>

// ----------- Entity -----------

Entity::Entity(float x, float y, float width, float height)
    : position{x, y}, bounds{x, y, width, height} {}

// ----------- Player -----------

Player::Player(float x, float y, float width, float height, float speed)
    : Entity(x, y, width, height), speed(speed) {}

void Player::Update(float deltaTime) {}

void Player::Draw() const {
  DrawRectangleV(position, {bounds.width, bounds.height}, YELLOW);
}