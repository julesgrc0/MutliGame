#pragma once

#ifndef GAME_CURSOR
#define GAME_CURSOR

#include<SDL.h>
#include "utils.h"
#include "draw.h"

static int CURSOR_SIZE = 8;
static RGB CURSOR_COLOR = {255,255,255};

void InitCursor(SDL_Cursor *cursor,int size,RGB rgb)
{
    CURSOR_COLOR = rgb;
    CURSOR_SIZE  = size;
    int32_t cursorData[2] = {0, 0};
    cursor = SDL_CreateCursor((Uint8 *)cursorData, (Uint8 *)cursorData, 8, 8, 4, 4);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(SDL_DISABLE);
}

void MoveCursor(SDL_Renderer *render,Point pos)
{
    SetRGB(render,CURSOR_COLOR,true);
    
    int size = CURSOR_SIZE;

    Point tb = {pos.x,pos.y-size};
    Point tb_end = {pos.x,pos.y+size};

    Point rl = {pos.x-size,pos.y};
    Point rl_end = {pos.x+size,pos.y};

    DrawLine(render,tb,tb_end);
    DrawLine(render,rl,rl_end);
}

void CursorTracer(SDL_Renderer *render,Point pos_cursor){
    SetRGB(render,(RGB){255, 42, 0},true);
    DrawLine(render,(Point){0,pos_cursor.y},(Point){pos_cursor.x,pos_cursor.y});

    SetRGB(render,(RGB){0, 255, 34},true);
    DrawLine(render,(Point){pos_cursor.x,0},(Point){pos_cursor.x,pos_cursor.y});
}



#endif // !GAME_CURSOR