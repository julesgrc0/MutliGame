#ifndef _GAME_
#define _GAME_
#include <SDL2/SDL.h>
#include<iostream>
#include<math.h>
#include<string>

void SetColor(SDL_Renderer *render, int r, int g, int b)
{
    SDL_SetRenderDrawColor(render, r, g, b, SDL_ALPHA_OPAQUE);
}

void SDL_RenderDrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);

        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);

        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);

        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

typedef struct Int16
{
    int a, b;
} Int16;

typedef struct Item
{
    int x, y;
    int size;
} Item;

double distance(Int16 pos1,Int16 pos2)
{
    double x = (pos1.a - pos2.a) * (pos1.a - pos2.a);
    double y = (pos1.b - pos2.b) * (pos1.b - pos2.b);
    return sqrt(x+y);
}

Int16 isIn(Int16 pos1, Int16 size1, Int16 pos2, Int16 size2)
{
    Int16 n = {};
    if ((pos2.a >= pos1.a && pos2.a <= (pos1.a + size1.a)) && (pos2.b >= pos1.b && pos2.b <= (pos1.b + size1.b)))
    {
        n = {0,0};
    }else{
        n = {pos2.a - pos1.a,pos2.b - pos1.a};
    }
    return n;
}

int random(int min, int max)
{
    return (min + (rand() % (max - min + 1)));
}

typedef enum Movement{
    TOP,
    LEFT,
    RIGHT,
    BOTTOM,
    NONE
}Movement;


#endif // !_GAME_