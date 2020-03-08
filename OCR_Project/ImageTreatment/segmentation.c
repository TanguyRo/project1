# include "segmentation.h"

/*
====================================
=========CUTTING FUNCTIONS==========
====================================
*/

/*==================================*/

/* Cut the horizontal lines of the image */
SDL_Surface* lineCut(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	/*Line is white before*/
	int before = 1; //True

	SDL_Surface *img_copy = copy_image(img);

	int width = img_copy->w;
	int height = img_copy->h;

	for(int y = 0; y < height; y++)
	{
		int is_white = 1 ;
		for(int x = 0 ; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			/*Check if there is a black character in this line*/
			if(r != 255 && g != 255 && b != 255)
			{
				is_white = 0;
				break;
			}
		}
		/*For the first cut we cut the pixel line
		before the line with character(s)*/
		if(is_white == 0 && before == 1)
		{
			for(int k = 0; k < width; k++)
			{
				pixel = SDL_MapRGB(img_copy->format, 255, 0, 0);
				put_pixel(img_copy, k, y - 1, pixel);
			}
			before = 0;
		}
		/*For the second cut we cut the first white line
		after a line with character(s)*/
		if(is_white == 1 && before == 0)
		{
			for(int k = 0; k < width; k++)
			{
				pixel = SDL_MapRGB(img_copy->format, 255, 0, 0);
				put_pixel(img_copy, k, y, pixel);
			}
			before = 1;
		}
	}
	return(img_copy);
}

/*==================================*/

/* Display the isolated cuts */
void cuttedSurface(SDL_Surface *img, int firstCut,
		int lastCut, struct nr *network)
{
	int width = img->w;

	Uint32 pixel;
	SDL_Surface* copy = NULL;

	/*Create a surface that contain the cutting zone*/
	copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
			width,
			lastCut - firstCut,
			img->format -> BitsPerPixel, 0, 0, 0, 0);
	int width_copy = copy->w;
	int height_copy = copy->h;

	/*Copy the cutting zone into the new surface*/
	for(int x = 0; x < width_copy; x++)
	{
		for(int y = 0; y < height_copy; y++)
		{
			pixel = get_pixel(img, x, firstCut + y);
			put_pixel(copy, x, y, pixel);
		}
	}
	/*Cut the characters*/
	charCut(copy);
	isolateChar(copy, network);
	network -> str = concat(network -> str, "\n");
}

/*==================================*/

/* Isolate the lines */
void isolateLine(SDL_Surface *img, struct nr *network)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int height = img->h;

	/*Line is white before the text*/
	int before;
	/*Line is white after the text*/
	int lastCut = 0;

	for(int y = 0; y < height; y++)
	{
		if(y != 0 && y <= lastCut)
		{
			continue;
		}

		pixel = get_pixel(img, 0, y);
		SDL_GetRGB(pixel, img->format, &r, &g, &b);

		//If there is a red line start cut
		if(r == 255 && b == 0 && g == 0)
		{
			before = y + 1;

			for(int j = before + 1; j < height; j++)
			{
				pixel = get_pixel(img, 0, j);
				SDL_GetRGB(pixel, img->format, &r, &g, &b);

				/*Detect the second line*/
				if(r == 255 && b == 0 && g == 0)
				{
					lastCut = j;
					/*Isolate into a surface the linecut*/
					cuttedSurface(img, before, lastCut, network);
					break;
				}
			}
		}
	}
}

/*===================================*/

/* Cut the characters into the isolated cuts */
void charCut(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int thereIsChar = 0;
	int lineWasWhite;
	int canCut = 0;

	for(int x = 0; x < width; x++)
	{
		lineWasWhite = 1;
		for(int y = 0; y < height; y++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			if(r == 0 && g == 0 && b == 0)
			{
				thereIsChar = 1;
				lineWasWhite = 0;
				break;
			}
		}
		if(lineWasWhite && !canCut)
		{
			continue;
		}
		if(thereIsChar && !canCut)
		{
			for(int k = 0; k < height; k++)
			{
				pixel = SDL_MapRGB(img->format, 255, 0, 0);
				put_pixel(img, x - 1, k, pixel);
			}
			canCut = 1;
		}
		if(lineWasWhite && canCut)
		{
			for(int k = 0; k < height; k++)
			{
				pixel = SDL_MapRGB(img->format, 255, 0, 0);
				put_pixel(img, x, k, pixel);
			}
			canCut = 0;
		}
	}
}

/*====================================*/

