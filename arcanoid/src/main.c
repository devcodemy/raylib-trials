#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "common.h"

// static bool _followBall = false;
static bool _followBall = true;
static bool _showDebugInfo = false;
static bool _bottomJumpForced = false;

// static bool brickBreak = false;
static bool jump = false;
static bool back = false;

static Brick _bricks[BRICK_MAX] = {0};
static float _lastPlatformUpdate = 0.04f;

const int brickWidth = 75;  // SCREEN_WIDTH / 8;
const int brickHeight = 35; //(int)brickkWidth / 5;
// const int brickHeight = 15; //(int)brickkWidth / 5;

const int platformWidth = SCREEN_WIDTH / 5; // div by 8 = 75
const int platformHeight = brickWidth / 5;

const int ballRadius = platformHeight / 2;
const int ballStartVelX = 10;
const int ballStartVelY = -8;

void MakeWall()
{
    int idx = 0, mutX = 8, mutY = 8;
    for (int j = 1; j < BRICK_MAX + 1; j++)
    {
        if (j > 1)
        {
            _bricks[idx] = CreateBrick(mutX, mutY, 1, (Size2d){brickWidth, brickHeight}, true);
        }
        mutX += brickWidth + PADDING * 2;
        // mutX += 85;

        if (j % MAGIC_EIGHT == 0) // 8 it is about screen width / brick width
        {
            mutX = 8, mutY += brickHeight + PADDING * 2;
            // mutX = 8, mutY += 25;
        }

        idx++;
    }
    TraceLog(LOG_INFO, "_MUT_: [%d, %d]", mutX, mutY);
}

int main(void)
{
    srand(time(0));
    InitWindow(SCREEN_WIDTH, SCREEN_DEPTH, "Arcanoid [Raylib]");
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
        .posX = SCREEN_WIDTH / 2,
        .posY = SCREEN_DEPTH / 2,
        .velX = ballStartVelX,
        .velY = ballStartVelY,
        .radius = ballRadius,
    };

    for (int i = 0; i < BRICK_MAX; i++)
    {
        TraceLog(LOG_INFO, "[%d] BRICK: [%d, %d] %s", i, _bricks[i].posX, _bricks[i].posY, _bricks[i].active ? "active" : "disabled");
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

/*

    for (int i = 0; i < BRICK_MAX; i++){
        TraceLog(
            LOG_INFO,
            "[%d] BRICK: [%d, %d] %s",
             i,
             _bricks[i].posX,
             _bricks[i].posY,
             _bricks[i].active ? "active" : "disabled"
        );
    }

 */

bool UpdateDrawFrame(Platform *plt, Ball *ball)
{
    float frametime = GetFrameTime();
    float time = GetTime();

    BeginDrawing();
    ClearBackground(NEARBLACK);

    // DRAW BRICKS
    for (int i = 0; i < BRICK_MAX; i++) // 5 bricks are not drawing ??!!
    {
        BrickDraw(&_bricks[i]);
        // BrickCollision(&_bricks[i], ball);
    }

    // DRAW BALL // UPDATE BALL POSITION
    DrawCircle(ball->posX, ball->posY, ballRadius, RED);


    if (time > _lastPlatformUpdate + SOME_DELAY)
    {
        for (int i = 0; i < BRICK_MAX; i++) // 5 bricks are not drawing ??!!
        {
            BrickCollision(&_bricks[i], ball);
        }

        // DRAW PLATFORM // UPDATE PLATFORM POSITION
        DrawRectangle(plt->posX, plt->posY, platformWidth, platformHeight, GREEN);
        back = UpdatePlatform(plt, back, _followBall, ball);
        // UPDATE BALL IF PLATFORM
        UpdateBall(ball);
        jump = PlatformCollision(plt, ball, jump);
        _lastPlatformUpdate = time;
    }

    if (ball->posY > SCREEN_DEPTH)
    {
        return false;
    } // GAME OVER

    if (IsKeyPressed(KEY_EQUAL))
    {
        _followBall = !_followBall;
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

void UpdateBall(Ball *ball)
{
    // if (ball->posX >= SCREEN_WIDTH - PADDING || ball->posX <= PADDING)
    if (ball->posX + ball->radius >= SCREEN_WIDTH || ball->posX <= ball->radius)
    {
        ball->velX = -ball->velX;
    }

    // if (ball->posY <= PADDING)
    if (ball->posY - ball->radius <= ball->radius)
    {
        ball->velY = -ball->velY;
    }

    // if (ball->posY >= SCREEN_DEPTH - 10 && _bottomJumpForced)
    if (ball->posY + ball->radius >= SCREEN_DEPTH - ball->radius && _bottomJumpForced)
    {
        ball->velY = -ball->velY;
    }
    // TBD: update if out of bottom border GAME OVER

    ball->posX = ball->posX + ball->velX;
    ball->posY = ball->posY + ball->velY;
}