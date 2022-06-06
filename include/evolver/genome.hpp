#pragma once

struct Gene {
  float weight;
  unsigned int source_type;
  unsigned int source_id;
  unsigned int sink_id;
};

Gene RandomGene(int input_neurons, int internal_neurons, int output_neurons);