void isolateChar(SDL_Surface *img, struct nr *network)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int firstCut;
	int lastCut = -1;
	int lastRead = -1;

	SDL_Surface* copy = NULL;
	SDL_Surface *resize = NULL;

	for(int i = 0; i < width; i++)
	{
		if(i < lastRead)
		{
			continue;
		}
		pixel = get_pixel(img, i, 0);
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		if(r == 255 && g == 0 && b == 0)
		{
			firstCut = i + 1;
			pixel = get_pixel(img, firstCut, 0);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			while(r == 255 && g == 0 && b == 0)
			{
				firstCut++;
				pixel = get_pixel(img, firstCut, 0);
				SDL_GetRGB(pixel, img->format, &r, &g, &b);
			}

			/*Search for second Cut*/
			for(int j = firstCut + 1; j < width; j++)
			{
				pixel = get_pixel(img, j, 0);
				SDL_GetRGB(pixel, img->format, &r, &g, &b);
				if(r == 255 && g == 0 && b == 0)
				{
					lastRead = j;
					lastCut = j - 1;

					/*Create a surface that contain the cutting zone*/
					copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
							lastCut-firstCut + 1,
							height,
							img->format->BitsPerPixel,0,0,0,0);
					
					int width_copy = copy->w;
					int height_copy = copy->h;

					/*Copy the cutting zone into the new surface*/
					for(int x = 0; x < width_copy; x++)
					{
						for(int y = 0; y < height_copy; y++)
						{
							pixel = get_pixel(img, firstCut + x, y);
							put_pixel(copy, x, y, pixel);
						}
					}
					resize = increaseChar(copy);
					/*Detect the chars*/
					double *letter = create_matrix(resize);
					/*Prevent false space*/
					int space = 1;
					//printf("%d\n",copy->w);
					if(copy->w <5)
					{
						for(int i = 0;i < 28*28; i++)
						{
							if((int)letter[i] == 1)
							{
								space = 0;
								break;
							}
						}
						if(space == 1)
							break;
					}

					space = 1;
					for(int i = 0;i < 28*28; i++)
					{
						if((int)letter[i] == 1)
						{
							space = 0;
							break;
						}
					}
					if(space)
					{

						network -> str = concat(network->str," ");
					}
					else
					{
						char res = DetectText(network, letter);
						char str[2] = "\0";
						str[0] = res;
						network -> str = concat(network -> str, str);
					}
					break;
				}
			}
		}
	}
}

/*=====================================*/

int mediumPixelSpacingHorizontal(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int whitePixel = 0;
	int blackPixel = 0;
	int res = 0;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
				whitePixel++;
			else
				blackPixel++;
		}

	}
	res = whitePixel / (blackPixel / 2);
	return res;
}

/*======================================*/

int mediumPixelSpacingVertical(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int whitePixel = 0;
	int blackPixel = 0;
	int res = 0;

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
				whitePixel++;
			else
				blackPixel++;
		}
	}
	res = whitePixel / (blackPixel / 2);
	return res;
}

/*=======================================*/

SDL_Surface* clean_image(SDL_Surface* img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	/*Width path*/
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if((r == 255 && g == 0 && b == 0) ||
				(r == 0 && g == 255 && b == 0) ||
				(r == 0 && g == 0 && b == 255))
			{
				Uint32 new_pixel = SDL_MapRGB(img->format, 255, 0, 0);
				put_pixel(img, x, y, new_pixel);
			}
		}
	}
	return img;
}

/*=====================================*/

/* RLSA Algorithm to detect blocks*/
void blockDetection_horizontal(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int C = mediumPixelSpacingHorizontal(img)*4;
	int countWhite;

	for(int y = 0; y < height; y++)
	{
		countWhite = 0;
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
			{
				countWhite++;
			}
			if(r == 0 && g == 0 && b == 0)
			{
				if(countWhite <= C)
				{
					int k = x - 1;
					while(countWhite > 0)
					{
						pixel = SDL_MapRGB(img -> format, 0, 0, 0);
						put_pixel(img, k, y, pixel);
						countWhite--;
						k--;
					}
				}
				else
				{
					countWhite = 0;
				}
			}
		}
	}
}

/*======================================*/

void blockDetection_vertical(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	int C = mediumPixelSpacingVertical(img) * 4;
	int countBlack;

	for(int x = 0; x < width; x++)
	{
		countBlack = 0;
		for(int y = 0; y < height; y++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
			{
				countBlack++;
			}
			else
			{
				if(countBlack <= C)
				{
					int k = y - 1;
					while(countBlack > 0)
					{
						pixel = SDL_MapRGB(img->format, 0, 0, 0);
						put_pixel(img, x, k, pixel);
						countBlack--;
						k--;
					}
				}
				else
				{
					countBlack = 0;
				}
			}
		}
	}
}
