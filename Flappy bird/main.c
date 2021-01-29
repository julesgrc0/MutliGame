#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <time.h>
#include <string.h>


#include "src/const.h"
#include "src/audio.h"
#include "src/draw.h"
#include "src/wall.h"
#include "src/collision.h"

void KeyAction(SDL_Event event, bool *Run);
void Update();
void Draw();
void ShowFPS(int fps);
void ContextUpdate();

bool Run = true;
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
double GameSpeed = 0.04;
int PipeInterval = 100;
int GSpeed = 2;
Point PlayerPos = {PLAYER_X,(WINDOW_HEIGHT-PLAYER_HEIGHT)/2};

int main(int argc, char **argv)
{
    Init(argc,argv);
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("[ERROR] Fail to initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }else if(DEBUG)
    {
        Write("Success to init SDL");
    }

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        printf("[ERROR] Fail to initilize Audio\n %s", Mix_GetError());
        return 1;
    }else if(DEBUG)
    {
        Write("Success to init Audio");
    }

    LoadSongs();
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        printf("[ERROR] Fail to create window \n%s\n", SDL_GetError());
        return 1;
    }else if(DEBUG)
    {
        Write("Success to create window");
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!render)
    {
        printf("[ERROR] Fail to present render\n%s\n", SDL_GetError());
        return 1;
    }else if(DEBUG)
    {
        Write("Success to create renderer");
    }
    SDL_RenderPresent(render);
    PlayMusic();

    // ____ <- 0 max DEFAULT_WALL_HEIGHT min
    //
    // SURFACE_HEIGHT
    //
    // ____ <- DEFAULT_WALL_HEIGHT max  100 min

    clock_t lastTime = clock();
    clock_t currentTime = clock();
    clock_t L_time = clock();
    int framesCount = 0; 
    int PipeMsCount = 0;
    
    while (Run)
    {
        
        currentTime = clock();
        framesCount++;
        if (currentTime >= (lastTime + GameSpeed * CLOCKS_PER_SEC))
        {
            PipeMsCount++;
            if(TotalWall>10)
            {
                PipeInterval = 95;
                Write("Change wall interval to 95");
            }
            if(TotalWall>20)
            {
                PipeInterval = 90;
                Write("Change wall interval to 90");
            }
            if(TotalWall>30)
            {
                GSpeed = 3;
                Write("Change speed to 3");
            }
            if(TotalWall>40)
            {
                GSpeed = 4;
                Write("Change speed to 4");
            }
            if(TotalWall>50)
            {
                GSpeed = 5;
                PipeInterval = 85;
                Write("Change speed to 3");
                Write("Change wall interval to 85");
            }
            
            if(PipeMsCount>=PipeInterval)
            {
                PipeMsCount = 0;
                MakeRandom();
            }

            if(DEBUG)
            {
                ShowFPS(framesCount);
            }else{
                ShowScore();
            }

            framesCount = 0;
            lastTime = currentTime;
            ContextUpdate();
            Update();
        }


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Run = false;
                break;
            case SDL_KEYDOWN:
                KeyAction(event,&Run);
                break;
            }
        }
    }

    if (SDL_RenderClear(render))
    {
        printf("[ERROR] Fail to clear render\n%s\n", SDL_GetError());
        return 1;
    }
    free(obstacles);
    FreeSongs();
    Mix_CloseAudio();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void KeyAction(SDL_Event event, bool *Run)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        Run = false;
        break;
    case SDLK_DOWN:
        PlayerPos.y += 4;
        break;
    case SDLK_UP:
    case SDLK_SPACE:
        PlayerPos.y -= 4;
        break;
    }
}

void ContextUpdate()
{
    PlayerPos.y += 2;
    switch (CheckCollision(PlayerPos.y+2))
    {
    case HIT:
        Run = 0;
        break;
    case BORDER:
        PlayerPos.y -= 100;
        break;
    }
    if(DEBUG&&!Run)
    {
        char bu[300];
        snprintf(bu, 300, "GameOver score:  %d", TotalWall);
        Write(bu);
    }

    int countRemove = 0;
    for(int i=0;i<actualSize;i++)
    {
        obstacles[i].x-=GSpeed;
        if(obstacles[i].x < -50)
        {
            countRemove++;
        }
    }

    if(countRemove>6)
    {
        if(DEBUG)
        {
            Write("Clear wall memory");
        }
        Wall berforelast = obstacles[actualSize-1];
        Wall last = obstacles[actualSize];
        free(obstacles);
        actualSize=0;
        AddWall(berforelast);
        AddWall(last);
    }
}



void Draw()
{
    DrawBackground(render);
    DrawFloor(render);
    for(int i=0;i<actualSize;i++)
    {
        DrawWall(render,(Wall){obstacles[i].x,obstacles[i].h1,obstacles[i].h2});
    }
    DrawPlayer(render,(Point){PlayerPos.x,PlayerPos.y});
}

void Update()
{
    if (SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE))
    {
        printf("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(render);
    Draw();
    SDL_RenderPresent(render);
}

void ShowFPS(int fps)
{
    char buf[30];
    snprintf(buf, 30,"%s - FPS: %d", WINDOW_TITLE,fps);
    SDL_SetWindowTitle(window, buf);
}

void ShowScore()
{
    char buf[30];
    snprintf(buf, 30,"%s - Score: %d", WINDOW_TITLE,TotalWall);
    SDL_SetWindowTitle(window, buf);
}

