#include "common.h"

Brick CreateBrick(int posX, int posY, int level, Size2d brickSize2d, bool isActive)
{
    return (Brick) {
        .posX = posX,
        .posY = posY,
        .level = 1,
        .size = brickSize2d,
        .active = isActive,
    };
}

void BrickDraw(Brick *brick)
{
    if (brick->active)
    {
        DrawRectangle(brick->posX, brick->posY, brick->size.width, brick->size.height, GRAY);
        // DrawRectangle(brick->posX, brick->posY, 75, 15, GRAY);
    }
}

void BrickUpdate(Brick *block, int level, bool isActive)
{
    block->level = level;
    block->active = isActive;
}

void BrickCollision(Brick *brick, Ball *ball)
{
    if (!brick->active) {
        return;
    }

    // if (ball->posX >= brick->posX - ball->radius && ball->posX <= brick->posX + brick->size.width + ball->radius)
    // if (ball->posX + ball->radius >= brick->posX && ball->posX <= brick->posX + brick->size.width + ball->radius)
    if (ball->posX >= brick->posX && ball->posX <= brick->posX + brick->size.width)
    {
        // if (ball->posY + ball->radius >= brick->posY && ball->posY - ball->radius <= brick->posY + brick->size.height)
        if (ball->posY  >= brick->posY && ball->posY <= brick->posY + brick->size.height)
        {
            brick->active = false;
            ball->velY = -ball->velY;
        }
    } // A LOT of condition!! depends of direction // SO might better create special logic for area or something like that!

    // if (ball->posX >= brick->posX - ball->radius && ball->posX <= brick->posX + brick->size.width + ball->radius)
    // if (ball->posY > brick->posY - ball->radius && ball->posY < brick->posY + brick->size.height + ball->radius)
    // {
    //     if (ball->posX + ball->radius > brick->posX && ball->posX - ball->radius < brick->posX + brick->size.width)
    //     {
    //         brick->active = false;
    //         ball->velX = -ball->velX;
    //         // ball->velY = -ball->velY;
    //     }
    // }
}

/*

void BrickCollision(Brick *brick, Ball *ball)
{
    if (!brick->active) {
        return;
    }

    if (ball->posX >= brick->posX - ball->radius && ball->posX <= brick->posX + brick->size.width + ball->radius)
    {
        if (ball->posY + ball->radius >= brick->posY && ball->posY - ball->radius <= brick->posY + brick->size.height)
        {
            brick->active = false;
            ball->velY = -ball->velY;
        }
    }
}
*/
