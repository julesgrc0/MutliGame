#ifndef _GAME_AUDIO_
#define _GAME_AUDIO_

#include<SDL_mixer.h>
#include "const.h"

void PlayMusic();
void PlayJumpSong();
void PlayLoseSong();
void LoadSongs();
void FreeSongs();

Mix_Music *background_song;
Mix_Music *jump_song;
Mix_Music *lose_song;


void PlayJumpSong()
{
    Mix_PlayChannel(-1, jump_song, 0);
}
void PlayLoseSong()
{
    Mix_PlayChannel(-1, lose_song, 0);
}

void LoadSongs()
{
    background_song = Mix_LoadMUS(SONG_BACKGROUND);
    // jump_song = Mix_LoadWAV(SONG_JUMP);
    // lose_song = Mix_LoadWAV(SONG_GAMEOVER);
}

void FreeSongs()
{
    if (background_song)
    {
        Mix_FreeChunk(background_song);
        background_song = NULL;
    }
    if (jump_song)
    {
        Mix_FreeChunk(jump_song);
        jump_song = NULL;
    }
    if (lose_song)
    {
        Mix_FreeChunk(lose_song);
        lose_song = NULL;
    }
}

void PlayMusic()
{
    Mix_PlayMusic(background_song, -1);
}

#endif // !_GAME_AUDIO_
