# ifndef NEURAL_NETWORK_H
# define NEURAL_NETWORK_H

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <time.h>
# include <math.h>
# include "../Tools/tools.h"


struct nr
{
    int input_number;
    int hidden_number;
    int output_number;

    //Arrays
    double input_output[28*28];
    double waited_result[52];
    //Weight Arrays
    double input_hidden_weight[28*28][20];
    double hidden_output_weight[20][52];
    //Bias Array
    double hidden_bias[20];
    //Bias output
    double output_bias[52];
    //Output Hidden
    double hidden_output[20];
    //Output of output
    double output_output[52];

    //delta weight
    double input_hidden_delta_weight[28*28][20];
    double hidden_output_delta_weight[20][52];
    //delta of output
    double output_delta[52];
    //delta of hidden
    double hidden_delta[20];

    double error_maximal;
    double error;
    double eta;
    double alpha;

    char *str;
};


#include "nr_tools.h"


struct nr* nr_init1();

void PrintState(struct nr *network);

void nr_ocr(struct nr *network, double *input, double *waited_result);

# endif
