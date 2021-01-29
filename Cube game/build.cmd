@echo off
g++ -std=c++1y "main.cpp" -o main -Wall  -I "C:\Win-builds\x86_64\include" -L "C:\Win-builds\x86_64\lib64" -lmingw32 -lSDL2main -lSDL2