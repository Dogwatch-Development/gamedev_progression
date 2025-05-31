#include "loop.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>

// Global constants
const int screenWidth = 800;
const int screenHeight = 600;

void MainLoop() {
  ClearBackground(BLACK);
  BeginDrawing();
  const char text[] = "EUREKA!";
  int fontSize = 120;
  int centerX = screenWidth / 2;
  int centerY = screenHeight / 2;
  DrawText(text, centerX - (MeasureText(text, fontSize) / 2),
           centerY - (fontSize / 2), fontSize, RAYWHITE);
  EndDrawing();
}

int main() {
  // Initialization
  InitWindow(screenWidth, screenHeight, "Avoid the Walls V2");
  SetTargetFPS(60);

  // Seed random number generator
  std::srand(std::time(nullptr));

  RunPlatformLoop(MainLoop);

  CloseWindow();

  return 0;
}