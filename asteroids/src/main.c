#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "asteroid.h"

const int screenWidth = 600;
const int screenHeight = 600;

// const Vector2 screenSize = { screenWidth, screenHeight };
const Vector2 screenCenter = { screenWidth / 2, screenHeight / 2 };
#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

#define MAX_ASTEROIDS 64
#define ASTEROID_RANDOM_ANGLE 30 * DEG2RAD
#define ASTEROID_SPEED_MIN 100 // pixels per second
#define ASTEROID_SPEED_MAX 300 // pixels per second

// DEBUG
bool _showAngleCone = false;
Vector2 line0[2];
Vector2 line1[2];
//

static AsteroidSize _sizes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
static Asteroid _asteroids[MAX_ASTEROIDS] = {0};

void UpdateDrawFrame();
void AddAsteroid(Vector2 position, AsteroidSize size);
// void AddAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size);

int main(void)
{
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "Raylib Asteroids Part I");

    while(!WindowShouldClose())
    {
        UpdateDrawFrame();
        if (IsKeyPressed(KEY_Q))
        {
            break;
        }
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
    float frametime = GetFrameTime();

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        AsteroidUpdate(_asteroids + i, frametime);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        AsteroidSize nextSize = _sizes[GetRandomValue(0, 3)];
        TraceLog(LOG_INFO, "Next size: %d", (int)nextSize);
        AddAsteroid(GetMousePosition(), nextSize);
        // AddAsteroid(GetMousePosition(), (Vector2){200, 0}, nextSize);
    }

    BeginDrawing();
        ClearBackground(NEARBLACK);

        for (int i = 0; i < MAX_ASTEROIDS; i++)
        {
            AsteroidDraw(_asteroids[i]);
        }

        if (_showAngleCone)
        {
            DrawLineV(line0[0], line0[1], RED);
            DrawLineV(line1[0], line1[1], BLUE);
        }

    EndDrawing();
}

// void AddAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size)
void AddAsteroid(Vector2 position, AsteroidSize size)
{
    // Asteroid asteroid = CreateAsteroid(position, velocity, size);
    bool created = false;
    Vector2 velocity = Vector2Subtract(screenCenter, position);

    velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(ASTEROID_SPEED_MIN, ASTEROID_SPEED_MIN));

    if (_showAngleCone)
    {
        line0[0] = position;
        line1[0] = position;

        line0[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), -ASTEROID_RANDOM_ANGLE));
        line1[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), ASTEROID_RANDOM_ANGLE));
    }

    velocity = Vector2Rotate(velocity, (float)GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE));

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (_asteroids[i].active)
        {
            continue;
        }

        _asteroids[i] = CreateAsteroid(position, velocity, size);
        created = true;
        break; // fine next innactive spot and create; and quit;
    }

    if (!created)
    {
        TraceLog(LOG_ERROR, "AAAA not created; no innactive spots in _array");
    }
}
