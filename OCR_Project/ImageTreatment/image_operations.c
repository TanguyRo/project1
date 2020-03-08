# include "image_operations.h"

/*
======================================
===========Image Treatment============
======================================
*/

/*=====================================*/

/* Transform the image in a greyscale image */
void greyscale(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	/* Iterate on each pixels the greyscale */
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			/* Apply luminance on r g b */
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);

			Uint8 average = 0.3*r + 0.59*g + 0.11*b;

			/*Get the new pixel value from the new RGB values*/
			Uint32 new_pixel = SDL_MapRGB(img->format, 
						average, average, average);
			put_pixel(img, x, y, new_pixel);
		}
	}
}

/*=====================================*/

/*Find the medium value of all pixel in the image*/
int Histo_average(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;
	int average = 0;

	int width = img->w;
	int height = img->h;

	int totalPixel = width * height;
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			average += r;
		}
	}
	average = average / totalPixel;
	return average;
}

/*=====================================*/

/* Transform image in Binary Image. Change each pixel in black or white*/
void blacknwhite(SDL_Surface *img)
{
	Uint32 pixel;
	Uint32 new_pixel;
	Uint8 r, g, b;

	int width = img->w;
	int height = img->h;

	//Uint8 average = Histo_average(img);
	/* Iterate on each pixels the blacknwhite function*/
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			pixel = get_pixel(img,x,y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			if(r >= 127 && g >= 127 && b >= 127) /*or r >= average*/
				new_pixel = SDL_MapRGB(img->format, 255, 255, 255);
			else
				new_pixel = SDL_MapRGB(img->format, 0, 0, 0);

			/*Put new_pixel value on the surface*/
			put_pixel(img, x, y, new_pixel);
		}
	}
}

/*====================================*/

/* Noise reduction X4 for the moment...*/
void noiseReduction(SDL_Surface *img)
{
	/* Variables */
	int w;
	int h;
	w = img -> w;
	h = img -> h;
	int pixelTable[4];

	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			for(int k = j; k <= j + 4; k++)
			{
				//Borders of picture
				if(i == 0)
				{
					if(k == 0)
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k);
						pixelTable[2] = get_pixel(img, i, k);
						pixelTable[3] = get_pixel(img, i, k + 1);
						pixelTable[4] = get_pixel(img, i + 1, k);
						break;
					}
					if(k == h)
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k - 1);
						pixelTable[2] = get_pixel(img, i, k);
						pixelTable[3] = get_pixel(img, i, k);
						pixelTable[4] = get_pixel(img, i + 1, k);
						break;
					}
					else
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k - 1);
						pixelTable[2] = get_pixel(img, i, k);
						pixelTable[3] = get_pixel(img, i, k + 1);
						pixelTable[4] = get_pixel(img, i + 1, k);
						break;
					}
				}
				if(i == w)
				{
					if(k == 0)
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k);
						pixelTable[2] = get_pixel(img, i - 1, k);
						pixelTable[3] = get_pixel(img, i, k + 1);
						pixelTable[4] = get_pixel(img, i, k);
						break;
					}
					if(k == h)
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k - 1);
						pixelTable[2] = get_pixel(img, i - 1, k);
						pixelTable[3] = get_pixel(img, i, k);
						pixelTable[4] = get_pixel(img, i, k);
						break;
					}
					else
					{
						pixelTable[0] = get_pixel(img, i, k);
						pixelTable[1] = get_pixel(img, i, k - 1);
						pixelTable[2] = get_pixel(img, i - 1, k);
						pixelTable[3] = get_pixel(img, i, k + 1);
						pixelTable[4] = get_pixel(img, i, k);
						break;
					}
				}
				if(k == 0)
				{
					pixelTable[0] = get_pixel(img, i, k);
					pixelTable[1] = get_pixel(img, i, k);
					pixelTable[2] = get_pixel(img, i - 1, k);
					pixelTable[3] = get_pixel(img, i, k + 1);
					pixelTable[4] = get_pixel(img, i + 1, k);
					break;
				}
				if(k == h)
				{
					pixelTable[0] = get_pixel(img, i, k);
					pixelTable[1] = get_pixel(img, i, k - 1);
					pixelTable[2] = get_pixel(img, i - 1, k);
					pixelTable[3] = get_pixel(img, i, k);
					pixelTable[4] = get_pixel(img, i + 1, k);
					break;
				}
				else
				{
					pixelTable[0] = get_pixel(img, i, k);
					pixelTable[1] = get_pixel(img, i, k - 1);
					pixelTable[2] = get_pixel(img, i - 1, k);
					pixelTable[3] = get_pixel(img, i, k + 1);
					pixelTable[4] = get_pixel(img, i + 1, k);
					break;
				}
			}
			array_select_sort(pixelTable, 5);
			int med = pixelTable[2];
			put_pixel(img, i, j, med);
		}
	}
}

SDL_Surface* increaseChar(SDL_Surface *img)
{
	SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
			28,
			28,
			img->format->BitsPerPixel,0,0,0,0);
	SDL_SoftStretch(img, NULL, dest, NULL);
	return dest;
}
SDL_Surface* Resize(SDL_Surface *img)
{
	SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
			576,
			460,
			img->format->BitsPerPixel,0,0,0,0);
	SDL_SoftStretch(img, NULL, dest, NULL);
	return dest;
}
