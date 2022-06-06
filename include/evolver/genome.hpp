#pragma once

struct Gene {
  float weight;
  unsigned int layer;
  unsigned int source;
  unsigned int sink;
};

Gene RandomGene(int input_neurons, int internal_neurons, int output_neurons);
