#include "raylib.h"
#include "raymath.h" // Add this for vector math
#include <cstdlib>
#include <ctime>
#include <functional>
#include <memory>
#include <vector>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

// Global constants
const int screenWidth = 800;
const int screenHeight = 600;

// Base Dot class
enum class DotType { Player, Target, Enemy, Other };

class Dot {
protected:
  Vector2 position;
  float radius;
  Color color;
  DotType type;

public:
  Dot(Vector2 startPos, float radius, Color color, DotType type)
      : position(startPos), radius(radius), color(color), type(type) {}

  virtual void Control(float deltaTime,
                       class PositionManager &positionManager) {
    // Default behavior: no movement
  }

  virtual void HandleCollision(Dot *other) {
    // Default behavior: do nothing
  }

  void Draw() const { DrawCircleV(position, radius, color); }

  Vector2 GetPosition() const { return position; }

  float GetRadius() const { return radius; }

  void SetPosition(Vector2 newPos) { position = newPos; }

  DotType GetType() const { return type; }

protected:
  static Vector2 Vector2WeightedAttraction(Vector2 from, Vector2 to,
                                           float threshold, float weight) {
    Vector2 dir = Vector2Subtract(to, from);
    float dist = Vector2Length(dir);
    if (dist < 0.01f || dist > threshold)
      return {0, 0};
    dir = Vector2Scale(dir, 1.0f / dist); // normalize
    float strength = weight * (threshold - dist) / threshold;
    return Vector2Scale(dir, strength);
  }
};

// Forward declaration
class Enemy;

// PositionManager class
class PositionManager {
private:
  struct DotEntry {
    Dot *dot;
    DotType type;
  };
  std::vector<DotEntry> dots;
  std::function<void()> onScoreIncrement;
  std::function<void()> onGameOver;

public:
  void AddDot(Dot *dot) { dots.push_back({dot, dot->GetType()}); }

  void SetScoreIncrementCallback(const std::function<void()> &callback) {
    onScoreIncrement = callback;
  }

  void SetGameOverCallback(const std::function<void()> &callback) {
    onGameOver = callback;
  }

  void Update(float deltaTime) {
    // Check for collisions between all dots
    for (size_t i = 0; i < dots.size(); ++i) {
      for (size_t j = i + 1; j < dots.size(); ++j) {
        Dot *dotA = dots[i].dot;
        Dot *dotB = dots[j].dot;
        DotType typeA = dots[i].type;
        DotType typeB = dots[j].type;
        if (CheckCollisionCircles(dotA->GetPosition(), dotA->GetRadius(),
                                  dotB->GetPosition(), dotB->GetRadius())) {
          dotA->HandleCollision(dotB);
          dotB->HandleCollision(dotA);

          // Score increment if player and target collide
          if (onScoreIncrement &&
              ((typeA == DotType::Player && typeB == DotType::Target) ||
               (typeA == DotType::Target && typeB == DotType::Player))) {
            onScoreIncrement();
          }
          // Game over if player and enemy collide
          if (onGameOver &&
              ((typeA == DotType::Player && typeB == DotType::Enemy) ||
               (typeA == DotType::Enemy && typeB == DotType::Player))) {
            onGameOver();
          }
        }
      }
    }
    for (auto &entry : dots) {
      entry.dot->Control(deltaTime, *this);
    }
  }

  bool IsPositionValid(Vector2 newPos, float radius) const {
    for (const auto &entry : dots) {
      if (CheckCollisionCircles(newPos, radius, entry.dot->GetPosition(),
                                entry.dot->GetRadius())) {
        return false;
      }
    }
    return true;
  }

  Vector2 GetValidPosition(float radius) {
    Vector2 newPos;
    do {
      newPos = {static_cast<float>(rand() % screenWidth),
                static_cast<float>(rand() % screenHeight)};
    } while (!IsPositionValid(newPos, radius));
    return newPos;
  }

  Vector2 UpdatePosition(Dot *dot, Vector2 newPos, float radius) {
    // Enforce screen bounds
    if (newPos.x < radius)
      newPos.x = radius;
    if (newPos.x > screenWidth - radius)
      newPos.x = screenWidth - radius;
    if (newPos.y < radius)
      newPos.y = radius;
    if (newPos.y > screenHeight - radius)
      newPos.y = screenHeight - radius;

    return newPos;
  }

  Vector2 GetPlayerPosition() const {
    for (const auto &entry : dots) {
      if (entry.type == DotType::Player)
        return entry.dot->GetPosition();
    }
    return {0, 0};
  }
};

// Player class (inherits from Dot)
class Player : public Dot {
private:
  float speed;

public:
  Player(Vector2 startPos, float radius, Color color, float speed)
      : Dot(startPos, radius, color, DotType::Player), speed(speed) {}

  virtual ~Player() {}

  void Control(float deltaTime, PositionManager &positionManager) override {
    Vector2 newPos = position;

    if (IsKeyDown(KEY_W))
      newPos.y -= speed * deltaTime;
    if (IsKeyDown(KEY_S))
      newPos.y += speed * deltaTime;
    if (IsKeyDown(KEY_A))
      newPos.x -= speed * deltaTime;
    if (IsKeyDown(KEY_D))
      newPos.x += speed * deltaTime;

    // Update position using the PositionManager
    position = positionManager.UpdatePosition(this, newPos, radius);
  }

