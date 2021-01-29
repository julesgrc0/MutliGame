#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <time.h>
#include <string.h>

#include "src/player.h"
#include "src/const.h"
#include "src/draw.h"
#include "src/fireball.h"

Player player1;
PlayerState player1State;
Fireball currenFB;
Fireball destory;
bool isCurrentRunF = false;
bool isDestroy = true;
bool isInitD = false;
bool isEnd = false;

Mix_Music *background;
Mix_Music *shoot;
Mix_Music *move;

typedef struct Point
{
    int x, y;
    bool isSet;
} Point;

Point listPoint[400];

void Draw(SDL_Renderer *render);
void Update(SDL_Renderer *render, SDL_Window *window, void (*callback)(SDL_Renderer *rnd));
void ChangePlayerState();
void DrawFireBallDestory(SDL_Renderer *render);
void ThrowFireBall();
void KeyAction(SDL_Event event, bool *EnableFPS, bool *Run);
void FreeSongs();
void LoadSongs();

int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_version nb;
    bool Run = true;

    SDL_VERSION(&nb);
    printf("SDL version %d.%d.%d\n", nb.major, nb.minor, nb.patch);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("[ERROR] Fail to initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] Fail to initilize Audio\n %s", Mix_GetError());
        return 1;
    }
    LoadSongs();

    window = SDL_CreateWindow("FireBall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        SDL_Log("[ERROR] Fail to create window \n%s\n", SDL_GetError());
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!render)
    {
        SDL_Log("[ERROR] Fail to present render\n%s\n", SDL_GetError());
        return 1;
    }
    SDL_RenderPresent(render);

    player1 = CreatePlayer("Jules", (PlayerSize){PLAYER_DEFAULT_WIDTH, PLAYER_DEFAULT_HEIGHT}, PLAYER_DEFAULT_SPEED);
    SetPlayerBorder(&player1, (PlayerBorder){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT});
    BallLimit limit = {0, WINDOW_WIDTH - FIREBALL_DEFAULT_WIDTH};
    clock_t lastTime = clock();
    clock_t lastT = clock();
    int framesCount = 0;
    bool EnableFPS = true;
    bool PlayerUp = false;

    Mix_PlayMusic(background, -1);

    while (Run)
    {

        clock_t currentTime = clock();
        if (EnableFPS)
        {
            framesCount++;
            if (currentTime >= (lastTime + 1 * CLOCKS_PER_SEC))
            {
                char buf[30];
                snprintf(buf, 30, "FireBall - FPS: %d", framesCount);
                SDL_SetWindowTitle(window, buf);
                printf("\rFPS: %d", framesCount);
                framesCount = 0;
                lastTime = currentTime;
            }
        }
        if (currentTime >= (lastT + 0.05 * CLOCKS_PER_SEC))
        {
            if (isEnd)
            {
                isEnd = false;
            }
            isDestroy = true;
            if (!currenFB.isRemove)
            {
                MoveFireBall(&currenFB, limit);
            }
            else
            {
                isEnd = true;
                destory = currenFB;
                isDestroy = false;
            }

            lastT = currentTime;
        }

        PlayerUp = !PlayerUp;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.button.type == _WINDOW_CLICK || event.button.type == WINDOW_CLICK)
            {
            }
            switch (event.type)
            {
            case SDL_QUIT:
                Run = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    ThrowFireBall();
                    /*int index = 0;
                    while (listPoint[index].isSet)
                    {
                        printf("\nindex: %d (%d;%d) isSet:%d\n", index, listPoint[index].x, listPoint[index].y, listPoint[index].isSet);
                        index++;
                        if (index >= 400)
                        {
                            break;
                        }
                    }
                    if (index <= 400)
                    {
                        listPoint[index].x = event.button.x;
                        listPoint[index].y = event.button.y;
                        listPoint[index].isSet = true;
                    }*/
                }
                else if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (player1State == ATTACK_L || player1State == ATTACK_R)
                    {
                        if (player1State == ATTACK_L)
                        {
                            player1State = ATTACK_L;
                        }
                        else
                        {
                            player1State = ATTACK_R;
                        }
                    }
                    else
                    {
                        Play(move);
                        int x = event.button.x / 20;
                        int y = event.button.y / 20;
                        SetPlayerPos(&player1, x * 20, y * 20);
                        player1State = CENTER;
                    }
                }
                break;
            case SDL_KEYDOWN:
                KeyAction(event, &EnableFPS, &Run);
                break;
            }
        }

        void (*draw)(SDL_Renderer * render) = &Draw;
        Update(render, window, draw);
    }

    if (SDL_RenderClear(render))
    {
        SDL_Log("[ERROR] Fail to clear render\n%s\n", SDL_GetError());
        return 1;
    }
    FreeSongs();
    Mix_CloseAudio();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void ThrowFireBall()
{
    if (player1State == ATTACK_L || player1State == ATTACK_R)
    {
        isCurrentRunF = true;
        Play(shoot);
        if (!isInitD)
        {
            isInitD = true;
        }
        else
        {
            isDestroy = false;
        }

        FireBallDirection direction = D_RIGHT;
        int add = player1.w;
        if (player1State == ATTACK_L)
        {
            direction = D_LEFT;
            add = player1.w - player1.w * 2;
        }
        destory = currenFB;
        destory.isRemove = false;
        currenFB = CreateFireBall((Coord){player1.x + add, player1.y}, player1.w, player1.w, direction);
    }
}

