#pragma once

#ifndef GAME_ENTITY
#define GAME_ENTITY

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "draw.h"
#include "utils.h"

static int actualIndex = 0;

typedef struct Entity
{
    Point pos;
    Size size;
    int id;
    bool NoColision;
    bool isRemove;
} Entity;

struct UpdateList{
    int id;
    Entity* list;
};

struct UpdateList RegisterEntity(Entity *list, Entity item)
{
    actualIndex++;
    list = (Entity *)calloc(actualIndex, sizeof(Entity));
    item.id = actualIndex;
    *(list + (actualIndex - 1)) = item;
    struct UpdateList update = {actualIndex,list};
    return update;
}

void MoveEntity(Entity *list, int index,Point pos)
{
    Entity item = list[index-1];
    item.pos.x = pos.x;
    item.pos.y = pos.y;

    for(int i=0;i<actualIndex;i++)
    {
        if(!list[i-1].isRemove)
        {
            printf("%d\n",isInSurface(list[i-1].size,list[i-1].pos,pos));
        }
    }


    *(list + (index - 1)) = item;
}



Entity GetEntity(Entity *list,int index){
    return  list[index-1];
}

#endif // !GAME_ENTITY