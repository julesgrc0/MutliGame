#pragma once

#ifndef _DRAW_
#define _DRAW_

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "player.h"
#include "fireball.h"

typedef struct Size
{
    int w, h;
} Size;

void DrawPlayer(SDL_Renderer *render, Player player)
{
    if (SDL_SetRenderDrawColor(render, 112, 168, 237, SDL_ALPHA_OPAQUE))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
    SDL_Rect rect =
        {
            player.x,
            player.y,
            player.w,
            player.h};
    if (SDL_RenderDrawRect(render, &rect))
    {
        SDL_Log("[ERROR] Fail to create rectangle\n%s\n", SDL_GetError());
        exit(1);
    }
}


void DrawFireBallImage(SDL_Renderer *render, Fireball ball, const char *path)
{
    SDL_Surface *image = SDL_LoadBMP(path);
    SDL_Texture *texture;
    SDL_Rect rect = {ball.x, ball.y, ball.w, ball.h};

    if (!image)
    {
        SDL_Log("[ERROR] Fail to load image\n %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTextureFromSurface(render, image);
    SDL_FreeSurface(image);

    if (!texture)
    {
        SDL_Log("[ERROR] Fail to create texture\n %s", SDL_GetError());
        exit(1);
    }

    if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h))
    {
        SDL_Log("[ERROR] Fail to query texture\n %s", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderCopy(render, texture, NULL, &rect))
    {
        SDL_Log("[ERROR] Fail to create texture\n %s", SDL_GetError());
        exit(1);
    }
}

void DrawFireBall(SDL_Renderer *render, Fireball ball)
{
    if(ball.direction == D_LEFT)
    {
        DrawFireBallImage(render,ball,FIREBALL_L_IMAGE);
    }
    else
    {
        DrawFireBallImage(render,ball,FIREBALL_R_IMAGE);
    }
}

void DrawPlayerImage(SDL_Renderer *render, Player player, const char *path)
{
    SDL_Surface *image = SDL_LoadBMP(path);
    SDL_Texture *texture;
    SDL_Rect rect = {player.x, player.y, player.w, player.h};

    if (!image)
    {
        SDL_Log("[ERROR] Fail to load image\n %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTextureFromSurface(render, image);
    SDL_FreeSurface(image);

    if (!texture)
    {
        SDL_Log("[ERROR] Fail to create texture\n %s", SDL_GetError());
        exit(1);
    }

    if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h))
    {
        SDL_Log("[ERROR] Fail to query texture\n %s", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderCopy(render, texture, NULL, &rect))
    {
        SDL_Log("[ERROR] Fail to create texture\n %s", SDL_GetError());
        exit(1);
    }
}

void ClearCase(SDL_Renderer *render, int x, int y)
{
    if (SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
    for(int xP=x;xP<=(x+20);xP++)
    {
        for(int yP=y;yP<=(y+20);yP++)
        {
            SDL_RenderDrawPoint(render,xP,yP);
        }
    }
}

void DrawGrid(SDL_Renderer *render, Size size)
{
    if (SDL_SetRenderDrawColor(render, 117, 117, 117, SDL_ALPHA_OPAQUE))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
    for (int x = 0; x <= size.w; x += 20)
    {
        SDL_RenderDrawLine(render, x, 0, x, size.h);
    }

    for (int y = 0; y <= size.h; y += 20)
    {
        SDL_RenderDrawLine(render, 0, y, size.w, y);
    }
}

#endif // !_DRAW_