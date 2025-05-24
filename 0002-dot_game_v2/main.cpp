#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

// Global constants
const int screenWidth = 800;
const int screenHeight = 600;

// Base Dot class
class Dot {
protected:
  Vector2 position;
  float radius;
  Color color;

public:
  Dot(Vector2 startPos, float radius, Color color)
      : position(startPos), radius(radius), color(color) {}

  virtual void Control(float deltaTime) {
    // Default behavior: no movement
  }

  void Draw() const { DrawCircleV(position, radius, color); }

  Vector2 GetPosition() const { return position; }

  float GetRadius() const { return radius; }

  void SetPosition(Vector2 newPos) { position = newPos; }
};

// Player class (inherits from Dot)
class Player : public Dot {
private:
  float speed;

public:
  Player(Vector2 startPos, float radius, Color color, float speed)
      : Dot(startPos, radius, color), speed(speed) {}

  virtual ~Player() {}

  void Control(float deltaTime) override {
    if (IsKeyDown(KEY_W))
      position.y -= speed * deltaTime;
    if (IsKeyDown(KEY_S))
      position.y += speed * deltaTime;
    if (IsKeyDown(KEY_A))
      position.x -= speed * deltaTime;
    if (IsKeyDown(KEY_D))
      position.x += speed * deltaTime;

    // Keep player within screen bounds
    if (position.x < radius)
      position.x = radius;
    if (position.x > screenWidth - radius)
      position.x = screenWidth - radius;
    if (position.y < radius)
      position.y = radius;
    if (position.y > screenHeight - radius)
      position.y = screenHeight - radius;
  }
};

// Target class (inherits from Dot)
class Target : public Dot {
public:
  Target(Vector2 startPos, float radius, Color color)
      : Dot(startPos, radius, color) {}

  virtual ~Target() {}

  void Respawn(Vector2 newPos) { position = newPos; }
};

// PositionManager class
class PositionManager {
private:
  std::vector<Dot *> dots;

public:
  void AddDot(Dot *dot) { dots.push_back(dot); }

  bool IsPositionValid(Vector2 newPos, float radius) const {
    for (const Dot *dot : dots) {
      if (CheckCollisionCircles(newPos, radius, dot->GetPosition(),
                                dot->GetRadius())) {
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
};

// Game class
class Game {
private:
  std::unique_ptr<Player> player;
  std::unique_ptr<Target> target;
  PositionManager positionManager;
  int score;

public:
  Game() : score(0) {
    // Initialize player and target
    player = std::make_unique<Player>(
        Vector2{screenWidth / 2.0f, screenHeight / 2.0f}, 15.0f, BLUE, 200.0f);
    target = std::make_unique<Target>(positionManager.GetValidPosition(10.0f),
                                      10.0f, RED);

    // Register dots with the position manager
    positionManager.AddDot(player.get());
    positionManager.AddDot(target.get());
  }

  void Update(float deltaTime) {
    // Update player
    player->Control(deltaTime);

    // Check collision between player and target
    if (!positionManager.IsPositionValid(target->GetPosition(),
                                         target->GetRadius())) {
      score++;
      target->Respawn(positionManager.GetValidPosition(target->GetRadius()));
    }
  }

  void Render() const {
    // Draw game elements
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Catch the moving dot!", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);

    player->Draw();
    target->Draw();

    EndDrawing();
  }

  bool IsRunning() const { return !WindowShouldClose(); }
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
  Game game;
  gameInstance = &game;

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(MainLoop, 0, 1);
#else
  while (game.IsRunning()) {
    float deltaTime = GetFrameTime();
    game.Update(deltaTime);
    game.Render();
  }
#endif

  CloseWindow();

  return 0;
}