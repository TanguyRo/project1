# ifndef IMAGE_OPERATIONS_H
# define IMAGE_OPERATIONS_H

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>
# include "../Tools/tools.h"

void greyscale(SDL_Surface *img);

int Histo_average(SDL_Surface *img);

void blacknwhite(SDL_Surface *img);

void noiseReduction(SDL_Surface *img);

SDL_Surface* increaseChar(SDL_Surface *img);

SDL_Surface* Resize(SDL_Surface *img);

# endif
