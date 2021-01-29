#pragma once

#ifndef _PLAYER_
#define _PLAYER_

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include<SDL_mixer.h>
#include "const.h"

Mix_Music *border;

typedef struct PlayerBorder
{
    int x, y;
    int w, h;
} PlayerBorder;

typedef struct Player
{
    int x, y;
    int w, h;
    char *name;
    int increment;
    SDL_Surface image;
    PlayerBorder border;
} Player;

typedef enum PlayerState{
    LEFT,
    RIGHT,
    CENTER,
    ATTACK_L,
    ATTACK_R,
    DEATH,
    DESTROY
}PlayerState;

typedef struct PlayerSize
{
    int w, h;
} PlayerSize;


void SetPlayerBorder(Player *pl, PlayerBorder border)
{
    pl->border = border;
}

void MovePlayerX(Player *pl)
{
    if (&pl->border)
    {
        if ((pl->x + pl->increment) <= (pl->border.w - pl->w))
        {
            pl->x += pl->increment;
        }else{
            Mix_PlayChannel(-1, border, 0);
        }
    }
    else
    {
        pl->x += pl->increment;
    }
}
void BackPlayerX(Player *pl)
{
    if (&pl->border)
    {
        if ((pl->x - pl->increment) >= pl->border.x)
        {
            pl->x -= pl->increment;
        }else{
            Mix_PlayChannel(-1, border, 0);
        }
    }
    else
    {
        pl->x -= pl->increment;
    }
}

void MovePlayerY(Player *pl)
{
    if (&pl->border)
    {
        if ((pl->y + pl->increment) <= (pl->border.h - pl->h))
        {
            pl->y += pl->increment;
        }else
        {
            Mix_PlayChannel(-1, border, 0);
        }
        
    }
    else
    {
        pl->y += pl->increment;
    }
}
void BackPlayerY(Player *pl)
{
    if (&pl->border)
    {
        int y = pl->y;
        y - pl->increment;
        if ((pl->y - pl->increment) >= pl->border.y)
        {
            pl->y -= pl->increment;
        }else{
            Mix_PlayChannel(-1, border, 0);
        }
    }
    else
    {
        pl->y -= pl->increment;
    }
}

void SetPlayerPos(Player *pl, int x, int y)
{
    pl->x = x;
    pl->y = y;
}

Player CreatePlayer(char *name, PlayerSize size, int increment)
{
    return (Player){0, 0, size.w, size.h, name, increment};
}

#endif // !_PLAYER_