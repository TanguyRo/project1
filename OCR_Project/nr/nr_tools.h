# ifndef NR_TOOLS_H
# define NR_TOOLS_H

# include "neural_network.h"
# include <stdlib.h>
# include <stdio.h>
# include <err.h>

//A random that returns a double in [-1; 1]
double Random();

//The classical sigmoid function
double Sigmoid(double x);

//The classical derivate of the sigmoid function
double SigmoidPrime(double x);

//Return the position of the output with the greatest sigmoid result
int get_pos(struct nr *network);

//Calculates the Squared error
void SquaredError(struct nr *network);

//Retrive the position of the char in the goal tab
int waited_position(double *waited_result);

//Retrive char from value val
char get_char(int val);

//Save data of the NN in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
void SaveData(struct nr *network);

//Extract data previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct nr* ExtractData ();

# endif
