#include "nr.h"

struct nr
{
	int input_number;
	int hidden_number;
	int output_number;
	//Arrays
	double *input_value;
	double *waited_result;
	//Weight Arrays
	double *input_hidden_weight;
	double *hidden_output_weight;
	//Bias Array
	double *hidden_bias;
	//Bias output
	double output_bias;
	//Output Hidden
	double *hidden_output;
	//Output of output
	double output_output;
	//Array of delta bias for hidden
	double *hidden_delta_bias;
	//delta bias for output
	double output_delta_bias;
	//delta weight
	double *input_hidden_delta_weight;
	double *hidden_output_delta_weight;
	//delta of output
	double output_delta;
	//delta of hidden
	double *hidden_delta;
	double error;
	double eta;
	double alpha;
};

// ====================== MATHEMATICAL FUNCTIONS =====================

static double sigmoid(double x)
{
	return(1.0/(1.0+exp(-x)));
}

static double Random()
{
	return (double)rand()/(double)RAND_MAX;
}



// =========================================================
// =========== NETWORK INITIALIZATION ======================
// =========================================================
struct nr nr_init2()
{
	struct nr network;
	network.input_number = 2;
	network.hidden_number = 3;
	network.input_value = malloc(sizeof(double)*4*2);
	network.waited_result = malloc(sizeof(double)*4);
	network.input_hidden_weight = malloc(sizeof(double)*network.input_number*network.hidden_number);
	network.hidden_output_weight = malloc(sizeof(double)*network.hidden_number);
	network.hidden_bias = malloc(sizeof(double)*network.hidden_number);
	network.output_bias = Random();
	network.hidden_output = malloc(sizeof(double)*network.hidden_number);
	network.output_output = 0;
	network.hidden_delta_bias = malloc(sizeof(double)*network.hidden_number);
	network.output_delta_bias = 0.0;
	network.input_hidden_delta_weight = malloc(sizeof(double)*network.input_number*network.hidden_number);
	network.hidden_output_delta_weight = malloc(sizeof(double)*network.hidden_number);
	network.output_delta = 0.0;  
	network.hidden_delta = malloc(sizeof(double)*network.hidden_number);
	network.error = 0.0;
	network.eta = 0.5;
	network.alpha = 0.9;
	return(network);

}



void value_init(struct nr *network)
{

	*((*network).input_value) = 0;
	*((*network).input_value +1) = 0;
	*((*network).input_value +2) = 0;
	*((*network).input_value +3) = 1;
	*((*network).input_value +4) = 1;
	*((*network).input_value +5) = 0;
	*((*network).input_value +6) = 1;
	*((*network).input_value +7) = 1;



	*((*network).waited_result) = 0;
	*((*network).waited_result +1) = 1;
	*((*network).waited_result +2) = 1;
	*((*network).waited_result +3) = 0;


	for (int i = 0;i< (*network).input_number; i++)
	{
		for (int h = 0; h < (*network).hidden_number; ++h)
		{
			*((*network).input_hidden_weight + (h + i * (*network).hidden_number)) = Random();
			*((*network).input_hidden_delta_weight + (h + i * (*network).hidden_number)) = 0.0;
		}
	}

	for (int h = 0; h < (*network).hidden_number; ++h)
	{
		*((*network).hidden_output_weight + h) = Random();
		*((*network).hidden_output_delta_weight + h) = 0.0;
		*((*network).hidden_bias + h) = Random();
		*((*network).hidden_delta_bias + h) = 0.0;
	}
	(*network).output_delta_bias = 0.0;
}




// ====================== FORWARD PROPAGATION =========================

