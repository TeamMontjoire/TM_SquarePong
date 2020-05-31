#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MENU_INFO 64

#define E_SIZE 8	// elementary size

#define STEP 8

#define PI 3.14159

//~ #define DELTA_TIME 100

typedef struct {
	SDL_Rect left;
	SDL_Rect right;
	SDL_Rect top;
	SDL_Rect middle;
}Wall;

typedef enum { N, S } DirectionNS;
typedef enum { W, E } DirectionWE;

int start_game(SDL_Window *window, SDL_Renderer *renderer, int *score);
#endif
