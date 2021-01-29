#ifndef _GAME_DRAW_
#define _GAME_DRAW_

#include <SDL.h>
#include "util.h"
#include "const.h"
#include "image.h"


void DrawPlayer(SDL_Renderer *render, Point pos);
void SetColor(SDL_Renderer *render, int r, int g, int b);
void DrawWall(SDL_Renderer *render, Wall wall);
void DrawFloor(SDL_Renderer *render);
void DrawBackground(SDL_Renderer *render);

void DrawPlayer(SDL_Renderer *render, Point pos)
{
    DrawTexture(render, IMG_PLAYER, pos, (Size){PLAYER_WIDTH,PLAYER_HEIGHT});

    if(DEBUG)
    {
        SetColor(render,227, 9, 56);
        SDL_Rect rect = {pos.x,pos.y,PLAYER_WIDTH,PLAYER_HEIGHT};
        SDL_RenderDrawRect(render,&rect);
        SetColor(render,3, 3, 252);
        SDL_RenderDrawLine(render,PLAYER_X,pos.y,0,pos.y);
        SDL_RenderDrawLine(render,PLAYER_X,0,PLAYER_X,pos.y);
    }   
}

void DrawWall(SDL_Renderer *render, Wall wall)
{
    Point pos1 = {wall.x,WINDOW_HEIGHT-wall.h2};
    Size size1 = {WALL_WIDTH_B,wall.h2};
    DrawTexture(render,IMG_WALL_B,pos1,size1);
    
    Point pos2 = {wall.x,0 - wall.h1};
    Size size2 = {WALL_WIDTH_T,wall.h1};
    DrawTexture(render,IMG_WALL_T,pos2,size2);

    if(DEBUG)
    {
        SetColor(render,227, 9, 56);
        SDL_Rect rect1 = {pos1.x,pos1.y,size1.w,size1.h};
        SDL_Rect rect2 = {pos2.x,pos2.y,size2.w,size2.h};
        SDL_RenderDrawRect(render,&rect1);
        SDL_RenderDrawRect(render,&rect2);

        SetColor(render,255, 0, 0);
        for(int x=0;x<pos1.x;x+=5)
        {
            SDL_RenderDrawPoint(render,x,pos1.y);
            SDL_RenderDrawPoint(render,x,pos2.y+DEFAULT_WALL_HEIGHT);
        }
    }  
}

void DrawFloor(SDL_Renderer *render)
{
    DrawTexture(render, IMG_FLOOR, (Point){0, WINDOW_HEIGHT-FLOOR_HEIGHT}, (Size){WINDOW_WIDTH, WINDOW_HEIGHT});

    if(DEBUG)
    {
        SetColor(render,3, 3, 252);
        SDL_RenderDrawLine(render,0,SURFACE_HEIGHT,WINDOW_WIDTH,SURFACE_HEIGHT);
    }
}

void DrawBackground(SDL_Renderer *render)
{
    DrawTexture(render, IMG_BACKGROUND, (Point){0, 0}, (Size){WINDOW_WIDTH, WINDOW_HEIGHT});
}

void SetColor(SDL_Renderer *render, int r, int g, int b)
{
    SDL_SetRenderDrawColor(render, r, g, b, SDL_ALPHA_OPAQUE);
}

#endif // !_GAME_DRAW_