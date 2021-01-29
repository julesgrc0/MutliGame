#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>

#include "src/init.h"
#include "src/utils.h"
#include "src/const.h"
#include "src/draw.h"
#include "src/entity.h"
#include "src/curor.h"
#include "src/camera.h"

static Settings3D settings;
static SDL_Window *window;
static SDL_Renderer *render;
static bool Run = true;
static Entity *ListItem;
static Point cursorPos = {0, 0};
static Point CAMERA_POS = {0, 0};
static SDL_Cursor *cursor;

void GetFPS(int fps);
void OnEvent();
void Draw(SDL_Renderer *render);
void Update(void (*Draw)(SDL_Renderer *render), SDL_Window *window, SDL_Renderer *render);
void KeyboardAction(SDL_Keycode key);
void InitEntitys();
void AddEntity(Point pos, Size size);

int wallItemId[100];

int main(int argc, char *argv[])
{
    /*
        GAME WINDOW Setup
    */
    Size size = {WINDOW_WIDTH, WINDOW_HEIGHT};
    Default def = {window, render};
    settings = SetupSettings(size, WINDOW_NO_FLAGS, def, Run, &GetFPS, &OnEvent);

    InitEntitys();
    InitCursor(cursor, DEFAULT_CURSOR_SIZE, DEFAULT_CUROSR_COLOR);
    init(&settings, Draw, Update);
    free(ListItem);
    return 0;
}

void Draw(SDL_Renderer *render)
{
    RGB color = {112, 168, 237};
    SetRGB(render, color, true);
    DrawBlock(render, GetEntity(ListItem, wallItemId[0]).size, InView(CAMERA_POS, GetEntity(ListItem, wallItemId[0]).pos));


    // Base block
    DrawBlock(render,(Size){WINDOW_WIDTH, WINDOW_HEIGHT},CAMERA_POS);

    CursorTracer(render, cursorPos);
    MoveCursor(render, cursorPos);
}

void Update(void (*Draw)(SDL_Renderer *render), SDL_Window *window, SDL_Renderer *render)
{
    RGB black = {0, 0, 0};
    SetRGB(render, black, true);
    SDL_RenderClear(render);
    (*Draw)(render);
    SDL_RenderPresent(render);
}

void GetFPS(int fps)
{
    char buf[300];
    snprintf(buf, sizeof(buf), "3D Game - FPS: %d  (%d;%d)", fps, CAMERA_POS.x, CAMERA_POS.y);
    SDL_SetWindowTitle(settings.window, buf);
}

void OnEvent(SDL_Event e)
{
    switch (e.type)
    {
    case SDL_MOUSEMOTION:
        cursorPos.x = e.button.x;
        cursorPos.y = e.button.y;
        break;
    case SDL_KEYDOWN:
        KeyboardAction(e.key.keysym.sym);
        break;
    case SDL_MOUSEWHEEL:
        if (e.button.x > 0)
        {
            MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x, CAMERA_POS.y - DEFAULT_CAMERA_SPEED});
            MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x - DEFAULT_CAMERA_SPEED, CAMERA_POS.y});
        }
        else
        {
            MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x, CAMERA_POS.y + DEFAULT_CAMERA_SPEED});
            MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x + DEFAULT_CAMERA_SPEED, CAMERA_POS.y});
        }
        break;
    }
}

void KeyboardAction(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_UP:
        MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x, CAMERA_POS.y - DEFAULT_CAMERA_SPEED});
        break;
    case SDLK_DOWN:
        MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x, CAMERA_POS.y + DEFAULT_CAMERA_SPEED});
        break;
    case SDLK_LEFT:
        MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x - DEFAULT_CAMERA_SPEED, CAMERA_POS.y});
        break;
    case SDLK_RIGHT:
        MoveCamera(&CAMERA_POS, (Point){CAMERA_POS.x + DEFAULT_CAMERA_SPEED, CAMERA_POS.y});
        break;
    }
}

void InitEntitys()
{
    Entity wallItem = {(Point){8, 198}, (Size){10, 10}, 0, false};
    struct UpdateList update = RegisterEntity(ListItem, wallItem);
    ListItem = update.list;
    wallItemId[0] = update.id;
}

/*
//printf("(%d:%d)\n", e.button.x, e.button.y);




SDL_Rect rect =
        {
            0,
            0,
            100,
            100};
    if (SDL_RenderDrawRect(render, &rect))
    {
        SDL_Log("[ERROR] Fail to create rectangle\n%s\n", SDL_GetError());
        exit(1);
    }
*/