#ifndef _GAME_WALL_
#define _GAME_WALL_

#include<stdlib.h>
#include "util.h"
#include "const.h"

Wall* obstacles;
int actualSize = 0;
int TotalWall = 0;

void AddWall(Wall w)
{
    TotalWall++;
    if(actualSize==0)
    {
        obstacles = malloc(sizeof(Wall));
        obstacles[0] = w;
    }else{
        obstacles = realloc(obstacles,(actualSize+1) * sizeof(Wall));
        obstacles[actualSize] = w;
    }
    actualSize++;
    if(DEBUG)
    {
        Write("Add to memory");
    }
}

void ClearMemory()
{
    free(obstacles);
}

Wall MakeRandom()
{
    int h1 = DEFAULT_WALL_HEIGHT - random(0,DEFAULT_WALL_HEIGHT);
    int h2 = random(100,DEFAULT_WALL_HEIGHT);
    
    int space = 0;
    while (space <= PLAYER_HEIGHT*3)
    {
        h1 = DEFAULT_WALL_HEIGHT - random(0,DEFAULT_WALL_HEIGHT);
        h2 = random(100,DEFAULT_WALL_HEIGHT);
        space = GetSpace(h1,h2);
    }
    if(DEBUG)
    {
        Write("Create wall");
    }

    Wall newWall = {WINDOW_WIDTH,h1,h2};
    AddWall(newWall);
    return newWall;
}

int GetSpace(int h1,int h2)
{
    int fh1 = DEFAULT_WALL_HEIGHT - h1;
    return WINDOW_HEIGHT-(fh1+h2);
}

#endif // !_GAME_WALL_