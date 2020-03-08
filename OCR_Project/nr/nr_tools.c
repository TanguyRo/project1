# include "nr_tools.h"




// ===========================================================================
// ====================== MATHEMATICAL FUNCTIONS =============================
// ===========================================================================

//A random that returns a double in [-1; 1]
double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2) - 1;
}

//The classical sigmoid function
double Sigmoid(double x)
{
  return(1.0 / (1.0 + exp(-x)));
}

//The classical derivate of the sigmoid function
double SigmoidPrime(double x)
{
  return x * (1.0 - x);
}


// ==================== GET OUTPUT POSITION ===============

//Return the position of the output with the greatest sigmoid result
int get_pos(struct nr *network)
{
  double max = 0;
  int posMax = 0;

  for (int o = 0; o < network -> output_number; o++)
  {
    if (max < network ->output_output[o])
    {
      posMax = o;
      max = network -> output_output[o];
    }
  }
  return posMax;
}


// ========== GET ERROR ==================
//Calculates the Squared error
void SquaredError(struct nr *network)
{
  double max = 0;
  double sum = 0;

  for (int o = 0; o < network -> output_number; o++)
  {
    if (max < network -> output_output[o])
    {
      max = network -> output_output[o];
    }
    sum += (network -> waited_result[o] - network -> output_output[o]) *
                      (network -> waited_result[o] - network -> output_output[o]);
  }
  network -> error += 0.5 * sum;
}

// ============ GET CHARACTER WAITED POSITION DURING THE TEST ==========
//Retrive the position of the char in the goal tab
int waited_position(double *waited_result)
{
  int count = 0;

  while(waited_result[count] != 1.0)
    count++;

  return count;
}


// ==================== GET CHAR ==================
//Retrive char from value val
char get_char(int val)
{
  char c;

  if(val <= 25)
	  c = val + 65;
  
  else if(val > 25 && val <= 51)
  {
    c = (val + 97 - 26);
  }
  else if(val > 51 && val <= 61)
  {
    c = val + 48 - 52;
  }
  return c;
}




// ============================================================================
// =============================== DATA SAVING ================================
// ============================================================================

// save data in 4 files 

void save_weight(struct nr *network)
{
	FILE* input_hidden_weight = fopen("input_hidden_weights.w", "w");
  for(int i = 0; i < network -> input_number; ++i)
  {
    for(int h = 0; h < network -> hidden_number; ++h)
    {
      fprintf(input_hidden_weight, "%f\n", network -> input_hidden_weight[i][h]);
    }
  }
  fclose(input_hidden_weight);

  FILE* hidden_output_weight = fopen("hidden_output_weights.w", "w");
  for(int h = 0; h < network -> hidden_number; ++h)
  {
    for(int o = 0; o < network -> output_number; ++o)
    {
        fprintf(hidden_output_weight, "%f\n", network -> hidden_output_weight[h][o]);
    }
  }
  fclose(hidden_output_weight);
}

void save_bias(struct nr *network)
{
	FILE* hidden_bias = fopen("hidden_bias.b", "w");
  for(int h = 0; h < network -> hidden_number; ++h)
  {
    fprintf(hidden_bias, "%f\n", network -> hidden_bias[h]);
  }
  fclose(hidden_bias);

  FILE* output_bias = fopen("output_bias.b", "w");
  for (int o = 0; o < network -> output_number; ++o)
  {
    fprintf(output_bias, "%f\n", network -> output_bias[o]);
  }
  fclose(output_bias);
}


void SaveData(struct nr *network)
{
	save_weight(network);
  	save_bias(network);	
}


// ================================================================================
// ============================= DATA EXTRACT =====================================
// ================================================================================

void extract_weight(struct nr *network, int sizeMax, char *line)
{
	//input_hidden_weight
  FILE* input_hidden_weight = fopen("input_hidden_weights.w", "r");
  for(int i = 0; i < network -> input_number; ++i)
  {
    for(int h = 0; h < network -> hidden_number; ++h)
    {
      fgets(line, sizeMax, input_hidden_weight);
      strtok(line, "\n");
      network -> input_hidden_weight[i][h] = atof(line);
    }
  }
  fclose(input_hidden_weight);

  //hidden_output_weight
  FILE* hidden_output_weight = fopen("hidden_output_weights.w", "r");
  for(int h = 0; h < network -> hidden_number; ++h)
  {
    for(int o = 0; o < network -> output_number; ++o)
    {
        fgets(line, sizeMax, hidden_output_weight);
        strtok(line, "\n");
        network -> hidden_output_weight[h][o] = atof(line);
    }
  }
  fclose(hidden_output_weight);
}
void extract_bias(struct nr *network, int sizeMax, char *line)
{
	//hidden_bias
  FILE* hidden_bias = fopen("hidden_bias.b", "r");
  for(int h = 0; h < network -> hidden_number; ++h)
  {
    fgets(line, sizeMax, hidden_bias);
    strtok(line, "\n");
    network -> hidden_bias[h] = atof(line);
  }
  fclose(hidden_bias);

  //output_bias
  FILE* output_bias = fopen("output_bias.b", "r");
  for (int o = 0; o < network -> output_number; ++o)
  {
    fgets(line, sizeMax, output_bias);
    strtok(line, "\n");
    network -> output_bias[o] = atof(line);
  }
  fclose(output_bias);
}


//Extract data previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct nr* ExtractData ()
{
  //CREATE Neural Network
  struct nr *network = malloc(sizeof(struct nr));
  network -> input_number = 28*28; //size of imgs
  network -> hidden_number = 20;
  network -> output_number = 52; //26*2 letters
  network -> str = malloc(sizeof(char)*1200);
  network -> str = "\0";

  int sizeMax = 15;
  char *line = calloc(15, sizeof(char));

  
  extract_weight(network, sizeMax, line);
  extract_bias(network, sizeMax, line);

  return network;
}
