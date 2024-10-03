#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#define CELL_COUNT 99
#define SOME_DELAY 0.025f

#define NEARBLACK \
    CLITERAL(Color) { 15, 15, 15, 255 }

const int screenWidth = 600;
const int screenHeight = 400;

static float _lastPlatformUpdate = 0.04f;

const int cellWidth = screenWidth / 10;
const int cellHeight = cellWidth / 5;

const int platformWidth = screenWidth / 9;
const int platformHeight = cellWidth / 5;

const int ballRadius = platformHeight / 2;

const Vector2 cellSize = { cellWidth, cellHeight };

typedef struct Cell {
    Vector2 position;
    int level;
    bool active;
} Cell;

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    // Vector2 velocity;
} Ball;

typedef struct Platform {
    int posX;
    int posY;
    int speed;
} Platform;

void UpdatePlatform(Platform *plt);
void UpdateDrawFrame(Platform *plt, Ball *ball);
void UpdateBall(Platform *plt, Ball *ball);

int main(void) {
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "Raylib arcanoid (my own implementation)");

    Platform plt = (Platform){
        .posX = 15,
        .posY = screenHeight - 20,
        .speed = 10,
    };

    Ball ball = (Ball){
        // .position = { plt.posX + platformWidth / 2, plt.posY + ballRadius },
        .position = { screenWidth /2, screenHeight / 2 },
        .velocity = { 3, -3 },
    };

    while (!WindowShouldClose())
    {
        UpdateDrawFrame(&plt, &ball);
        if (IsKeyPressed(KEY_Q))
        {
            break;
        }
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(Platform *plt, Ball *ball)
{
    float frametime = GetFrameTime();
    float time = GetTime();

    BeginDrawing();
        ClearBackground(NEARBLACK);

        DrawRectangle(20, 20, cellWidth, cellHeight, WHITE);

        // DrawCircle(screenWidth - 50, screenHeight - 50, 5, RED);
        DrawCircle(ball->position.x, ball->position.y, ballRadius, RED);

        DrawRectangle(plt->posX, plt->posY, platformWidth, platformHeight, GREEN);

        if (time > _lastPlatformUpdate + SOME_DELAY)
        {
            UpdatePlatform(plt);
            UpdateBall(plt, ball);
            _lastPlatformUpdate = time;
        }

        if (IsKeyPressed(KEY_A) && plt->speed > 0)
        {
            plt->speed = -plt->speed;
        }

        if (IsKeyPressed(KEY_D) && plt->speed < 0)
        {
            plt->speed = -plt->speed;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            plt->speed = -plt->speed;
        }

    EndDrawing();
}

void UpdatePlatform(Platform *plt)
{
    if (plt->posX >= screenWidth - platformWidth - 10 || plt->posX <= 10)
    {
        plt->speed = -plt->speed;
    }

    plt->posX = plt->posX + plt->speed;
}

void UpdateBall(Platform *plt, Ball *ball)
{
    if (ball->position.x >= screenWidth - 5 || ball->position.x <= 5)
    {
        ball->velocity.x = -ball->velocity.x;
    }

    // if (ball->position.y <= 5)
    if (ball->position.y <= 5 || ball->position.y >= screenHeight - 20)
    {
        ball->velocity.y = -ball->velocity.y;
    }

    ball->position.x = ball->position.x + ball->velocity.x;
    ball->position.y = ball->position.y + ball->velocity.y;
}

/*

TraceLog(LOG_INFO, "PLT %d", plt->posX);

 */