void KeyAction(SDL_Event event, bool *EnableFPS, bool *Run)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_p:
        *EnableFPS = !EnableFPS;
        break;
    case SDLK_ESCAPE:
        Run = false;
        break;
    case SDLK_SPACE:
        ThrowFireBall();
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        ChangePlayerState();
        break;
    case SDLK_UP:
        if (player1State == ATTACK_L || player1State == ATTACK_R)
        {
            if (player1State == ATTACK_L)
            {
                player1State = ATTACK_L;
            }
            else
            {
                player1State = ATTACK_R;
            }
        }
        else
        {
            player1State = CENTER;
        }
        BackPlayerY(&player1);
        break;
    case SDLK_DOWN:
        if (player1State == ATTACK_L || player1State == ATTACK_R)
        {

            if (player1State == ATTACK_L)
            {
                player1State = ATTACK_L;
            }
            else
            {
                player1State = ATTACK_R;
            }
        }
        else
        {
            player1State = CENTER;
        }
        MovePlayerY(&player1);
        break;
    case SDLK_LEFT:
        if (player1State == ATTACK_L || player1State == ATTACK_R)
        {
            player1State = ATTACK_L;
        }
        else
        {
            player1State = LEFT;
        }
        BackPlayerX(&player1);
        break;
    case SDLK_RIGHT:
        if (player1State == ATTACK_L || player1State == ATTACK_R)
        {
            player1State = ATTACK_R;
        }
        else
        {
            player1State = RIGHT;
        }
        MovePlayerX(&player1);
        break;
    }
}

void Play(Mix_Music *song)
{
    Mix_PlayChannel(-1, song, 0);
}

void LoadSongs()
{
    background = Mix_LoadMUS(SONG_BACKGROUND);
    shoot = Mix_LoadWAV(SONG_SHOOT);
    move = Mix_LoadWAV(SONG_MOVE);
    border = Mix_LoadWAV(SONG_BORDER);
}

void FreeSongs()
{
    Mix_FreeChunk(background);
    Mix_FreeChunk(shoot);
    Mix_FreeChunk(move);
    Mix_FreeChunk(border);
    background = NULL;
    shoot = NULL;
    move = NULL;
    border = NULL;
}

void ChangePlayerState()
{
    switch (player1State)
    {
    case LEFT:
        player1State = ATTACK_L;
        break;
    case RIGHT:
        player1State = ATTACK_R;
        break;
    case ATTACK_L:
        player1State = LEFT;
        break;
    case ATTACK_R:
        player1State = RIGHT;
        break;
    case CENTER:
        player1State = ATTACK_L;
        break;
    case DEATH:
        player1State = CENTER;
        player1.x = 0;
        player1.y = 0;
        break;
    }
}

void DrawFireBallDestory(SDL_Renderer *render)
{
    DrawFireBallImage(render, destory, FIREBALL_DESTORY);
}

void Draw(SDL_Renderer *render)
{
    DrawGrid(render, (Size){WINDOW_WIDTH, WINDOW_HEIGHT});
    DrawPlayer(render, player1);
    if (!isDestroy && isInitD)
    {
        if (!destory.isRemove && !isEnd)
        {
            DrawFireBallDestory(render);
        }
    }
    if (isCurrentRunF)
    {
        if (!currenFB.isRemove)
        {
            DrawFireBall(render, currenFB);
        }
    }
    switch (player1State)
    {
    case CENTER:
        DrawPlayerImage(render, player1, PLAYER_DEFAULT_IMAGE);
        break;
    case LEFT:
        DrawPlayerImage(render, player1, PLAYER_L_IMAGE);
        break;
    case RIGHT:
        DrawPlayerImage(render, player1, PLAYER_R_IMAGE);
        break;
    case ATTACK_L:
        DrawPlayerImage(render, player1, PLAYER_ATTACK_L_IMAGE);
        break;
    case ATTACK_R:
        DrawPlayerImage(render, player1, PLAYER_ATTACK_R_IMAGE);
        break;
    case DESTROY:
        DrawPlayerImage(render, player1, PLAYER_DESTROY_IMAGE);
        break;
    case DEATH:
        DrawPlayerImage(render, player1, PLAYER_DEATH_IMAGE);
        break;
    default:
        DrawPlayerImage(render, player1, PLAYER_DEFAULT_IMAGE);
        break;
    }
}

void Update(SDL_Renderer *render, SDL_Window *window, void (*callback)(SDL_Renderer *rnd))
{
    if (SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE))
    {
        SDL_Log("[ERROR] Fail to change render color \n%s\n", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(render);
    (*callback)(render);
    SDL_RenderPresent(render);
}