#pragma once

struct Gene {
  float weight = 0;
  unsigned int layer = 0;
  unsigned int source = 0;
  unsigned int sink = 0;
};

Gene RandomGene(int input_neurons, int internal_neurons, int output_neurons);
