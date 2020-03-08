# ifndef GTK_H
# define GTK_H

# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "../nr/nr_tools.h"
# include "../character_detection/character_detection.h"
# include "../Tools/tools.h"
# include "../ImageTreatment/image_operations.h"
# include "../ImageTreatment/segmentation.h"


typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

#define UNUSED(x) (void)(x)

void save(GtkButton *button, GtkTextBuffer *buffer);

void load(GtkButton *button, GtkImage *image);

int trainNN();

void openFile(GtkButton *button, GtkLabel *text_label);

int launch(GtkButton *button, GtkTextBuffer *buffer);

void create(int argc, char *argv[]);

# endif
