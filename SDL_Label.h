#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect rect;
	SDL_Color colorText;
	SDL_Color colorBackground;
	TTF_Font *font;
	int fontSize;
	int paddingTop;
	int paddingLeft;
	char *textLabel;
}SDL_Label;

SDL_Label *SDL_CreateLabel(SDL_Renderer *renderer, int x, int y, int w, int h, char *text);
void SDL_UpdateLabel(SDL_Label *label, SDL_Renderer *renderer);
void SDL_SetBackgroundColorLabel(SDL_Label *label, int r, int g, int b);
void SDL_SetTextLabel(SDL_Label *label, SDL_Renderer *renderer, char *text);
void SDL_DestroyLabel(SDL_Label *label);
#endif
