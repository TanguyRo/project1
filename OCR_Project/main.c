#include "main.h"



void help()
{
    printf("This is CATA Team project \nTo understand how this OCR works you can use the following commands : \n");
    printf("   --help  To see this page\n");
    printf("   --XOR  To see the XOR function results\n");
    printf("   --OCR  To show the OCR network training\n");
    printf("   if no argument:  Launch the project\n");
}

int main(int argc, char *argv[])
{
    
    if(argc>1)
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            help();
        }
        if(strcmp(argv[1], "--XOR") == 0)
        {
            main_XOR();
        }
        if(strcmp(argv[1], "--OCR") == 0)
        {
            trainNN();
        }
    }
    else
    {
      //create interface with GTK
      create(argc,argv);
    }
    return(EXIT_SUCCESS);
}
