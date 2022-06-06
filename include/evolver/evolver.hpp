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
  int input_count_ = 0;
  vector<function<float(Creature creature)>> input_funcs_;

  Evolver(EvolverConfig config);

  void AddInput(function<float(Creature creature)> input_func);

  void RunSimulation(int generations);

private:
  void SetRandomGoal();

  void SpawnCreatures();

  void RunGeneration();

  vector<float> GetInputs(Creature creature);

  float AverageFitness();

  void RankCreatures();

  void Reproduce();

  float Input1(Creature creature);

  float Input2(Creature creature);

  vector<Image> GenerateFrames(vector<float> fitness_scores);
};
