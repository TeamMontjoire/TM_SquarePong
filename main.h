#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_bool quitGame;
	SDL_bool quitPart;
	int score;
	int scoreMax;
}Data;

#endif
