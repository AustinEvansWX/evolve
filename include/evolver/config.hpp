#pragma once

struct EvolverConfig {
  unsigned int population_size;
  unsigned int genome_size;
  unsigned int generation_steps;
  float mutation_rate;
  unsigned int input_neurons;
  unsigned int internal_neurons;
  unsigned int output_neurons;
  float goal;
};
