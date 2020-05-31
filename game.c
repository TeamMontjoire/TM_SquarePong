#include "main.h"
#include "game.h"

static void init_wall(Wall *w) {
	w->left.x=0; w->left.y=MENU_INFO; w->left.w=E_SIZE; w->left.h=HEIGHT;
	w->right.x=WIDTH - E_SIZE; w->right.y=MENU_INFO; w->right.w=E_SIZE; w->right.h=HEIGHT;
	w->top.x=0; w->top.y=MENU_INFO; w->top.w=WIDTH; w->top.h=E_SIZE;
	w->middle.x=38*E_SIZE; w->middle.y=18*E_SIZE; w->middle.w=E_SIZE; w->middle.h=23*E_SIZE;
}

static SDL_bool check_collision(SDL_Rect rect1, SDL_Rect rect2) {
	if (rect1.x+rect1.w < rect2.x) return SDL_FALSE;
	if (rect1.y+rect1.h < rect2.y) return SDL_FALSE;
	if (rect1.y > rect2.y+rect2.h) return SDL_FALSE;
	if (rect1.x > rect2.x+rect2.w) return SDL_FALSE;
	return SDL_TRUE;
}

static SDL_bool check_ball_attach(SDL_Rect rPlayer, SDL_Rect rball) {
	if ( rball.y + E_SIZE == rPlayer.y) return SDL_TRUE;
	return SDL_FALSE;
}

static int random_between(int min, int max) {
	return min + rand()%(max-min+1);
}

static
void play_sound(Mix_Chunk *sound) {
	if (Mix_PlayChannel(-1, sound, 0) == -1) {
		SDL_Log("Couldn't play channel 1: %s", Mix_GetError());
	}
}

static int update_menu(SDL_Renderer *renderer, int score, int delta_time) {
	SDL_Surface *surface;
	SDL_Texture *texture;
	TTF_Font *font;
	SDL_Color colorText = {0, 0, 0};
	char text[50];
	
	if ( TTF_Init() == -1 ) {SDL_Log("Couldn't init ttf : %s\n", TTF_GetError());return 1;}

	font = TTF_OpenFont("unifont.ttf", 26);
	if (font == NULL) {SDL_Log("Couldn't open ttf : %s\n", TTF_GetError());return 1;}
	
	sprintf(text, "Score : %d       Frame (ms) : %d", score, delta_time);
	surface = TTF_RenderText_Solid(font, text, colorText);
	if (surface == NULL) {SDL_Log("Couldn't create surface : %s", SDL_GetError());return 1;}
	
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {SDL_Log("Coulndn't create texture : %s", SDL_GetError());return 1;}
	
	surface->clip_rect.x = 64;
	surface->clip_rect.y = 16;
	SDL_RenderCopy(renderer, texture, NULL, &surface->clip_rect);
	
	TTF_CloseFont(font);
	TTF_Quit();	
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return 0;
}

static void update_decor(SDL_Renderer *renderer, Wall wall) {
	SDL_SetRenderDrawColor(renderer, 170,170,170,255);
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &wall.left);
	SDL_RenderDrawRect(renderer, &wall.left);
	SDL_RenderFillRect(renderer, &wall.right);
	SDL_RenderDrawRect(renderer, &wall.right);
	SDL_RenderFillRect(renderer, &wall.top);
	SDL_RenderDrawRect(renderer, &wall.top);
	SDL_RenderFillRect(renderer, &wall.middle);
	SDL_RenderDrawRect(renderer, &wall.middle);
}

static void update_ball(SDL_Renderer *renderer, SDL_Rect rBall) {
	SDL_SetRenderDrawColor(renderer, 0, 170, 170, 255);
	SDL_RenderFillRect(renderer, &rBall);
	SDL_RenderDrawRect(renderer, &rBall);
}

static void update_player(SDL_Renderer *renderer, SDL_Rect rPlayer) {
	SDL_SetRenderDrawColor(renderer, 170,0,0,255);
	SDL_RenderFillRect(renderer, &rPlayer);
	SDL_RenderDrawRect(renderer, &rPlayer);
}

static
void update_scene(SDL_Renderer *renderer, Wall wall,
				SDL_Rect rPlayer, SDL_Rect rBall, int score, int delta_time) {
	
	update_decor(renderer, wall);
	update_menu(renderer, score, delta_time);
	update_player(renderer, rPlayer);
	update_ball(renderer, rBall);
	
	SDL_RenderPresent(renderer);
}

