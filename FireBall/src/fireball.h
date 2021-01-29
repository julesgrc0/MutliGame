#pragma once

#ifndef _FIREBALL_
#define _FIREBALL_

#include <SDL.h>
#include "const.h"

typedef enum FireBallDirection
{
    D_LEFT,
    D_RIGHT
} FireBallDirection;

typedef struct Fireball
{
    int x, y;
    int w, h;
    int increment;
    FireBallDirection direction;
    bool isRemove;
    SDL_Surface image;
} Fireball;



typedef struct Coord
{
    int x, y;
} Coord;

typedef struct BallLimit
{
    int min;
    int max;
} BallLimit;

void MoveFireBall(Fireball *ball, BallLimit limit)
{
    if (ball->direction == D_LEFT)
    {
        if ((ball->x - ball->increment) >= limit.min)
        {
            ball->x -= ball->increment;
        }
        else
        {
            ball->isRemove = true;
        }
    }
    else
    {
        if ((ball->x + ball->increment) <= limit.max)
        {
            ball->x += ball->increment;
        }
        else
        {
            ball->isRemove = true;
        }
    }
}

Fireball CreateFireBall(Coord pos, int size, int increment, FireBallDirection direction)
{
    Fireball fireball = {pos.x, pos.y, size, size, increment, direction, false};
    return fireball;
}

#endif // !_FIREBALL_