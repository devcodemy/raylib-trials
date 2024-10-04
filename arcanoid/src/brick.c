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

    if (ball->position.x >= brick->posX && ball->position.x <= brick->posX + brick->size.width)
    {
        if (ball->position.y >= brick->posY && ball->position.y <= brick->posY + brick->size.height)
        { // working well
            brick->active = false;
            ball->velocity.y = -ball->velocity.y;
        }
    }
}