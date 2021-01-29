#pragma once

#ifndef _INIT_3D_
#define _INIT_3D_

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"

typedef struct Settings3D
{
    int width, height;
    Uint32 flags;
    SDL_Window *window;
    SDL_Renderer *render;
    bool run;
    void (*getFPS)(int value);
    void (*getAction)(SDL_Event event);
} Settings3D;

void init(Settings3D *settings, void (*Draw)(SDL_Renderer *render), void (*Update)(void (*Draw)(SDL_Renderer *render), SDL_Window *window, SDL_Renderer *render))
{
    SDL_version version;
    SDL_VERSION(&version);
    printf("[INFO] Init SDL v%d.%d.%d \n", version.major, version.minor, version.patch);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("[ERROR] Fail to init SDL\n%s \n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[INFO] Success to init SDL\n");
    }
    if (SDL_CreateWindowAndRenderer(settings->width, settings->height, settings->flags, &settings->window, &settings->render))
    {
        printf("[ERROR] Fail to create window and renderer \n%s \n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("[INFO] Success to create window and renderer\n");
    }
    SDL_RenderPresent(settings->render);
    clock_t lastTime = clock();
    clock_t currentTime = clock();
    int framesCount = 0;

    while (settings->run)
    {
        clock_t currentTime = clock();
        framesCount++;
        if (currentTime >= (lastTime + 1 * CLOCKS_PER_SEC))
        {
            settings->getFPS(framesCount);
            framesCount = 0;
            lastTime = currentTime;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            settings->getAction(event);
            switch (event.type)
            {
            case SDL_QUIT:
                settings->run = false;
                break;
            }
        }

        (*Update)(Draw, settings->window, settings->render);
    }

    if (SDL_RenderClear(settings->render))
    {
        printf("[ERROR] Fail to clear render\n%s\n", SDL_GetError());
    }
    else
    {
        printf("[INFO] Success to clear renderer\n");
    }

    SDL_DestroyRenderer(settings->render);
    printf("[INFO] Renderer destroy\n");
    SDL_DestroyWindow(settings->window);
    printf("[INFO] Window destroy\n");
    SDL_Quit();
    printf("[INFO] Quit SDL\n");
}

Settings3D SetupSettings(Size window_size, Uint32 window_flags, Default win_rnd, bool run, void (*getFPS)(int value), void (*getAction)(SDL_Event event))
{
    return (Settings3D){window_size.w, window_size.h, window_flags, win_rnd.window, win_rnd.render, run, getFPS, getAction};
}

#endif // !_INIT_3D_