#pragma once

#ifndef _UTILS_
#define _UTILS_

#include <SDL.h>
#include<stdbool.h>

typedef struct Point
{
    int x, y;
} Point;

typedef struct Size
{
    int w, h;
} Size;

typedef struct Default
{
    SDL_Window *window;
    SDL_Renderer *render;
} Default;

typedef struct Color
{
    int r, g, b, a;
} Color;

typedef struct RGB
{
    int r, g, b;
} RGB;

bool isInSurface(Size sizeItem1,Point posItem1,Point item)
{
    int px = sizeItem1.w+posItem1.x; 
    int py = sizeItem1.h+posItem1.y; 
    bool inSurface = false;

    if(item.x>posItem1.x&&item.x<px)
    {       
        inSurface = true;
    } 
    if(item.y>posItem1.y&&item.y<py)
    {
        inSurface = true;
    }

    return inSurface;
}

#endif // !_UTILS_