#include "block.h"

Block CreateBlock(int posX, int posY, int level, bool isActive)
{
    return (Block) {
        .posX = posX,
        .posY = posY,
        .level = 1,
        .active = isActive,
    };
}

void BlockDraw(Block *block)
{
    if (block->active)
    {
        DrawRectangle(block->posX, block->posY, 75, 15, GRAY);
    }
}

void BlockUpdate(Block *block, int level, bool isActive)
{
    block->level = level;
    block->active = isActive;
}