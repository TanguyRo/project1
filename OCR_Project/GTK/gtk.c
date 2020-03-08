# include "gtk.h"


gchar *filename = "";
char *text = "";
GtkWidget *parent;
void save(GtkButton *button, GtkTextBuffer *buffer)
{
	UNUSED(button);
	UNUSED(buffer);
	GtkWidget *dialog;
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
	dialog = gtk_file_chooser_dialog_new ("Save Text ",
			GTK_WINDOW (toplevel),
			GTK_FILE_CHOOSER_ACTION_SAVE,
			"Cancel", GTK_RESPONSE_CANCEL,
			"Save", GTK_RESPONSE_ACCEPT,
			NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		/* set the contents of the file to the text from the buffer */
		if (filename != NULL)
			g_file_set_contents (filename, text, strlen(text), NULL);
		else
			g_file_set_contents (filename, text, strlen(text), NULL);
	}
	gtk_widget_destroy (dialog);

}

void load(GtkButton *button, GtkImage *image)
{
	if(strcmp(filename,"") == 0)
		return;
	UNUSED(button);
	SDL_Surface *img = IMG_Load((char *)filename);
	if(img->w > 576 && img->h > 460)
	{

		SDL_Surface *new = Resize(img);

		SDL_SaveBMP(new,"image_resize");
		gtk_image_set_from_file (GTK_IMAGE (image), "image_resize");
	}
	else
		gtk_image_set_from_file (GTK_IMAGE (image), filename);
}

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

int trainNN()
{
	//Variables
	int epoch_number = 5000;
	int letters_number = 26 * 1 + 26 * 1; //5 fonts for uppers & 4 for lowers
	int currentChar = 0;
	//int count = 0;
	srand(time(NULL));

	//Intialize network
	struct nr *network = nr_init1();

	//Initialize all goals & letters
	double **waited_result = waited_matrix();
	double **letters = letters_matrix();

	for (int epoch = 0; epoch < epoch_number; epoch++)
	{
		currentChar = 0;
		for (int l = 0; l < letters_number; l++)
		{

			nr_ocr(network, letters[l],
					waited_result[currentChar]);
			currentChar++;

			if (epoch % 100 == 0)
			{
				PrintState(network);
			}
		}
		if (epoch % 100 == 0)
		{
			printf("number of iteration %-5d | error maximal = %s %f \n",
					epoch,KGRN,network->error_maximal);
			printf("%s",KWHT);
		}
		network -> error_maximal = 0.0;
	}
	SaveData(network);
	printf("finish, you can open an image\n");
	return EXIT_SUCCESS;
}


void openFile(GtkButton *button, GtkLabel *text_label)
{
	GtkWidget *label = (GtkWidget *) text_label;
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
	//GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
			GTK_WINDOW (toplevel),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"Open", GTK_RESPONSE_ACCEPT,
			"Cancel", GTK_RESPONSE_CANCEL,
			NULL);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
			{
				filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
				gtk_label_set_text(GTK_LABEL(label),filename);
				break;
			}
		default:
			break;
	}
	gtk_widget_destroy(dialog);
}

int launch(GtkButton *button, GtkTextBuffer *buffer)
{
	if(strcmp(filename,"") == 0)
	{
		return 1;
	}
	UNUSED(button);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *img = IMG_Load((char *)filename);

	greyscale(img);
		
	blacknwhite(img);
	

	SDL_Surface *image_cut = lineCut(img);
	struct nr *network = ExtractData();
	isolateLine(image_cut,network);
	
	gtk_text_buffer_set_text (buffer,network->str,strlen(network->str));
	text = network->str;
	
	SDL_Quit();
	return EXIT_SUCCESS;
}

void create(int argc, char *argv[])
{
	//Init variables
	GtkWidget *main_window;
	SGlobalData data;
	//Init GTK
	gtk_init(&argc, &argv);
	//Build from .glade
	data.builder = gtk_builder_new();
	gtk_builder_add_from_file(data.builder, "main.glade", NULL);
	//Get main_window
	main_window =  GTK_WIDGET(gtk_builder_get_object(data.builder,"main_window"));
	parent = main_window;
	//Connect signals
	gtk_builder_connect_signals(data.builder, &data);

	gtk_window_set_title(GTK_WINDOW(main_window), "CATA");
	gtk_widget_show_all(main_window);
	gtk_main();
}
