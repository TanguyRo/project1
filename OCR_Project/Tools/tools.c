# include "tools.h"

/* 
====================================
===========Classic tools============
====================================
 */

/*==================================*/

static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y)
{
	int bpp = surf -> format -> BytesPerPixel;
	return (Uint8*)surf -> pixels + y * surf -> pitch + x * bpp;
}

/*==================================*/

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
	Uint8 *p = pixelref(surface, x, y);
	switch(surface -> format -> BytesPerPixel) {
		case 1:
			return *p;
		case 2:
			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
	}
	return 0;
}

/*==================================*/

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
	Uint8 *p = pixelref(surface, x, y);
	switch(surface -> format -> BytesPerPixel) {
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

/*=================================*/

void wait_for_keypressed()
{
	SDL_Event event;
	
	// Wait for a key to be down.
	do
	{
		SDL_PollEvent(&event);
	}while(event.type != SDL_KEYDOWN);

	// Wait for a key to be up.
	do
	{
		SDL_PollEvent(&event);
	}while(event.type != SDL_KEYUP);
}

/*==================================*/

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

/*==================================*/

SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;

	/*Set the window to the same size as the image*/
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if (screen == NULL)
	{
		/*error management*/
		errx(1, "Couldn't set %dx%d video mode: %s\n",
				img->w, img->h, SDL_GetError());
	}
	
	/* Blit into the screen surface*/
	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());
	
	/* Update the screen*/
	SDL_UpdateRect(screen, 0, 0, img->w, img->h);
	wait_for_keypressed();
	
	/* return the screen for further uses*/
	return screen;
}

/*==================================*/

/* Copy a SDL_Surface */
SDL_Surface* copy_image(SDL_Surface *img)
{
	int width = img->w;
	int height = img->h;

	Uint32 pixel;
	
	SDL_Surface* copy;
	copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
			width,
			height,
			img -> format -> BitsPerPixel,0,0,0,0);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			put_pixel(copy, x, y, pixel);
		}
	}
	return(copy);
}

/*==================================*/

/* 
====================================
====Array tools for mediumPixel=====
====================================
 */

/*==================================*/

/*Swap two elements in an array*/
void array_swap(int array[], size_t i, size_t j)
{
	int element1 = array[i];
	int element2 = array[j];
	array[i] = element2;
	array[j] = element1;
}

/*==================================*/

/*Find the minimum index*/
void array_select_sort(int array[], size_t len)
{
	size_t i = 0;
	size_t j;
	int min_index;
	while(i<len)
	{
		j= i;
		min_index = j;
		while(j<len)
		{
			if(array[j]<array[min_index])
			{
				min_index = j;
			}
			j+=1;
		}
		array_swap(array,i,min_index);
		i++;
	}
}

/*==================================*/

/*Create the .txt file with the values of pixels*/
void create_matrix_file(SDL_Surface *img, char *filename)
{
	int width = img->w;
	int height = img->h;
	
	Uint32 pixel;
	Uint8 r, g, b;

	strtok(filename,".");
	strcat(filename,".txt");
	FILE *file = fopen(filename,"w");
	printf("%s \n",filename);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
				fputs("0",file);
			else
				fputs("1",file);
		}
		fputs("\n",file);
	}
	fclose(file);
}

/*=================================*/

void print_matrix(double mat[], size_t lines, size_t cols)
{
	for(size_t y = 0; y < lines; y++)
	{
		for(size_t x = 0; x < cols; x++)
			printf("%d ", (int)mat[x + y * cols]);
		printf("\n");
	}
}

/*==================================*/

/*Create & return the double* pixels values from filename*/
double *matrix_from_file(char *filename)
{
	double *matrix = malloc(sizeof(double) * 28 * 28);
	FILE *file = fopen(filename,"r");

	if(file == NULL)
		printf("File is NULL \n");
	for(int i = 0; i <= 28; i++)
	{
		for(int j = 0; j <= 28; j++)
		{
			int c = fgetc(file);
			if(c == 49)
				matrix[j+i*28] = 1;
			if(c == 48)
				matrix[j+i*28] = 0;
		}
	}
	fclose(file);
	return matrix;
}

/*==================================*/

/*Create & return the double* goals pixels values from filename*/
double *waited_array(char letter)
{
	double *waited_result = calloc(52, sizeof(double));

	if(letter >= 'A' && letter <= 'Z')
		waited_result[(int)(letter) - 65] = 1;

	else if(letter >= 'a' && letter <= 'z')
		waited_result[((int)(letter) - 97) + 26] = 1;

	return waited_result;
}

/*==================================*/

/*Create & return all the goals matrixes (for all letters)*/
double **waited_matrix()
{
	double **waited_matrix = malloc(sizeof(double*) * 52);
	char maj = 'A';
	char min = 'a';
	for(int i = 0; i < 52; i++)
	{
		if(i < 26)
		{
			waited_matrix[i] = waited_array(maj);
			maj++;
		}
		else
		{
			waited_matrix[i] = waited_array(min);
			min++;
		}
	}
	return waited_matrix;
}

/*==================================*/

/*Create & return all the letters matrixes (for all letters)*/
double **letters_matrix()
{
	//Variables
	char majs_path[14] = "majs/0/00.txt\0";
	char mins_path[14] = "mins/0/00.txt\0";
	double **letters_matrix = malloc(sizeof(double *) * 52);
	char maj = 'A';
	char min = 'a';
	char count = '4';

	for(int i = 0; i < 52; i++)
	{

		if(i < 26 * 1)
		{
			majs_path[5] = maj;
			majs_path[7] = maj;
			majs_path[8] = count;
			//printf("%s\n",majs_path);
			letters_matrix[i] = matrix_from_file(majs_path);
			maj++;

		}
		else if(i >= 26*1)
		{
			count = '3';
			mins_path[5] = min;
			mins_path[7] = min;
			mins_path[8] = count;
			//printf("%s\n",mins_path);
			letters_matrix[i] = matrix_from_file(mins_path);
			min++;

		}
	}
	return letters_matrix;
}

/*==================================*/

double *create_matrix(SDL_Surface *img)
{
	double *letter_matrix = malloc(sizeof(double) * 28 * 28);

	int width = img->w;
	int height = img->h;

	Uint32 pixel;
	Uint8 r, g, b;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixel = get_pixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
				letter_matrix[x + y * width] = 0;
			else
				letter_matrix[x + y * width] = 1;
		}
	}
	return letter_matrix;
}

/*=================================*/

void append(char* s, char c)
{
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

/*=================================*/

char* concat(const char *s1, const char *s2)
{
	//+1 for the null-terminator
	char *result = malloc(strlen(s1) + strlen(s2) + 1);

	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}
