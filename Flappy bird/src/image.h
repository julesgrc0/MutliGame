#ifndef _GAME_TEXTURE_
#define _GAME_TEXTURE_

#include <SDL.h>
#include <stdio.h>
#include "util.h"
#include "const.h"

void DrawTexture(SDL_Renderer *render, const char *path, Point pos, Size size);

void DrawTexture(SDL_Renderer *render, const char *path, Point pos, Size size)
{
    SDL_Surface *image = SDL_LoadBMP(path);
    SDL_Texture *texture;
    SDL_Rect rect = {pos.x, pos.y, size.w, size.h};

    if (!image)
    {
        printf("\n[ERROR] Fail to load image: %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTextureFromSurface(render, image);

    if (!texture)
    {
        printf("\n[ERROR] Fail to create texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(image);

    if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h))
    {
        printf("\n[ERROR] Fail to query texture: %s", SDL_GetError());
        exit(1);
    }
    if (SDL_RenderCopy(render, texture, NULL, &rect))
    {
        printf("\n[ERROR] Fail to create render texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_DestroyTexture(texture);
}

#endif // !_GAME_TEXTURE_