void Forward_propagation(struct nr *network, int p,int epoch)
{
	for (int h = 0; h <  (*network).hidden_number; ++h)
	{
		double SumIH = 0.0;
		for (int i = 0; i < (*network).input_number; ++i)
		{
			SumIH += *((*network).input_hidden_weight+(h+i*(*network).hidden_number)) * 
				*((*network).input_value+(i+p*(*network).input_number));
		}
		*((*network).hidden_output +h) = sigmoid(SumIH +  *((*network).hidden_bias + h));
	}
	double SumHO = 0;
	for (int h = 0; h < (*network).hidden_number; ++h)
	{
		SumHO += *((*network).hidden_output_weight + h) * *((*network).hidden_output +h);
	}
	(*network).output_output = sigmoid(SumHO + (*network).output_bias);

	//Squared error function
	(*network).error += 0.5 * ( *((*network).waited_result+p) - (*network).output_output) * 
		(*((*network).waited_result + p) - (*network).output_output);
	if (epoch % 100 == 0)
	{
		printf("test n°: %d\n		first value: %f -- second value: %f\n ===> output value: %f\n\n\n"
				, p 
				,*((*network).input_value + p*2 )
				,*((*network).input_value + p*2 +1)
				, (*network).output_output);
	}
}



// ========================== BACKWARD PROPAGATION ==================================
void Backward_propagation(struct nr *network,int p)
{
	//== BACKPROPAGATION ==//
	(*network).output_delta = (*((*network).waited_result+p) - (*network).output_output) *
		(*network).output_output * (1.0 - (*network).output_output);
	for (int h = 0; h < (*network).hidden_number; ++h)
	{
		double dSumOutput = *((*network).hidden_output_weight + h) * (*network).output_delta ;
		*((*network).hidden_delta +h) = dSumOutput * *((*network).hidden_output +h) * 
			(1.0 - *((*network).hidden_output +h));
	}
	//Update weights & bias between Input and Hidden layers
	for (int h = 0; h < (*network).hidden_number; ++h)
	{
		//Update BiasH
		*((*network).hidden_delta_bias + h) = (*network).eta *  *((*network).hidden_delta +h);
		*((*network).hidden_bias +h) += *((*network).hidden_delta_bias +h) ;
		for (int i = 0; i < (*network).input_number; ++i)
		{
			//Update WeightIH
			*((*network).input_hidden_delta_weight + (h+i*(*network).hidden_number)) = (*network).eta * 
				*((*network).input_value + 
						(i+p*(*network).input_number)) *
				*((*network).hidden_delta +h) + 
				(*network).alpha * 
				*((*network).input_hidden_delta_weight + 
						(h+i*(*network).hidden_number));

			*((*network).input_hidden_weight + (h+i*(*network).hidden_number)) += *((*network).input_hidden_delta_weight +
					(h+i*(*network).hidden_number));
		}
	}
	//Update weights & bias between Hidden and Ouput layers
	//Update BiasO
	(*network).output_delta_bias = (*network).eta * (*network).output_delta;
	(*network).output_bias  += (*network).output_delta_bias ;
	for (int h = 0; h < (*network).hidden_number; ++h)
	{
		//Update WeightsHO
		*((*network).hidden_output_delta_weight + h) = (*network).eta * *((*network).hidden_output + h) *
			(*network).output_delta + (*network).alpha *
			*((*network).hidden_output_delta_weight + h);
		*((*network).hidden_output_weight + h) += *((*network).hidden_output_delta_weight + h) ;
	}
}



// ================================== PRINCIPAL FUNCTION ================================
// ~main

void main_XOR()
{
	srand(time(NULL));

	int NbPattern = 4;
	int NbEpoch = 10000;

	struct nr net_1 = nr_init2();
	struct nr *network = &net_1;

	value_init(network);

	for (int epoch = 0; epoch <= NbEpoch; ++epoch)
	{
		(*network).error = 0.0;
		for (int p = 0; p < NbPattern; ++p)
		{
			Forward_propagation(network,p,epoch);
			Backward_propagation(network,p);
		}
		//== PRINT ==//
		if (epoch % 100 == 0)
		{
			printf("               epoch %-5d -- error = %f\n", epoch, (*network).error);
		}
	}
}
