#ifndef COMMON_H_
#define COMMON_H_

#include "raylib.h"

#define SCREEN_WIDTH 600
#define SCREEN_DEPTH 400

#define SOME_DELAY 0.025f
#define MAGIC_EIGHT 8
#define BRICK_MAX 30
#define PADDING 5

#define NEARBLACK \
    CLITERAL(Color) { 15, 15, 15, 255 }

typedef struct Size2d {
    int width;
    int height;
} Size2d;

typedef struct Brick
{
    int posX;
    int posY;
    int level;
    Size2d size;
    bool active;
} Brick;

typedef struct Ball
{
    Vector2 position;
    Vector2 velocity;
    int radius;
} Ball;

typedef struct Platform
{
    int posX;
    int posY;
    int width;
    int height;
    int speed;
} Platform;

void MakeWall();

Brick CreateBrick(int posX, int posY, int level, Size2d brickSize2d, bool isActive);
void BrickDraw(Brick *block);
void BrickUpdate(Brick *block, int level, bool isActive);

bool UpdatePlatform(Platform *plt, bool back, bool followBall, Ball *ball);
void UpdateBall(Platform *plt, Ball *ball);
bool UpdateDrawFrame(Platform *plt, Ball *ball);

void BrickCollision(Brick *brick, Ball *ball);
bool PlatformCollision(Platform *plt, Ball *ball, bool jump);

#endif