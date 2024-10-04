#include "raylib.h"
#include "common.h"

bool UpdatePlatform(Platform *plt, bool back, bool followBall, Ball *ball)
{
    if (!followBall)
    {
        if (!back && plt->posX < 5 || !back && plt->posX > SCREEN_WIDTH - plt->width)
        {
            plt->speed = -plt->speed;
            plt->posX = plt->posX + plt->speed;
            return true;
        }

        plt->posX = plt->posX + plt->speed;
    } 
    else
    {
        plt->posX = ball->position.x - plt->width / 2;
    }
    return false;
}

bool PlatformCollision(Platform *plt, Ball *ball, bool jump)
{
    if (ball->position.x >= plt->posX  && ball->position.x <= plt->posX + plt->width)
    {
        if (ball->position.y >= plt->posY && ball->position.y <= plt->posY + plt->height / 2 && !jump)
        {
            ball->velocity.y = -ball->velocity.y;
            return true;
        }
    }

    return false;
}