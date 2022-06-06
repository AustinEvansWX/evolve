#include <evolver/genome.hpp>
#include <utils/random.hpp>

Gene RandomGene(int input_neurons, int internal_neurons, int output_neurons) {
  Gene gene = Gene{};

  gene.weight = rng::Range(-4.0f, 4.0f);
  gene.layer = rng::Range(0, 1);

  if (gene.layer == 0) {
    gene.source = rng::Range(0, input_neurons);
    gene.sink = rng::Range(0, internal_neurons);
  } else {
    gene.source = rng::Range(0, internal_neurons);
    gene.sink = rng::Range(0, output_neurons);
  }

  return gene;
}
