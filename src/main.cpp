#include "evolver/creature.hpp"
#include <Magick++.h>
#include <evolver/evolver.hpp>
#include <utils/random.hpp>

using namespace Magick;

int main(int argc, char **argv) {
  InitializeMagick(*argv);
  Evolver evolver = Evolver({200, 6, 100, 0.02, 4, 2});

  evolver.AddInput([&evolver](Creature creature) {
    return evolver.goal_ < creature.position ? 1.0f : 0.0f;
  });

  evolver.AddInput([&evolver](Creature creature) {
    return evolver.goal_ > creature.position ? 1.0f : 0.0f;
  });

  evolver.RunSimulation(50);
  return 0;
}
