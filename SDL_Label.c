#include "SDL_Label.h"

SDL_Label *SDL_CreateLabel(SDL_Renderer *renderer, int x, int y, int w, int h, char *text)
{
	SDL_Label *this = (SDL_Label*) malloc(sizeof(SDL_Label));
	size_t size = strlen(text)+1;
	this->textLabel = (char*) malloc(size);
	strcpy(this->textLabel, text);
	
	this->colorText.r = 0;
	this->colorText.g = 0;
	this->colorText.b = 0;
	this->colorText.a = 255;
	this->colorBackground.r = 170;
	this->colorBackground.g = 170;
	this->colorBackground.b = 170;
	this->colorBackground.a = 255;
	this->paddingLeft = 6;
	this->paddingTop = 4;
	this->fontSize = 26;
	
	if (TTF_Init() == -1) {SDL_Log("Couldn't init ttf : %s", TTF_GetError());return NULL;}
	this->font = TTF_OpenFont("unifont.ttf", this->fontSize);
	TTF_SetFontStyle(this->font, TTF_STYLE_BOLD);
	
	this->surface = TTF_RenderText_Solid(this->font, this->textLabel, this->colorText);
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}
	
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Couldn't create texture : %s", SDL_GetError());return NULL;}
	
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	return this;
}

void SDL_UpdateLabel(SDL_Label *this, SDL_Renderer *renderer)
{
	this->surface->clip_rect.x = this->rect.x + this->paddingLeft;
	this->surface->clip_rect.y = this->rect.y + this->paddingTop;
	SDL_SetRenderDrawColor(renderer,
		this->colorBackground.r,
		this->colorBackground.g,
		this->colorBackground.b,
		this->colorBackground.a);
	SDL_RenderFillRect(renderer, &this->rect);
	SDL_RenderDrawRect(renderer, &this->rect);
	SDL_RenderCopy(renderer, this->texture, NULL, &this->surface->clip_rect);
}

void SDL_SetTextLabel(SDL_Label *this, SDL_Renderer *renderer, char *text)
{
	size_t size = strlen(text)+1;
	this->textLabel = (char*) malloc(size);
	strcpy(this->textLabel, text);
	
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	
	this->font = TTF_OpenFont("unifont.ttf", this->fontSize);
	this->surface = TTF_RenderText_Solid(this->font, this->textLabel, this->colorText);
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	this->surface->clip_rect.x = this->rect.x + this->paddingLeft;
	this->surface->clip_rect.y = this->rect.y + this->paddingTop;
	SDL_RenderFillRect(renderer, &this->rect);
	SDL_RenderDrawRect(renderer, &this->rect);
	SDL_RenderCopy(renderer, this->texture, NULL, &this->surface->clip_rect);
}

void SDL_SetBackgroundColorLabel(SDL_Label *this, int r, int g, int b)
{
	this->colorBackground.r = r;
	this->colorBackground.g = g;
	this->colorBackground.b = b;
}

void SDL_DestroyLabel(SDL_Label *this)
{
	TTF_CloseFont(this->font);
	TTF_Quit();
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
