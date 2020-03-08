#Makefile
#Authors : antoine.montes

CC=gcc -rdynamic

CPPFLAGS =  `pkg-config gtk+-3.0 --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =
LDLIBS= `pkg-config gtk+-3.0 --libs sdl` -lSDL_image -lm

SRC = main.c ImageTreatment/image_operations.c Tools/tools.c ImageTreatment/segmentation.c GTK/gtk.c nr/nr.c	nr/neural_network.c nr/nr_tools.c character_detection/character_detection.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)

clean:
		$(RM) $(OBJ) $(OBJ_TESTS) $(DEP) $(DEP_TESTS) main && clear && rm -f *.w && rm -f *.b && rm -f image_resize
