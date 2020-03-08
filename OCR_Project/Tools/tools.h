/* tools.h*/

# ifndef TOOLS_H
# define TOOLS_H

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void wait_for_keypressed();

void update_surface(SDL_Surface* screen, SDL_Surface* image);

SDL_Surface* display_image(SDL_Surface *img);

SDL_Surface* copy_image(SDL_Surface *img);

void array_swap(int array[], size_t i, size_t j);

void array_select_sort(int array[], size_t len);

void create_matrix_file(SDL_Surface *img, char *filename);

void print_matrix(double mat[], size_t lines, size_t cols);

double* matrix_from_file(char *filename);

double *waited_array(char letter);

double **waited_matrix();

double **letters_matrix();

double *create_matrix(SDL_Surface *img);

void append(char* s, char c);

char* concat(const char *s1, const char *s2);

# endif
