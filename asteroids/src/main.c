#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "asteroid.h"

const int screenWidth = 600;
const int screenHeight = 600;

const Vector2 screenSize = { screenWidth, screenHeight };
#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

#define MAX_ASTEROIDS 64
static AsteroidSize _sizes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
static Asteroid _asteroids[MAX_ASTEROIDS] = {0};

void UpdateDrawFrame();
void AddAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size);

int main(void)
{
    srand(time(0));
    InitWindow(screenWidth, screenHeight, "Raylib Asteroids Part I");

    while(!WindowShouldClose())
    {
        UpdateDrawFrame();
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
        AddAsteroid(GetMousePosition(), (Vector2){200, 0}, nextSize);
    }

    BeginDrawing();
        ClearBackground(NEARBLACK);

        for (int i = 0; i < MAX_ASTEROIDS; i++)
        {
            AsteroidDraw(_asteroids[i]);
        }

    EndDrawing();
}

void AddAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size)
{
    Asteroid asteroid = CreateAsteroid(position, velocity, size);

    bool created = false;

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
