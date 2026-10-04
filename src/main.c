#include "raylib.h"
#include <math.h>

#define TRAIL 18

int main(void) {
    const int W = 900, H = 720;
    InitWindow(W, H, "Neon Bounce");
    SetTargetFPS(60);

    Rectangle paddle = { W / 2 - 60, H - 50, 120, 16 };
    Vector2 ball = { W / 2, H / 2 };
    Vector2 vel = { 3.5f, 6 };
    float radius = 10;
    float speed = 7;
    int score = 0, best = 0, flash = 0;

    Vector2 trail[TRAIL];
    for (int i = 0; i < TRAIL; i++) trail[i] = ball;

    while (!WindowShouldClose()) {
        // paddle
        float move = IsKeyDown(KEY_LEFT_SHIFT) ? 12 : 8;
        if (IsKeyDown(KEY_D)) paddle.x += move;
        if (IsKeyDown(KEY_A)) paddle.x -= move;
        if (paddle.x < 0) paddle.x = 0;
        if (paddle.x + paddle.width > W) paddle.x = W - paddle.width;

        // ball
        ball.x += vel.x;
        ball.y += vel.y;
        if (ball.x < radius) { ball.x = radius;     vel.x *= -1; }
        if (ball.x > W - radius) { ball.x = W - radius; vel.x *= -1; }
        if (ball.y < radius) { ball.y = radius;     vel.y *= -1; }

        // paddle hit: the bounce angle depends on where it lands
        if (CheckCollisionCircleRec(ball, radius, paddle) && vel.y > 0) {
            float hit = (ball.x - (paddle.x + paddle.width / 2)) / (paddle.width / 2);
            if (hit < -1) hit = -1;
            if (hit > 1)  hit = 1;

            speed += 0.5f;
            if (speed > 14) speed = 14;

            vel.x = hit * speed * 0.8f;
            vel.y = -sqrtf(speed * speed - vel.x * vel.x);
            ball.y = paddle.y - radius;

            score++;
            if (score > best) best = score;
            flash = 8;
        }
        if (flash > 0) flash--;

        // missed: reset
        if (ball.y > H + 40) {
            ball = (Vector2){ W / 2, H / 2 };
            speed = 7;
            vel = (Vector2){ GetRandomValue(0, 1) ? 3.5f : -3.5f, 6 };
            score = 0;
        }

        // trail: shift everything back, newest first
        for (int i = TRAIL - 1; i > 0; i--) trail[i] = trail[i - 1];
        trail[0] = ball;

        // draw
        BeginDrawing();
        DrawRectangleGradientV(0, 0, W, H, (Color) { 18, 14, 43, 255 }, (Color) { 52, 28, 110, 255 });

        for (int y = 0; y < H; y += 30)
            DrawRectangle(W / 2 - 2, y, 4, 16, Fade(WHITE, 0.08f));

        const char* s = TextFormat("%d", score);
        DrawText(s, (W - MeasureText(s, 160)) / 2, H / 2 - 80, 160, Fade(WHITE, 0.12f));

        for (int i = 0; i < TRAIL; i++) {
            float t = 1.0f - i / (float)TRAIL;
            DrawCircleV(trail[i], radius * t, Fade((Color) { 255, 200, 60, 255 }, 0.35f * t));
        }
        DrawCircleV(ball, radius * 2.4f, Fade(YELLOW, 0.10f));
        DrawCircleV(ball, radius * 1.7f, Fade(YELLOW, 0.18f));
        DrawCircleV(ball, radius, (Color) { 255, 225, 100, 255 });

        Rectangle glow = { paddle.x - 4, paddle.y - 4, paddle.width + 8, paddle.height + 8 };
        DrawRectangleRounded(glow, 1.0f, 8, Fade(SKYBLUE, 0.25f));
        DrawRectangleRounded(paddle, 1.0f, 8, flash > 0 ? WHITE : (Color) { 120, 220, 255, 255 });

        DrawText(TextFormat("Best: %d", best), 20, 20, 24, Fade(WHITE, 0.7f));
        DrawText("A / D to move   SHIFT to sprint", 20, H - 30, 18, Fade(WHITE, 0.4f));
        EndDrawing();
    }

    CloseWindow();
    return 0;
}