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
        plt->posX = ball->posX - plt->width / 2;
    }
    return false;
}

bool PlatformCollision(Platform *plt, Ball *ball, bool jump)
{
    if (ball->posX >= plt->posX  && ball->posX <= plt->posX + plt->width)
    {
        if (ball->posY >= plt->posY && ball->posY <= plt->posY + plt->height / 2 && !jump)
        {
            ball->velY = -ball->velY;
            return true;
        }
    }

    return false;
}