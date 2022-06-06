#pragma once

#include <evolver/creature.hpp>
#include <evolver/genome.hpp>
#include <functional>
#include <utils/random.hpp>

#include <Magick++.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace Magick;

class Evolver {
public:
  unsigned int population_size_;
  unsigned int genome_size_;
  unsigned int generation_steps_;
  float mutation_rate_;
  unsigned int internal_count_;
  vector<Creature> creatures_;
  float goal_;

  Evolver(int population_size, int genome_size, int generation_steps, float mutation_rate, unsigned int internal_count);

  int input_count_ = 0;
  int output_count_ = 0;
  vector<function<float(Creature &creature)>> input_funcs_;
  vector<function<void(Creature &creature)>> output_funcs_;

  void AddInput(function<float(Creature &creature)> input_func);
  void AddOutput(function<void(Creature &creature)> output_func);

  void RunSimulation(int generations);

private:
  void SetRandomGoal();

  void SpawnCreatures();

  void RunGeneration();

  vector<float> GetInputs(Creature &creature);

  float AverageFitness();

  void RankCreatures();

  void Reproduce();

  vector<Image> GenerateFrames(vector<float> fitness_scores);
};