  void HandleCollision(Dot *other) override {
    // Player-specific collision handling (e.g., no special behavior for now)
  }
};

// Target class (inherits from Dot)
class Target : public Dot {
public:
  Target(Vector2 startPos, float radius, Color color)
      : Dot(startPos, radius, color, DotType::Target) {}

  virtual ~Target() {}

  void Control(float deltaTime, PositionManager &positionManager) override {
    // Strong repulsion from player only
    Vector2 playerPos = positionManager.GetPlayerPosition();
    Vector2 repulsion =
        Vector2WeightedAttraction(position, playerPos, 200.0f, -400.0f);

    // Use repulsion as velocity
    Vector2 velocity = repulsion;

    // Limit speed
    float maxSpeed = 160.0f;
    if (Vector2Length(velocity) > maxSpeed) {
      velocity = Vector2Scale(Vector2Normalize(velocity), maxSpeed);
    }

    // Move target
    Vector2 newPos = Vector2Add(position, Vector2Scale(velocity, deltaTime));
    position = positionManager.UpdatePosition(this, newPos, radius);
  }

  void HandleCollision(Dot *other) override {
    // Respawn the target at a new valid position
    position = {static_cast<float>(rand() % screenWidth),
                static_cast<float>(rand() % screenHeight)};
  }
};

// Enemy class (inherits from Dot)
class Enemy : public Dot {
private:
  float speed;

public:
  Enemy(Vector2 startPos, float radius, Color color, float speed)
      : Dot(startPos, radius, color, DotType::Enemy), speed(speed) {}

  virtual ~Enemy() {}

  void Control(float deltaTime, PositionManager &positionManager) override {
    // Move towards the player
    Vector2 playerPos = positionManager.GetPlayerPosition();
    Vector2 direction = Vector2Subtract(playerPos, position);
    float dist = Vector2Length(direction);
    if (dist > 0.01f) {
      direction = Vector2Scale(direction, 1.0f / dist); // normalize
      Vector2 velocity = Vector2Scale(direction, speed);
      Vector2 newPos = Vector2Add(position, Vector2Scale(velocity, deltaTime));
      position = positionManager.UpdatePosition(this, newPos, radius);
    }
  }

  void HandleCollision(Dot *other) override {
    // Enemy-specific collision handling (e.g., could end game if collides with
    // player)
  }
};

// Game class
class Game {
private:
  std::unique_ptr<Player> player;
  std::unique_ptr<Target> target;
  std::unique_ptr<Enemy> enemy;
  PositionManager positionManager;
  int score;
  bool gameOver;

  void InitGameObjects() {
    // Initialize player and target
    player = std::make_unique<Player>(
        Vector2{screenWidth / 2.0f, screenHeight / 2.0f}, 15.0f, BLUE, 200.0f);
    target = std::make_unique<Target>(positionManager.GetValidPosition(10.0f),
                                      10.0f, RED);
    enemy = std::make_unique<Enemy>(positionManager.GetValidPosition(12.0f),
                                    12.0f, DARKGREEN, 120.0f);

    // Register dots with the position manager
    positionManager = PositionManager(); // Reset position manager
    positionManager.AddDot(player.get());
    positionManager.AddDot(target.get());
    positionManager.AddDot(enemy.get());

    // Set the score increment callback
    positionManager.SetScoreIncrementCallback([this]() { score++; });
    // Set the game over callback
    positionManager.SetGameOverCallback([this]() { gameOver = true; });
  }

public:
  Game() : score(0), gameOver(false) { InitGameObjects(); }

  void Reset() {
    score = 0;
    gameOver = false;
    InitGameObjects();
  }

  void Update(float deltaTime) {
    if (!gameOver) {
      // Delegate all updates to the PositionManager
      positionManager.Update(deltaTime);
    } else {
      if (IsKeyPressed(KEY_R)) {
        Reset();
      }
    }
  }

  void Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameOver) {
      DrawText("Game Over!", screenWidth / 2 - 100, screenHeight / 2 - 40, 40,
               RED);
      DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - 100,
               screenHeight / 2 + 10, 30, DARKGRAY);
      DrawText("Press R to Restart", screenWidth / 2 - 120,
               screenHeight / 2 + 60, 28, DARKBLUE);
    } else {
      DrawText("Catch the moving dot!", 10, 10, 20, DARKGRAY);
      DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);
      player->Draw();
      target->Draw();
      enemy->Draw();
    }
    EndDrawing();
  }
};

Game *gameInstance = nullptr;

void MainLoop() {
  float deltaTime = GetFrameTime();
  gameInstance->Update(deltaTime);
  gameInstance->Render();
}

int main() {
  // Initialization
  InitWindow(screenWidth, screenHeight, "Dot Game - Catch the Dot");
  SetTargetFPS(60);

  // Seed random number generator
  std::srand(std::time(nullptr));

  // Create game object
  static Game game;
  gameInstance = &game;

#ifndef PLATFORM_WEB
  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    game.Update(deltaTime);
    game.Render();
  }
#else
  emscripten_set_main_loop(MainLoop, 0, 1);
#endif

  CloseWindow();

  return 0;
}