#include "raylib.h"
#include <cstdlib>
#include <ctime>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

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
  InitWindow(screenWidth, screenHeight, "Dot Game - Catch the Dot");
  SetTargetFPS(60);

  // Seed random number generator
  std::srand(std::time(nullptr));

#ifdef PLATFORM_WEB
  emscripten_set_main_loop(MainLoop, 0, 1);
#else
  while (!WindowShouldClose()) {
    MainLoop();
  }
#endif

  CloseWindow();

  return 0;
}