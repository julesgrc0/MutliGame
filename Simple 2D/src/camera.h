#pragma once

#ifndef GAME_CAMERA
#define GAME_CAMERA

#include <SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "utils.h"
#include "const.h"

void MoveCamera(Point *camera, Point pos)
{
    *camera = pos;
}

Point InView(Point camera, Point posItem)
{
    return (Point){camera.x + posItem.x, camera.y + posItem.y};
}

void DrawCamera(SDL_Renderer *render, Point CAMERA_POS)
{
    SetRGB(render, (RGB){13, 0, 255}, true);
    DrawBlock(render, (Size){WINDOW_WIDTH, WINDOW_HEIGHT}, CAMERA_POS);
}

bool isInView(Point camera, Size cam_size, Point posItem,Size item_size)
{
    return isInSurface(cam_size,camera,posItem);
}

#endif // !GAME_CAMERA