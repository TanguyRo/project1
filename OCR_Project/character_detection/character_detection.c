#include "character_detection.h"

//forward progation
static void ForwardPass(struct nr *network)
{
	double s, weight, output, bias;

	//Calculate the Output of hidden neurons
	for (int y = 0; y < network->hidden_number; y++)
	{
		s = 0.0;
		for (int i = 0; i < network -> input_number; i++)
		{
			weight = network -> input_hidden_weight[i][y];
			output = network -> input_output[i];
			s += weight * output;
		}
		bias = network -> hidden_bias[y];
		network -> hidden_output[y] = Sigmoid(s + bias);
	}

	//Calculate the output of output neurons
	for (int i = 0; i < network -> output_number; i++)
	{
		s = 0.0;
		for (int y = 0; y < network -> hidden_number; y++)
		{
			weight = network -> hidden_output_weight[y][i];
			output = network -> hidden_output[y];
			s += weight * output;
		}
		bias = network -> output_bias[i];
		network -> output_output[i] = Sigmoid(s + bias);
	}
}

char DetectText(struct nr *network, double* letter)
{
	for(int l = 0; l < network->input_number; l++)
		network->input_output[l] = letter[l];

	ForwardPass(network);

	//Retrieve the character detected
	int pos = get_pos(network);
	char c = get_char(pos);

	return c;
}
