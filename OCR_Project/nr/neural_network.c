# include "neural_network.h"

// ================================================================
// ================== NETWORK INITIALIZATION ======================
// ================================================================

void input_hidden_set_value(struct nr *network)
{
	for (int i = 0; i < network -> input_number; i++)
	{
		for (int h = 0; h < network -> hidden_number; h++)
		{
			network -> input_hidden_weight[i][h] = Random();
			network -> input_hidden_delta_weight[i][h] = 0.0;
		}
	}
}

void hidden_output_set_value(struct nr *network)
{

	for (int h = 0; h < network -> hidden_number; h++)
	{
		for(int o = 0; o < network -> output_number; o++)
		{
			network -> hidden_output_weight[h][o] = Random();
			network -> hidden_output_delta_weight[h][o] = 0.0;
		}
		network -> hidden_bias[h] = Random();
	}

}

void output_bias_set_value(struct nr *network)
{
	for (int o = 0; o < network -> output_number; o++)
	{
		network -> output_bias[o] = Random();
		network -> output_delta[o] = 0.0;
	}

}


struct nr* nr_init1()
{
	struct nr *network = NULL;
	network = malloc(sizeof(struct nr));
	network -> input_number = 28*28;
	network -> hidden_number = 20;
	network -> output_number = 52;
	network -> error = 0.0;
	network -> error_maximal = 0.0;
	network -> eta = 0.5;
	network -> alpha = 0.9;
	network -> str = "";

	//Set Values

	input_hidden_set_value(network);
	hidden_output_set_value(network);
	output_bias_set_value(network);
	return network;
}



// ===============================================================
// ===================== FORWARD PROPAGATION =====================
// ===============================================================


void output_hidden(struct nr *network)
{
	double sum;
	double weight;
	double output;
	double bias;

	for (int h = 0; h < network -> hidden_number; h++)
	{
		sum = 0.0;
		for (int i = 0; i < network -> input_number; i++)
		{
			weight = network -> input_hidden_weight[i][h];
			output = network -> input_output[i];

			sum += weight * output;
		}
		bias = network -> hidden_bias[h];
		network -> hidden_output[h] = Sigmoid(sum + bias);
	}

}

void output_output(struct nr *network)
{
	double sum;
	double weight;
	double output;
	double bias;
	for (int o = 0; o < network -> output_number; o++)
	{
		sum = 0.0;
		for (int h = 0; h < network -> hidden_number; h++)
		{
			weight = network -> hidden_output_weight[h][o];
			output = network -> hidden_output[h];

			sum += weight * output;
		}
		bias = network -> output_bias[o];
		network -> output_output[o] = Sigmoid(sum + bias);
	}

}

//Here, the fonction that call the two function for update weights
static void Forward_propagation(struct nr *network)
{
	output_hidden(network);
	output_output(network);
}


// =================================================================
// ===================== BACKWARD PROPAGATION ======================
// =================================================================


void cost_output(struct nr *network)
{
	double output;
	double derivate;
	double waited_result;
	double error;
	for (int o = 0; o < network -> output_number; o++)
	{
		output = network -> output_output[o];
		derivate = SigmoidPrime(output);
		waited_result = network -> waited_result[o];
		error = (waited_result - output) * derivate;

		network -> output_delta[o] = error;
	}

}

void cost_hidden(struct nr *network)
{
	double sum;
	double weight;
	double delta;
	double derivate;
	double output;
	for (int h = 0; h < network -> hidden_number; h++)
	{
		sum = 0.0;
		for (int o = 0; o < network -> output_number; o++)
		{
			weight = network -> hidden_output_weight[h][o];
			delta = network -> output_delta[o];
			sum += weight * delta;
		}
		output = network -> hidden_output[h];
		derivate = SigmoidPrime(output);
		network -> hidden_delta[h] = sum * derivate;
	}
}


static void Backward_propagation(struct nr *network)
{
	//Calulate Cost function for output neurons
	///Calculate Cost function for hidden neurons

	cost_output(network);
	cost_hidden(network);
}



