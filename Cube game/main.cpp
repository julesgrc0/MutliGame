#include <iostream>
#include <fstream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>
#include <string>
#include <chrono>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "src/render.h"

using namespace std;

void Update();
void Draw();
void DrawCube(int x, int y);
void MoveCase(SDL_Event event);
void DrawAround(int x, int y);
void MoveSpecial(SDL_Event event);
void AddPoint(int x, int y);
void DrawCubeFill(int x, int y);
std::vector<std::string> split(std::string const &str, const char delim);
void DrawColisionAround(int x, int y);

int W_HEIGHT = 400;
int W_WIDTH = 600;
#define ITEM_S 10

bool run = true;
bool SpaceActive = false;
bool HideMouse = false;

SDL_Window *window;
SDL_Renderer *render;
Item player = {ITEM_S, ITEM_S, ITEM_S};
Item special = {ITEM_S, ITEM_S, ITEM_S};
Int16 mouse = {0, 0};
std::vector<Int16> points = {};

int main(int argc, char *argv[])
{
    int w, h;
    bool change = false;
    if (argc >= 2)
    {
        ifstream backup(argv[1]);
        if (backup.is_open())
        {
            string line;
            int i = 0;
            while (getline(backup, line))
            {
                switch (i)
                {
                case 0:
                    if (split(line, ' ').size() >= 2)
                    {
                        player.x = atoi(split(line, ' ')[0].c_str());
                        player.y = atoi(split(line, ' ')[1].c_str());
                    }
                    break;
                case 1:
                    if (split(line, ' ').size() >= 2)
                    {
                        w = atoi(split(line, ' ')[0].c_str());
                        h = atoi(split(line, ' ')[1].c_str());
                        if (w != W_WIDTH || h != W_HEIGHT)
                        {
                            change = true;
                        }
                    }
                    break;
                default:
                    if (split(line, ' ').size() >= 2)
                    {
                        int x = atoi(split(line, ' ')[0].c_str());
                        int y = atoi(split(line, ' ')[1].c_str());
                        points.push_back({x, y});
                    }
                    break;
                }
                i++;
            }
            backup.close();
        }
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(W_WIDTH, W_HEIGHT, NULL, &window, &render) != 0)
    {
        return 1;
    }

    if (change)
    {
        W_WIDTH = w;
        W_HEIGHT = h;
        SDL_SetWindowSize(window, W_WIDTH, W_HEIGHT);
    }

    int last = 0;
    int current = 0;
    int ms = 0;
    int seconde = 0;
    while (run)
    {
        current = SDL_GetTicks();
        if (current - last > 10)
        {
            ms += 10;
            if (ms >= 1000)
            {
                seconde++;
                ms = 0;
                if (seconde >= 1)
                {
                    HideMouse = true,
                    seconde = 0;
                }
            }
            Update();
            if (!SpaceActive)
            {
                string title = "x: " + to_string(player.x) + " y:" + to_string(player.y);
                SDL_SetWindowTitle(window, title.c_str());
            }
            else
            {
                string title = "Distance: " + to_string(distance({player.x, player.y}, {special.x, special.y}));
                SDL_SetWindowTitle(window, title.c_str());
            }
            last = current;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                run = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    if (SpaceActive)
                    {
                        player = special;
                    }
                    else
                    {
                        special = player;
                    }
                    SpaceActive = !SpaceActive;
                }
                else
                {
                    if (SpaceActive)
                    {
                        MoveSpecial(event);
                    }
                    else
                    {

                        MoveCase(event);
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                HideMouse = false,
                seconde = 0;
                mouse = {event.button.x / (ITEM_S * 2), event.button.y / (ITEM_S * 2)};
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x = (event.button.x / (ITEM_S * 2)) * (ITEM_S * 2);
                int y = (event.button.y / (ITEM_S * 2)) * (ITEM_S * 2);
                AddPoint(x, y);
                break;
            }
        }
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    ofstream backup;
    backup.open("backup.txt");
    backup << player.x << " " << player.y << endl;
    backup << W_WIDTH << " " << W_HEIGHT << endl;
    for (Int16 p : points)
    {
        backup << p.a << " " << p.b << endl;
    }
    backup.close();
    return 0;
}

void MoveSpecial(SDL_Event event)
{
    Movement action = NONE;
    int unity = ITEM_S * 2;

    switch (event.key.keysym.sym)
    {
    case SDLK_LEFT:
        action = LEFT;
        special.x -= unity;
        break;
    case SDLK_RIGHT:
        action = RIGHT;
        special.x += unity;
        break;
    case SDLK_UP:
        action = TOP;
        special.y -= unity;
        break;
    case SDLK_DOWN:
        action = BOTTOM;
        special.y += unity;
        break;
    }

    for (Int16 p : points)
    {
        if ((p.a + ITEM_S) == special.x && (p.b + ITEM_S) == special.y)
        {
            switch (action)
            {
            case TOP:
                special.y += unity;
                break;
            case BOTTOM:
                special.y -= unity;
                break;
            case LEFT:
                special.x += unity;
                break;
            case RIGHT:
                special.x -= unity;
                break;
            }
        }
    }

    Int16 res = isIn({0, 0}, {W_WIDTH, W_HEIGHT}, {special.x, special.y}, {special.size, special.size});
    if (res.a != 0 || res.b != 0)
    {
        if (res.a > W_WIDTH || res.a < 0)
        {
            special.x = special.size;
            if (res.a > W_WIDTH)
            {
                special.x = W_WIDTH - special.size;
            }
        }
        if (res.b > W_HEIGHT || res.b < 0)
        {
            special.y = special.size;
            if (res.b > W_HEIGHT)
            {
                special.y = W_HEIGHT - special.size;
            }
        }
    }
}

void AddPoint(int x, int y)
{
    int i = 0;
    for (Int16 p : points)
    {
        if (p.a == x && p.b == y)
        {
            points.erase(points.begin() + i);
            return;
        }
        i++;
    }
    points.push_back({x, y});
}

void MoveCase(SDL_Event event)
{
    Movement action = NONE;
    int unity = ITEM_S * 2;

    switch (event.key.keysym.sym)
    {
    case SDLK_LEFT:
        action = LEFT;
        player.x -= unity;
        break;
    case SDLK_RIGHT:
        action = RIGHT;
        player.x += unity;
        break;
    case SDLK_UP:
        action = TOP;
        player.y -= unity;
        break;
    case SDLK_DOWN:
        action = BOTTOM;
        player.y += unity;
        break;
    }

    for (Int16 p : points)
    {
        if ((p.a + ITEM_S) == player.x && (p.b + ITEM_S) == player.y)
        {
            switch (action)
            {
            case TOP:
                player.y += unity;
                break;
            case BOTTOM:
                player.y -= unity;
                break;
            case LEFT:
                player.x += unity;
                break;
            case RIGHT:
                player.x -= unity;
                break;
            }
        }
    }

    Int16 res = isIn({0, 0}, {W_WIDTH, W_HEIGHT}, {player.x, player.y}, {player.size, player.size});
    if (res.a != 0 || res.b != 0)
    {
        if (res.a > W_WIDTH || res.a < 0)
        {
            player.x = player.size;
            if (res.a > W_WIDTH)
            {
                player.x = W_WIDTH - player.size;
            }
        }
        if (res.b > W_HEIGHT || res.b < 0)
        {
            player.y = player.size;
            if (res.b > W_HEIGHT)
            {
                player.y = W_HEIGHT - player.size;
            }
        }
    }
}

std::vector<std::string> split(std::string const &str, const char delim)
{
    size_t start;
    size_t end = 0;
    std::vector<std::string> out = {};

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
    return out;
}

void DrawAround(int x, int y)
{
    // top
    DrawCube(x - (ITEM_S * 3), y - ITEM_S * 3);
    DrawCube(x - ITEM_S, y - ITEM_S * 3);
    DrawCube(x + ITEM_S, y - ITEM_S * 3);
    // center
    DrawCube(x - ITEM_S * 3, y - ITEM_S);
    DrawCube(x - ITEM_S, y - ITEM_S);
    DrawCube(x + ITEM_S, y - ITEM_S);
    // bottom
    DrawCube(x - (ITEM_S * 3), y + ITEM_S);
    DrawCube(x - ITEM_S, y + ITEM_S);
    DrawCube(x + ITEM_S, y + ITEM_S);
}

void DrawColisionAround(int x, int y)
{
    vector<Int16> m = {
        {x - (ITEM_S * 3), y - ITEM_S * 3},
        {x - ITEM_S, y - ITEM_S * 3},
        {x + ITEM_S, y - ITEM_S * 3},

        {x - ITEM_S * 3, y - ITEM_S},
        {x - ITEM_S, y - ITEM_S},
        {x + ITEM_S, y - ITEM_S},

        {x - (ITEM_S * 3), y + ITEM_S},
        {x - ITEM_S, y + ITEM_S},
        {x + ITEM_S, y + ITEM_S}};

    for (Int16 cube : m)
    {
        SetColor(render, 20, 157, 179);
        for (Int16 p : points)
        {
            if(p.a==cube.a&&cube.b==p.b)
            {
                SetColor(render,209, 21, 21);
            }
        }
        DrawCube(cube.a,cube.b);
    }
}

void DrawCube(int x, int y)
{
    SDL_Rect rect = {x, y, ITEM_S * 2, ITEM_S * 2};
    SDL_RenderDrawRect(render, &rect);
}

void DrawCubeFill(int x, int y)
{
    SDL_Rect rect = {x, y, ITEM_S * 2, ITEM_S * 2};
    SDL_RenderDrawRect(render, &rect);
    for (int Px = x; Px < (x + ITEM_S * 2); Px++)
    {
        for (int Py = y; Py < (y + ITEM_S * 2); Py++)
        {
            SDL_RenderDrawPoint(render, Px, Py);
        }
    }
}

void Draw()
{
    SetColor(render, 100, 100, 100);
    for (int x = 0; x < W_WIDTH; x += ITEM_S * 2)
    {
        for (int y = 0; y < W_HEIGHT; y += ITEM_S * 2)
        {
            SDL_RenderDrawLine(render, 0, y, W_WIDTH, y);
            SDL_RenderDrawLine(render, x, 0, x, W_HEIGHT);
        }
    }

    for (Int16 p : points)
    {
        DrawCubeFill(p.a, p.b);
    }

    int x = player.x;
    int y = player.y;
    if (!SpaceActive)
    {
        SetColor(render, 20, 157, 179);
        // DrawAround(x, y);
        DrawColisionAround(x, y);
        SetColor(render, 255, 255, 255);
    }
    else
    {
        SetColor(render, 255, 0, 0);
        DrawCube(special.x - ITEM_S, special.y - ITEM_S);
        SetColor(render, 200, 200, 200);
    }
    SDL_RenderDrawCircle(render, player.x, player.y, player.size);
    if (!HideMouse)
    {
        SetColor(render, 200, 200, 200);
        DrawCube(mouse.a * ITEM_S * 2, mouse.b * ITEM_S * 2);
    }
}

void Update()
{
    SetColor(render, 0, 0, 0);
    SDL_RenderClear(render);
    Draw();
    SDL_RenderPresent(render);
}
