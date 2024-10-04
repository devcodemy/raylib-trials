#include "raylib.h"
#include "common.h"

bool UpdatePlatform(Platform *plt, bool back)
{
    if (!back && plt->posX < 5 || !back && plt->posX> SCREEN_WIDTH - plt->width)
    {
        plt->speed = -plt->speed;
        plt->posX = plt->posX + plt->speed;
        // TraceLog(LOG_INFO, "BACK");
        return true;
    }

    plt->posX = plt->posX + plt->speed;
    return false;
}

bool PlatformCollision(Platform *plt, Ball *ball, bool jump)
{
    int minX = plt->posX;
    int maxX = plt->posX + plt->width;

    int minY = plt->posY;
    int maxY = plt->posY + 3;

    if (ball->position.x > minX && ball->position.x < maxX)
    {
        if (ball->position.y >= minY && ball->position.y < maxY && !jump)
        {
            ball->velocity.y = -ball->velocity.y;
            // TraceLog(LOG_INFO, "JUMP");
            return true;
        }
    }

    return false;
}