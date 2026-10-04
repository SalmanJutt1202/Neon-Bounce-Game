#include "raylib.h"

int main() {
    InitWindow(800, 450, "My First Game");
    SetTargetFPS(60);

    float x = 400, y = 225;          // player position

    while (!WindowShouldClose()) {   // the GAME LOOP
        // 1. INPUT + UPDATE
        if (IsKeyDown(KEY_RIGHT)) x += 4;
        if (IsKeyDown(KEY_LEFT))  x -= 4;
        if (IsKeyDown(KEY_DOWN))  y += 4;
        if (IsKeyDown(KEY_UP))    y -= 4;

        // 2. DRAW
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawRectangle((int)x, (int)y, 40, 40, ORANGE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}