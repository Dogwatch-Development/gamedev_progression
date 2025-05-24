#include "raylib.h"
#include <cstdlib>
#include <ctime>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

// Global variables
const int screenWidth = 800;
const int screenHeight = 600;
Vector2 playerPos;
const float playerRadius = 15.0f;
const float playerSpeed = 200.0f;
Vector2 targetPos;
const float targetRadius = 10.0f;
int score;

void GameLoop() {
  // Delta time for smooth movement
  float deltaTime = GetFrameTime();

  // Player movement
  if (IsKeyDown(KEY_W))
    playerPos.y -= playerSpeed * deltaTime;
  if (IsKeyDown(KEY_S))
    playerPos.y += playerSpeed * deltaTime;
  if (IsKeyDown(KEY_A))
    playerPos.x -= playerSpeed * deltaTime;
  if (IsKeyDown(KEY_D))
    playerPos.x += playerSpeed * deltaTime;

  // Keep player within screen bounds
  if (playerPos.x < playerRadius)
    playerPos.x = playerRadius;
  if (playerPos.x > screenWidth - playerRadius)
    playerPos.x = screenWidth - playerRadius;
  if (playerPos.y < playerRadius)
    playerPos.y = playerRadius;
  if (playerPos.y > screenHeight - playerRadius)
    playerPos.y = screenHeight - playerRadius;

  // Collision detection
  if (CheckCollisionCircles(playerPos, playerRadius, targetPos, targetRadius)) {
    score++;
    targetPos = {static_cast<float>(rand() % screenWidth),
                 static_cast<float>(rand() % screenHeight)};
  }

  // Drawing
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawText("Catch the moving dot!", 10, 10, 20, DARKGRAY);
  DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);

  DrawCircleV(playerPos, playerRadius, BLUE);
  DrawCircleV(targetPos, targetRadius, RED);

  EndDrawing();
}

int main() {
  // Initialization
  InitWindow(screenWidth, screenHeight, "Dot Game - Catch the Dot");
  SetTargetFPS(60);

  // Player dot
  playerPos = {screenWidth / 2.0f, screenHeight / 2.0f};

  // Target dot
  targetPos = {static_cast<float>(rand() % screenWidth),
               static_cast<float>(rand() % screenHeight)};

  // Score
  score = 0;

  // Seed random number generator
  std::srand(std::time(nullptr));

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(GameLoop, 0, 1);
#else
  while (!WindowShouldClose()) {
    GameLoop();
  }
#endif

  CloseWindow();

  return 0;
}