int end_game(int score, Mix_Chunk *soundEnd) {
	play_sound(soundEnd);
	printf("Score final : %d\n", score);
	printf("Fin de partie\n");
	return 0;
}

int start_game(SDL_Window *window, SDL_Renderer *renderer, int *score) {
	srand(time(NULL));
	int PITCH = random_between(-80, 80); // pour placer la barre du joueur a + ou - 80 aleatoirement
	printf("Def rect\n");
	SDL_Rect rPlayer = {WIDTH/2 + PITCH, HEIGHT - E_SIZE, 5*E_SIZE, E_SIZE}; // on se place au milieu a + ou - 80
	SDL_Rect rBall = {WIDTH/2+2*E_SIZE + PITCH, HEIGHT - 2*E_SIZE, E_SIZE, E_SIZE};
	SDL_Event event;
	Wall wall;
	SDL_bool quit = SDL_FALSE, moveBall = SDL_FALSE, collision, ball_attach;
	Uint32 lastTime=0, currentTime;
	int DELTA_TIME = 78;
	int SCORE = 0;
	Mix_Chunk *soundCollision, *soundEnd;

	
	DirectionWE directionWE = random_between(0, 1);
	DirectionNS directionNS = N;

	printf("init_wall\n");
	init_wall(&wall);
	update_scene(renderer, wall, rPlayer, rBall, SCORE, DELTA_TIME);
	soundCollision = Mix_LoadWAV("sound/collision.wav");
	if (soundCollision == NULL) {SDL_Log("Couldn't load wav : %s", Mix_GetError());}
	
	soundEnd = Mix_LoadWAV("sound/arpege.wav");
	if (soundEnd == NULL) {SDL_Log("Couldn't load wav : %s", Mix_GetError());}
	
	SDL_Delay(400);
	moveBall = SDL_TRUE;
	
	while (!quit) {
		if ( SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit=SDL_TRUE;
					break;
				
				case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
							case SDLK_ESCAPE:
								quit=SDL_TRUE;
								break;
						}//end switch event.key
						break;
			}//end switch event.type
		}//endif PollEvent
		
		currentTime = SDL_GetTicks();
		if (currentTime - lastTime > DELTA_TIME) {
			if (event.key.state == SDL_PRESSED)
			{
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
								collision = check_collision(rPlayer, wall.left);
								if (!collision) rPlayer.x -= STEP;
								if (ball_attach && !collision) rBall.x -= STEP;
								break;
							case SDLK_RIGHT:
								collision = check_collision(rPlayer, wall.right);
								if (!collision) rPlayer.x += STEP;
								if (ball_attach && !collision) rBall.x += STEP;
								break;
					}//end switch event.key.keysym.sym
					update_scene(renderer, wall, rPlayer, rBall, SCORE, DELTA_TIME);
			}//end event.key.state
			
			if (moveBall) {
				switch (directionNS) {
					case N:
						rBall.y -= STEP;
						break;
					case S:
						rBall.y += STEP;
						break;
				}//end switch NS
				switch (directionWE) {
					case W:
					 rBall.x -= STEP;
					 if (check_collision(rBall, wall.middle)) {play_sound(soundCollision);directionWE = E;}
					 break;
					case E:
					 rBall.x += STEP;
					 if (check_collision(rBall, wall.middle)) {play_sound(soundCollision);directionWE = W;}
					 break;
				}//end switch WE
				update_scene(renderer, wall, rPlayer, rBall, SCORE, DELTA_TIME);
			}//end if moveBall
			ball_attach = check_ball_attach(rPlayer, rBall);
			if (check_collision(rBall, wall.top) ) { directionNS = S; play_sound(soundCollision);}
			if ( check_collision(rBall, wall.right) ) { directionWE = W; play_sound(soundCollision);}
			if ( check_collision(rBall, wall.left) ) { directionWE = E; play_sound(soundCollision);}
			
			if ( check_collision(rBall, rPlayer) ) {
				play_sound(soundCollision);
				directionNS = N; SCORE += 1;
				DELTA_TIME -= 1;
				//~ printf("Score : %i\tDelta Time : %i \n", SCORE, DELTA_TIME);
			}
			
			if ( rBall.y > HEIGHT ) {
				int res=end_game(SCORE, soundEnd);
				//~ printf("res = %d\n", res);
				*score = SCORE;
				quit = SDL_TRUE;
			}		
			lastTime = currentTime;
		}//end if DELTA_TIME
	}//end while
	printf("Fin game.c\n");
	
	Mix_FreeChunk(soundCollision);
	return 0;
}