// ================================================================
// ========================== WEIGHTS UPDATE ======================
// ================================================================

//Weights between Input and Hidden layers
void weight_input_hidden(struct nr *network)
{
	double eta;
	double alpha;
	double error;
	double output;
	double weight_delta;
	eta = network -> eta;
	alpha = network -> alpha;

	
	for (int h = 0; h < network -> hidden_number; h++)
	{
		for(int i = 0; i < network -> input_number; i++)
		{
			output = network -> input_output[i];
			error = network -> hidden_delta[h];
			weight_delta = network -> input_hidden_delta_weight[i][h];

			network -> input_hidden_weight[i][h] += eta * error * output +
				alpha * weight_delta;
			network -> input_hidden_delta_weight[i][h] = eta * error * output;
		}
	}
}

//Weights between Input and Hidden layers
void weight_hidden_output(struct nr *network)
{
	double eta;
	double alpha;
	double error;
	double output;
	double weight_delta;
	eta = network -> eta;
	alpha = network -> alpha;

	for (int o = 0; o < network -> output_number; o++)
	{
		for (int h = 0; h < network -> hidden_number; h++)
		{
			output = network -> hidden_output[h];
			error = network -> output_delta[o];
			weight_delta = network -> hidden_output_delta_weight[h][o];

			network -> hidden_output_weight[h][o] += eta * error * output +
				alpha * weight_delta;
			network -> hidden_output_delta_weight[h][o] = eta * error * output;
		}
	}
}



static void UpdateWeights(struct nr *network)
{
	weight_input_hidden(network);
	weight_hidden_output(network);
}


// =====================================================================
// ======================= BIAS UPDATE =================================
// =====================================================================

void update_hidden_bias(struct nr *network)
{
	double eta;
	double bias_delta;
	double delta;

	eta = network -> eta;
	// update hidden bias
	for (int h = 0; h < network -> hidden_number; h++)
	{
		delta = network -> hidden_delta[h];
		bias_delta = eta * delta;

		network -> hidden_bias[h] += bias_delta;
	}

}


void update_output_bias(struct nr *network)
{
	double eta;
	double bias_delta;
	double delta;
	eta = network -> eta;
	for (int o = 0; o < network -> output_number; o++)
	{
		delta = network -> output_delta[o];
		bias_delta = eta * delta;

		network -> output_bias[o] += bias_delta;
	}

}


static void UpdateBiases(struct nr *network)
{
	update_hidden_bias(network);
	update_output_bias(network);
}

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"




// =====================================================================
// ==================== PRINT STATUS ===================================
// =====================================================================

void PrintState(struct nr *network)
{
	//Squared error function
	SquaredError(network);
	int output = get_pos(network);

	//Retrive the chars : wanted & found
	char waited_char = get_char(waited_position(network -> waited_result));
	char recognizedChar = get_char(output);

	if(network -> error > network -> error_maximal)
		network -> error_maximal = network -> error;

	//Print the progress

	printf("character: %c ===> result: %c ------------------------- error: %f\n",
			waited_char,recognizedChar,network -> error);
	printf("%s",KWHT);

	if(output == waited_position(network -> waited_result))
		printf("%swork\n\n",KGRN);
	else
		printf("%sdoesn't work\n\n",KRED);
}

// ==================================================================================
// ======================== PRINCIPAL FUNCTION ======================================
// ==================================================================================


// LEARNING -> NEURAL NETWORK TRAINING

void nr_ocr(struct nr *network, double *input, double *waited_result)
{
	//Initialise Goals & InputValues for this char
	for (int g = 0; g < 52; g++)
		network -> waited_result[g] = waited_result[g];

	for(int i = 0; i < network -> input_number; i++)
		network -> input_output[i] = input[i];

	//Run the learning
	network -> error = 0;
	Forward_propagation(network);
	Backward_propagation(network);
	UpdateWeights(network);
	UpdateBiases(network);
}
