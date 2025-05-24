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
};

// PositionManager class
class PositionManager {
private:
  std::vector<Dot *> dots;

public:
  void AddDot(Dot *dot) { dots.push_back(dot); }

  void Update(float deltaTime) {
    // Check for collisions between all dots
    for (size_t i = 0; i < dots.size(); ++i) {
      for (size_t j = i + 1; j < dots.size(); ++j) {
        Dot *dotA = dots[i];
        Dot *dotB = dots[j];

        if (CheckCollisionCircles(dotA->GetPosition(), dotA->GetRadius(),
                                  dotB->GetPosition(), dotB->GetRadius())) {
          // Notify both dots of the collision
          dotA->HandleCollision(dotB);
          dotB->HandleCollision(dotA);
        }
      }
    }

    // Let each dot update its position
    for (Dot *dot : dots) {
      dot->Control(deltaTime, *this);
    }
  }

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
};

// Player class (inherits from Dot)
class Player : public Dot {
private:
  float speed;

public:
  Player(Vector2 startPos, float radius, Color color, float speed)
      : Dot(startPos, radius, color), speed(speed) {}

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
      : Dot(startPos, radius, color) {}

  virtual ~Target() {}

  void HandleCollision(Dot *other) override {
    // Respawn the target at a new valid position
    position = {static_cast<float>(rand() % screenWidth),
                static_cast<float>(rand() % screenHeight)};
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
    // Delegate all updates to the PositionManager
    positionManager.Update(deltaTime);
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