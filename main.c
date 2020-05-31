#include "main.h"
#include "game.h"
#include "SDL_Button.h"
#include "SDL_Label.h"

#define MARGIN_LEFT 120
#define HEIGHT_BUTTON 40

static
SDL_bool file_exist(char *path_file)
{
	SDL_RWops *f = SDL_RWFromFile(path_file, "r");
	if (f == NULL) return SDL_FALSE;
	SDL_RWclose(f);
	return SDL_TRUE;
}

static
SDL_bool write_config_file(int scoreMax)
{
	FILE *f = fopen("config.txt", "w");
	if (f==NULL) {
		printf("Error opening file config.txt\n");
		return SDL_FALSE;
	}
	fprintf(f, "%s %d%c", "SCORE_MAX", scoreMax, '\n');
	fclose(f);
	return SDL_TRUE;
}

static
int read_score_max()
{
	FILE *f = fopen("config.txt", "r");
	if (f==NULL) {
		printf("Error opening file config.txt\n");
		printf("Fix score max = 0\n");
		return 0;
	}
	char str[10];
	int scoreMax;
	fscanf(f, "%s %d", &str, &scoreMax);
	return scoreMax;
}

static
void start(void *pData)
{
	printf("Start\n");
	Mix_Pause(1);
	Data *data = pData;
	int result = start_game(data->window, data->renderer, &data->score);
	printf("Fin de partie - retour ecran accueil\n");
	Mix_Resume(1);
	data->quitPart = SDL_TRUE;
	printf("Score fin de partie : %d\n", data->score);
}

static
void quit(void *pData) {
	Data *data = pData;
	printf("Quit\n");
	data->quitGame = SDL_TRUE;
}

static
void play_sound(Mix_Chunk *sound) {
	if (Mix_PlayChannel(1, sound, -1) == -1) {
		SDL_Log("Couldn't play channel 1: %s", Mix_GetError());
	}
}

static
void update_renderer(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

int main(int argc, char* argv[]) {
	Data data;
	SDL_Event event;
	data.quitGame = SDL_FALSE;
	data.quitGame = SDL_FALSE;
	data.score = 0;
	char scoreStr[4];
	Mix_Chunk *soundIntro;
	
	if (file_exist("config.txt") == SDL_FALSE) write_config_file(0);
	
	data.scoreMax = read_score_max();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
		SDL_Log("Couldn't init : %s", SDL_GetError());
		return 1;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2024) <0) {
		SDL_Log("Couldn't open audio : %s", Mix_GetError());
		return 1;
	}
	
	if ( argc == 2 ) {
		if ( strcmp("fs", argv[1]) == 0 ) {
			printf("Option fs\n");
			data.window = SDL_CreateWindow("Square",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
		} else {
			printf("Mauvais argument : %s\n", argv[1]);
			exit(0);
		}
	} else {
		data.window = SDL_CreateWindow("Square",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	}
	
	if (data.window == NULL) { SDL_Log("Couldn't create Window : %s", SDL_GetError()); return 1; }
		
	data.renderer = SDL_CreateRenderer(data.window, -1, SDL_RENDERER_ACCELERATED);
	if (data.renderer == NULL) { SDL_Log("Couldn't create Renderer : %s", SDL_GetError()); return 1; }
	
	soundIntro = Mix_LoadWAV("sound/intro.wav");
	if (soundIntro == NULL) {SDL_Log("Couldn't load wav : %s", Mix_GetError());}
	play_sound(soundIntro);
	
	SDL_Button *buttonStart = SDL_CreateButton(data.renderer, MARGIN_LEFT, 80, 400, HEIGHT_BUTTON, "Start the Game");
	buttonStart->paddingLeft = 80;
	SDL_SetBackgroundColorButton(buttonStart, 0, 170, 170);
	SDL_SetHoverBackgroundColorButton(buttonStart, 170, 170, 170);
	
	SDL_Button *buttonQuit = SDL_CreateButton(data.renderer, MARGIN_LEFT, 200, 400, HEIGHT_BUTTON, "Quit");
	buttonQuit->paddingLeft = 160;
	SDL_SetBackgroundColorButton(buttonQuit, 0, 170, 170);
	SDL_SetHoverBackgroundColorButton(buttonQuit, 170, 170, 170);

	SDL_Label *labelScore = SDL_CreateLabel(data.renderer, 0, HEIGHT - HEIGHT_BUTTON, WIDTH, HEIGHT_BUTTON, "Score max : 0");
	SDL_SetBackgroundColorLabel(labelScore, 0, 170, 170);
	labelScore->fontSize = 18;
	
	while (!data.quitGame) {
		if ( SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					data.quitGame=SDL_TRUE;
					break;
			}//end switch event.type
			update_renderer(data.renderer);
			SDL_UpdateButton(buttonStart, data.renderer, event);
			SDL_UpdateButton(buttonQuit, data.renderer, event);
			SDL_SignalConnect(buttonStart, event, &start, &data);
			SDL_SignalConnect(buttonQuit, event, &quit, &data);
			if (data.quitPart == SDL_TRUE) {
				if (data.score > data.scoreMax) {
					sprintf(scoreStr, "Nouveau score max : %i", data.score);
					SDL_UpdateLabel(labelScore, data.renderer);
					SDL_SetTextLabel(labelScore, data.renderer, scoreStr);
					data.scoreMax = data.score;
					write_config_file(data.score);
				}
				data.quitPart = SDL_FALSE;
			} 
			else {
				sprintf(scoreStr, "Score max : %i | Your Score : %i", data.scoreMax, data.score);
				SDL_UpdateLabel(labelScore, data.renderer);
				SDL_SetTextLabel(labelScore, data.renderer, scoreStr);
			}

			SDL_RenderPresent(data.renderer);
		}//endif PollEvent
	}//end while quitGame
		
	Mix_FreeChunk(soundIntro);
	SDL_DestroyLabel(labelScore);
	SDL_DestroyButton(buttonStart);
	SDL_DestroyRenderer(data.renderer);
	SDL_DestroyWindow(data.window);
	SDL_Quit();
	return 0;
}
