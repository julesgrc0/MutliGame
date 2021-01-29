#pragma once

#ifndef GAME_DRAW_
#define GAME_DRAW_

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>
#include "utils.h"

void SetRGB(SDL_Renderer *render, RGB color, bool opaque)
{
    int alpha = SDL_ALPHA_TRANSPARENT;
    if (opaque)
    {
        alpha = SDL_ALPHA_OPAQUE;
    }
    if (SDL_SetRenderDrawColor(render, color.r, color.g, color.b, alpha))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
}
void SetColor(SDL_Renderer *render, Color color)
{
    if (SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
}

void DrawLine(SDL_Renderer *render, Point p1, Point p2)
{
    if (SDL_RenderDrawLine(render, p1.x, p1.y, p2.x, p2.y))
    {
        printf("[ERROR] Fail to draw line : %s\n", SDL_GetError());
        exit(1);
    }
}

void DrawBlock(SDL_Renderer *render, Size size, Point pos)
{
    SDL_Rect rect = {
        pos.x,
        pos.y,
        size.w,
        size.h};

    if (SDL_RenderDrawRect(render, &rect))
    {
        printf("[ERROR] Fail to draw rect : %s\n", SDL_GetError());
        exit(1);
    }
}

#endif // !GAME_DRAW_