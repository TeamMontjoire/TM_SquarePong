#ifndef SDL_BUTTON_H
#define SDL_BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

typedef struct {
	SDL_Surface *surface;
	SDL_Rect rect;
	SDL_Texture *texture;
	SDL_Color colorText;
	SDL_Color colorBackground;
	SDL_Color colorHoverBackground;
	SDL_bool belongsButton;
	TTF_Font *font;
	int fontSize;
	int paddingTop;
	int paddingLeft;
	char *textButton;
}SDL_Button;

SDL_Button *SDL_CreateButton(SDL_Renderer *renderer, int x, int y, int w, int h, char *text);
void SDL_SetBackgroundColorButton(SDL_Button *button, int r, int g, int b);
void SDL_SetHoverBackgroundColorButton(SDL_Button *button, int r, int g, int b);
void SDL_SignalConnect(SDL_Button *button, SDL_Event event, void (*callback)(void*), void *data);
void SDL_UpdateButton(SDL_Button *button, SDL_Renderer *renderer, SDL_Event event);
void SDL_DestroyButton(SDL_Button *button);
#endif
