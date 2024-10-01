#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "asteroid.h"

const int screenWidth = 600;
const int screenHeight = 600;

// const Vector2 screenSize = { screenWidth, screenHeight };
const Vector2 screenCenter = { screenWidth / 2, screenHeight / 2 };
#define NEARBLACK CLITERAL(Color){15, 15, 15, 255}

#define ASTEROIDS_MAX 64
#define ASTEROID_RANDOM_ANGLE 20 * DEG2RAD
// #define ASTEROID_RANDOM_ANGLE 30 * DEG2RAD
#define ASTEROID_DELAY 0.5f
// #define ASTEROID_DELAY 0.25f

#define ASTEROID_SPEED_MIN 100 // pixels per second
#define ASTEROID_SPEED_MAX 300 // pixels per second

// DEBUG
bool _showDebugMenu = false;
bool _showAsteroidsCount = true;
bool _showAngleCone = false;

Vector2 line0[2];
Vector2 line1[2];
//

static AsteroidSize _sizes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
static Asteroid _asteroids[ASTEROIDS_MAX] = {0};
static float _lastAsteroidCreationTime = -1.0f;
// static float _lastAsteroidCreationTime = 0;

void UpdateDrawFrame();
Vector2 GetNextAsteroidPosition(void);
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
    float time = GetTime();

    for (int i = 0; i < ASTEROIDS_MAX; i++)
    {
        AsteroidUpdate(_asteroids + i, frametime, time); // 111
        // AsteroidUpdate(_asteroids + i, frametime, GetTime()); // ???
    }

    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    if (time > _lastAsteroidCreationTime + ASTEROID_DELAY)
    {
        AsteroidSize nextSize = _sizes[GetRandomValue(0, 3)];
        AddAsteroid(GetNextAsteroidPosition(), nextSize);
        _lastAsteroidCreationTime = time;
    }

    if (IsKeyPressed(KEY_GRAVE)) // it's a "tilda" "~" before "1"
    {
        _showDebugMenu = !_showDebugMenu;
    }

    BeginDrawing();
        ClearBackground(NEARBLACK);

        for (int i = 0; i < ASTEROIDS_MAX; i++)
        {
            AsteroidDraw(_asteroids[i]);
        }

        if (_showAngleCone)
        {
            DrawLineV(line0[0], line0[1], RED);
            DrawLineV(line1[0], line1[1], BLUE);
        }

        if (_showAsteroidsCount)
        {
            int count = 0;
            for (int i = 0; i < ASTEROIDS_MAX; i++)
            {
                if (_asteroids[i].active)
                {
                    count++;
                }
            }

            DrawRectangle(10, 10, 100, 52, Fade(BLACK, 0.6f));
            DrawText(TextFormat("Count: %d", count), 20, 20, 32, WHITE);
        }

        if (_showDebugMenu)
        {
            Rectangle r = {10, screenHeight - 100, 20, 20};
            GuiCheckBox(r, "Show Asteroids count", &_showAsteroidsCount);

            r.y += 30;
            GuiCheckBox(r, "Show Angle Cone", &_showAngleCone);

        }
    EndDrawing();
}

// void AddAsteroid(Vector2 position, Vector2 velocity, AsteroidSize size)
void AddAsteroid(Vector2 position, AsteroidSize size)
{
    // Asteroid asteroid = CreateAsteroid(position, velocity, size);
    bool created = false;
    Vector2 velocity = Vector2Subtract(screenCenter, position);

    velocity = Vector2Scale(Vector2Normalize(velocity), GetRandomValue(ASTEROID_SPEED_MIN, ASTEROID_SPEED_MAX));

    if (_showAngleCone)
    {
        line0[0] = position;
        line1[0] = position;

        line0[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), -ASTEROID_RANDOM_ANGLE));
        line1[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), ASTEROID_RANDOM_ANGLE));
    }

    velocity = Vector2Rotate(velocity, (float)GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE));

    for (int i = 0; i < ASTEROIDS_MAX; i++)
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

Vector2 GetNextAsteroidPosition(void)
{
    int padding = 128;
    Vector2 result = { -padding, - padding };

    if (GetRandomValue(0, 1))
    {
        if (GetRandomValue(0, 1))
        {
            result.y = screenHeight + padding;
        }
        result.x = GetRandomValue(-padding, screenWidth + padding);
    }
    else
    {
        if (GetRandomValue(0, 1))
        {
            result.x = screenWidth + padding;
        }
        result.y = GetRandomValue(-padding, screenHeight + padding);
    }

    TraceLog(LOG_INFO, "Next position: { %d, %d }", (int)result.x, (int)result.y);
    return result;
}

/*

TraceLog(LOG_INFO, "GUI TOGGLE: _showAsteroidsCount is %d", _showAsteroidsCount);

            Rectangle r = {10, screenHeight - 100, 150, 40};
            GuiToggle(r, "Show Asteroids count", &_showAsteroidsCount);

            r.x += 150 + 10;
            GuiToggle(r, "Show Angle Cone", &_showAngleCone);

work perfect:
            Rectangle r = {10, screenHeight - 100, 150, 40};

            GuiToggle(r, "Show Asteroids count", &_showAsteroidsCount);
            r.x += 150 + 10;

            GuiToggle(r, "Show Angle Cone", &_showAngleCone);

            if (GuiButton(r, "Toggle Asteroid Count"))
            {
                _showAsteroidsCount = !_showAsteroidsCount;
            }

            if (GuiButton(r, "Show Asteroid Cone"))
            {
                _showAngleCone = !_showAngleCone;
            }

        AddAsteroid(GetMousePosition(), nextSize);
        TraceLog(LOG_INFO, "Next size: %d", (int)nextSize);

    BAD code -> consume much CPU time:
    if (1919191 % GetRandomValue(2, 499) == 0) {
        AsteroidSize nextSize = _sizes[GetRandomValue(0, 3)];
        AddAsteroid(GetNextAsteroidPosition(), nextSize);
    }
 */