#include <utils/random.hpp>

struct Gene {
  float weight;
  unsigned int source_type;
  unsigned int source_id;
  unsigned int sink_id;
};

Gene RandomGene(int input_neurons, int internal_neurons, int output_neurons) {
  Gene gene = Gene{};

  gene.weight = rng::Range(-4.0f, 4.0f);
  gene.source_type = rng::Range(0, 1);

  switch (gene.source_type) {
  case 0:
    gene.source_id = rng::Range(0, input_neurons);
    gene.sink_id = rng::Range(0, internal_neurons);
    break;
  case 1:
    gene.source_id = rng::Range(0, internal_neurons);
    gene.sink_id = rng::Range(0, output_neurons);
    break;
  }

  return gene;
}
