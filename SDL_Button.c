#include "SDL_Button.h"

static SDL_bool check_belongs_button(SDL_Button *button, SDL_Event event) {
	if (
		event.motion.x > button->rect.x && 
		event.motion.x < button->rect.x + button->rect.w &&
		event.motion.y > button->rect.y &&
		event.motion.y < button->rect.y + button->rect.h
		)
		{
			button->belongsButton = SDL_TRUE;
			return SDL_TRUE;
		}
	else
	{
		button->belongsButton = SDL_FALSE;
		return SDL_FALSE;
	}
}

static void SDL_UpdateText(SDL_Button *this, SDL_Renderer *renderer)
{
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);

	this->surface = TTF_RenderText_Solid(this->font, this->textButton, this->colorText);
	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	
	this->surface->clip_rect.x = this->rect.x + this->paddingLeft;
	this->surface->clip_rect.y = this->rect.y + this->paddingTop;
	SDL_RenderCopy(renderer, this->texture, NULL, &this->surface->clip_rect);

}

void SDL_SetBackgroundColorButton(SDL_Button *this, int r, int g, int b)
{
	this->colorBackground.r = r;
	this->colorBackground.g = g;
	this->colorBackground.b = b;
}

void SDL_SetHoverBackgroundColorButton(SDL_Button *this, int r, int g, int b)
{
	this->colorHoverBackground.r = r;
	this->colorHoverBackground.g = g;
	this->colorHoverBackground.b = b;
}

SDL_Button *SDL_CreateButton(SDL_Renderer *renderer, int x, int y, int w, int h, char *text)
{
	SDL_Button *this = (SDL_Button*) malloc(sizeof(SDL_Button));
	size_t size = strlen(text) + 1;
	this->textButton = (char*) malloc(size);
	strcpy(this->textButton, text);
	
	this->colorText.r = 0;
	this->colorText.g = 0;
	this->colorText.b = 0;
	this->colorText.a = 255;
	this->colorBackground.r = 170;
	this->colorBackground.g = 170;
	this->colorBackground.b = 170;
	this->colorBackground.a = 255;
	this->colorHoverBackground.r = 200;
	this->colorHoverBackground.g = 200;
	this->colorHoverBackground.b = 200;
	this->colorHoverBackground.a = 255;
	this->paddingTop = 4;
	this->paddingLeft = 6;
	this->fontSize = 26;
	
	if (TTF_Init() == -1) {SDL_Log("Couldn't init ttf : %s", TTF_GetError());return NULL;}
	this->font = TTF_OpenFont("unifont.ttf", this->fontSize);
	if (this->font == NULL) {SDL_Log("Couldn't open font : %s", TTF_GetError());return NULL;}
	
	TTF_SetFontStyle(this->font, TTF_STYLE_BOLD);
	
	this->surface = TTF_RenderText_Solid(this->font, this->textButton, this->colorText);
	if (this->surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return NULL;}

	this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
	if (this->texture == NULL) {SDL_Log("Couldn't create texture : %s", SDL_GetError());return NULL;}
	
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->belongsButton = SDL_FALSE;
	
	return this;
}


void SDL_SignalConnect(
		SDL_Button *this,
		SDL_Event event,
		void (*callback)(void*),
		void *data)
{
	SDL_bool buttonHover = check_belongs_button(this, event);
	if (buttonHover) {
		if (event.button.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT)
			{
					(*callback)(data);
			}//endif event.button.button
		}//end if event.button.type
	}//endif buttonHover
}

void SDL_UpdateButton(SDL_Button *this, SDL_Renderer *renderer, SDL_Event event)
{
	SDL_bool buttonHover = check_belongs_button(this, event);

	switch(buttonHover) {
		case SDL_TRUE:
			SDL_SetRenderDrawColor(renderer,
				this->colorHoverBackground.r,
				this->colorHoverBackground.g,
				this->colorHoverBackground.b,
				this->colorHoverBackground.a);
			SDL_RenderFillRect(renderer, &this->rect);
			SDL_RenderDrawRect(renderer, &this->rect);
			break;
		case SDL_FALSE:
			SDL_SetRenderDrawColor(renderer,
				this->colorBackground.r,
				this->colorBackground.g,
				this->colorBackground.b,
				this->colorBackground.a);
			SDL_RenderFillRect(renderer, &this->rect);
			SDL_RenderDrawRect(renderer, &this->rect);
			break;
		default:
			SDL_SetRenderDrawColor(renderer,
				this->colorHoverBackground.r,
				this->colorHoverBackground.g,
				this->colorHoverBackground.b,
				this->colorHoverBackground.a);
			SDL_RenderFillRect(renderer, &this->rect);
			SDL_RenderDrawRect(renderer, &this->rect);
	}//end switch
	SDL_UpdateText(this, renderer);
}


void SDL_DestroyButton(SDL_Button *this)
{
	TTF_CloseFont(this->font);
	TTF_Quit();
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->surface);
	free(this);
}
