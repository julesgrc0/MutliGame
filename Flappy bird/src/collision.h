#ifndef _GAME_OVER_
#define _GAME_OVER_

#include<stdio.h>
#include<stdbool.h>
#include "wall.h"
#include "util.h"
#include "const.h"

enum PlayerState
{
    NONE = 0,
    HIT = 1,
    BORDER = 2
};

enum PlayerState CheckCollision(int player_y)
{
    int y = player_y;
    int x = PLAYER_X;
    int h = PLAYER_HEIGHT;
    int w = PLAYER_WIDTH;
    enum PlayerState HaveHit = NONE;
    if((y+h)>SURFACE_HEIGHT)
    {
        HaveHit = BORDER;
    }

    for (int i = 0; i < actualSize; i++)
    {
        if((x+w)>=obstacles[i].x&&x<=obstacles[i].x+WALL_WIDTH_B)
        {
            if((y+h)>SURFACE_HEIGHT)
            {
                HaveHit = BORDER;
            }
            
            if(!(y>(DEFAULT_WALL_HEIGHT-obstacles[i].h1)))
            {   
                HaveHit = HIT;
            }

            if((y+h)>WINDOW_HEIGHT-obstacles[i].h2)
            {
               HaveHit = HIT; 
            }
        }
    }
    return HaveHit;
}

#endif // !_GAME_OVER_