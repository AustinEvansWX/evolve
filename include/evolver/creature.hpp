#pragma once

#include <evolver/genome.hpp>
#include <vector>

using namespace std;

struct Creature {
  float position;
  float fitness;
  vector<Gene> genome;
};
