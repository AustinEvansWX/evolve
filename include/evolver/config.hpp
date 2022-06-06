#pragma once

struct EvolverConfig {
  unsigned int population_size;
  unsigned int genome_size;
  unsigned int generation_steps;
  float mutation_rate;
  unsigned int internal_neurons;

  EvolverConfig(int population_size, int genome_size, int generation_steps, float mutation_rate, unsigned int internal_neurons) {
    this->population_size = population_size;
    this->genome_size = genome_size;
    this->generation_steps = generation_steps;
    this->mutation_rate = mutation_rate;
    this->internal_neurons = internal_neurons;
  };
};
