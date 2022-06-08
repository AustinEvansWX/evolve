#pragma once

#include <evolver/genome.hpp>
#include <vector>

using namespace std;

struct Creature {
  float position = 0;
  float fitness = 0;
  vector<Gene> genome = {};
};
