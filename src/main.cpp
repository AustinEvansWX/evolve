#include <Magick++.h>
#include <evolver/evolver.hpp>
#include <utils/random.hpp>

using namespace Magick;

int main(int argc, char **argv) {
  InitializeMagick(*argv);
  Evolver evolver = Evolver({200, 6, 100, 0.02, 2, 1, 2});
  evolver.RunSimulation(50);
  return 0;
}
