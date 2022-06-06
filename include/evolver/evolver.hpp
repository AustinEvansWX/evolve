#include <evolver/config.hpp>
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
  EvolverConfig config_;
  vector<Creature> creatures_;
  float goal_;

  Evolver(EvolverConfig config);

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
