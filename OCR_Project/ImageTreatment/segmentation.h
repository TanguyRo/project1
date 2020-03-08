# ifndef SEGMENTATION_H
# define SEGMENTATION_H

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>
# include "../Tools/tools.h"
# include "image_operations.h"
# include "../character_detection/character_detection.h"


SDL_Surface* lineCut(SDL_Surface *img);

void isolateLine(SDL_Surface *img, struct nr *network);

void cuttedSurface(SDL_Surface *img, int firstCut,
                    int lastCut, struct nr *network);

void charCut(SDL_Surface *img);

void isolateChar(SDL_Surface *img, struct nr *network);

int mediumPixelSpacingHorizontal(SDL_Surface *img);

int mediumPixelSpacingVertical(SDL_Surface *img);

SDL_Surface* clean_image(SDL_Surface* img);

void blockDetection_horizontal(SDL_Surface *img);

void blockDetection_vertical(SDL_Surface *img);

# endif
