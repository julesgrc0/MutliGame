
#ifndef _GAME_UTILS_
#define _GAME_UTILS_

#include <sys/stat.h>

bool DEBUG = false;

typedef struct Point
{
    int x,y;
}Point;

typedef struct Size
{
    int w,h;
}Size;

typedef struct Wall
{
    int x;
    int h1;
    int h2;
}Wall;


bool access(const char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

char* Time()
{
    time_t current_time;
    char* s;
    current_time = time(NULL);
    s = ctime(&current_time);
    return s;
}

bool Write(const char* buffer)
{
    if(DEBUG && access(WINDOW_DEBUG))
    {
        FILE *f;
        f = fopen(WINDOW_DEBUG, "a");
        if(f)
        {
            f = fopen(WINDOW_DEBUG, "a");
            char bu[999];
            snprintf(bu, 999, "[INFO] %s - %s\n",Time(),buffer);
            fprintf(f, "%s", bu);
            printf("%s",bu);
            fclose(f);
            return true; 
        }
        return false;
    }
    return false;
}



int random(int min,int max)
{
    return (rand() % (max - min + 1)) + min;
}

void Init(int argc,char** argv)
{
    if (argc>1&&strcmp(argv[1], "--debug") == 0)    
    {  
        if(!access(WINDOW_DEBUG))
        {
            FILE * f;
            f = fopen(WINDOW_DEBUG, "w");
            fclose(f);
        }
    }
    if(access(WINDOW_DEBUG))
    {
        DEBUG = true;
        SDL_version nb;
        SDL_VERSION(&nb);
        Write("\nInit Main\n");
        Write("Success to find debug file\n");
        char bu[300];
        snprintf(bu, 300, "SDL version %d.%d.%d\n", nb.major, nb.minor, nb.patch);
        Write(bu);
        if(Write("Init Game"))
        {
            printf("[INFO] Success to update debug file\n");
        }
    }
}

#endif // !_GAME_UTILS_