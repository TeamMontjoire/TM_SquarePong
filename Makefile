OS=$(shell uname -o)

# Verif OS
ifeq ($(OS), Msys)
	OSFLAG = -lmingw32 -lSDL2main
endif

all: square

square: main.o SDL_Button.o SDL_Label.o game.o
	gcc -o square main.c SDL_Button.c SDL_Label.c game.c $(OSFLAG) -lSDL2 -lSDL2_ttf -lSDL2_mixer
	
main.o: main.c main.h
	gcc -c main.c $(OSFLAG) -lSDL2 -lSDL2_ttf

SDL_Button.o: SDL_Button.c SDL_Button.h
	gcc -c SDL_Button.c

SDL_Label.o: SDL_Label.c SDL_Label.h
	gcc -c SDL_Label.c

game.o: game.c game.h main.h
	gcc -c game.c -lSDL2

clean:
	rm *.o
