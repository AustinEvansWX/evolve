#include "evolver/creature.hpp"
#include <Magick++.h>
#include <evolver/evolver.hpp>
#include <utils/random.hpp>

using namespace Magick;

int main(int argc, char **argv) {
  InitializeMagick(*argv);
  Evolver evolver = Evolver({200, 18, 100, 0.01, 8});

  evolver.AddInput([&evolver](Creature &creature) {
    return evolver.goal_ < creature.position ? 1.0f : 0.0f;
  });

  evolver.AddInput([&evolver](Creature &creature) {
    return evolver.goal_ > creature.position ? 1.0f : 0.0f;
  });

  evolver.AddOutput([](Creature &creature) {
    creature.position = min(1.0f, creature.position += 0.01);
  });

  evolver.AddOutput([](Creature &creature) {
    creature.position = max(0.0f, creature.position -= 0.01);
  });

  evolver.RunSimulation(50);
  return 0;
}
