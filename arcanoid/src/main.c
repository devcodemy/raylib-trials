#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "block.h"
#include "common.h"

static bool _showDebugInfo = false;
static bool _bottomJumpForced = false;

static bool jump = false;
static bool back = false;

static Block _blocks[BLOCK_MAX] = {0};
static float _lastPlatformUpdate = 0.04f;

const int blockWidth = 75;  // SCREEN_WIDTH / 8;
const int blockHeight = 15; //(int)blockWidth / 5;

const int platformWidth = SCREEN_WIDTH / 5; // div by 8 = 75
const int platformHeight = blockWidth / 5;

const int ballRadius = platformHeight / 2;
const Vector2 ballStartVelocity = {8, -6};

void MakeWall()
{
    int idx = 0, mutX = 8, mutY = 8;
    for (int j = 1; j < BLOCK_MAX + 1; j++)
    {
        if (j > 1)
        {
            _blocks[idx] = CreateBlock(mutX, mutY, 1, true);
        }
        mutX += 85;

        if (j % MAGIC_EIGHT == 0) // 8 it is about screen width / block width
        {
            mutX = 8, mutY += 25;
        }

        idx++;
    }
    TraceLog(LOG_INFO, "_MUT_: [%d, %d]", mutX, mutY);
}

int main(void)
{
    srand(time(0));
    InitWindow(SCREEN_WIDTH, SCREEN_DEPTH, "Raylib arcanoid (my own implementation)");
    SetTargetFPS(30);

    MakeWall();

    Platform plt = (Platform){
        .posX = SCREEN_WIDTH / 2,
        .posY = SCREEN_DEPTH - 20,
        .width = platformWidth,
        .height = platformHeight,
        .speed = 10,
    };

    Ball ball = (Ball){
        .position = {SCREEN_WIDTH / 2, SCREEN_DEPTH / 2},
        .velocity = ballStartVelocity,
        .radius = ballRadius,
    };

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        TraceLog(LOG_INFO, "[%d] BLOCK: [%d, %d] %s", i, _blocks[i].posX, _blocks[i].posY, _blocks[i].active ? "active" : "disabled");
    }

    while (!WindowShouldClose())
    {
        // UpdateDrawFrame(&plt, &ball);
        // TBD: update if out of bottom border GAME OVER
        if (IsKeyPressed(KEY_Q) || !UpdateDrawFrame(&plt, &ball))
        {
            break;
        }
    }

    CloseWindow();
    return 0;
}

bool UpdateDrawFrame(Platform *plt, Ball *ball)
{
    float frametime = GetFrameTime();
    float time = GetTime();

    BeginDrawing();
    ClearBackground(NEARBLACK);

    // DRAW BLOCKS
    for (int i = 0; i < BLOCK_MAX; i++) // 5 blocks are not drawing ??!!
    {
        BlockDraw(&_blocks[i]);
    }

    // DRAW BALL // UPDATE BALL POSITION
    DrawCircle(ball->position.x, ball->position.y, ballRadius, RED);

    // DRAW PLATFORM // UPDATE PLATFORM POSITION
    DrawRectangle(plt->posX, plt->posY, platformWidth, platformHeight, GREEN);
    jump = PlatformCollision(plt, ball, jump);

    if (time > _lastPlatformUpdate + SOME_DELAY)
    {
        back = UpdatePlatform(plt, back);
        UpdateBall(plt, ball);
        _lastPlatformUpdate = time;
    }

    if (ball->position.y > SCREEN_DEPTH)
    {
        return false;
    } // GAME OVER

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

    if (IsKeyPressed(KEY_GRAVE))
    {
        _showDebugInfo = !_showDebugInfo;
        _bottomJumpForced = !_bottomJumpForced;
    }

    if (IsKeyPressed(KEY_MINUS))
    {
        _showDebugInfo = !_showDebugInfo;
        _bottomJumpForced = !_bottomJumpForced;
    }

    if (_showDebugInfo)
    {
        DrawFPS(10, 10);
        if (_bottomJumpForced)
        {
            DrawRectangle(5, SCREEN_DEPTH - 5, SCREEN_WIDTH - 10, 2, LIGHTGRAY);
        }
    }

    EndDrawing();
    return true;
}

void UpdateBall(Platform *plt, Ball *ball)
{
    if (ball->position.x >= SCREEN_WIDTH - PADDING || ball->position.x <= PADDING)
    {
        ball->velocity.x = -ball->velocity.x;
    }

    if (ball->position.y <= PADDING)
    {
        ball->velocity.y = -ball->velocity.y;
    }

    if (ball->position.y >= SCREEN_DEPTH - 10 && _bottomJumpForced)
    {
        ball->velocity.y = -ball->velocity.y;
    }
    // TBD: update if out of bottom border GAME OVER

    ball->position.x = ball->position.x + ball->velocity.x;
    ball->position.y = ball->position.y + ball->velocity.y;
}
