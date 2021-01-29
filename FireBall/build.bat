@echo off
gcc main.c -o bin/app -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer && cd bin && app && cd ..