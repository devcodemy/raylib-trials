#ifndef BLOCK_H_
#define BLOCK_H_

#include "raylib.h"

typedef struct Block
{
    int posX;
    int posY;
    int level;
    bool active;
} Block;

Block CreateBlock(int posX, int posY, int level, bool isActive);

void BlockDraw(Block *block);
void BlockUpdate(Block *block, int level, bool isActive);

#endif
