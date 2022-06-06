#include <Magick++.h>
#include <evolver/evolver.hpp>
#include <utils/random.hpp>

using namespace Magick;

int main(int argc, char **argv) {
  InitializeMagick(*argv);
  Evolver evolver = Evolver({200, 32, 100, 0.00, 2, 1, 2});
  evolver.RunSimulation(50);
  return 0;
}
