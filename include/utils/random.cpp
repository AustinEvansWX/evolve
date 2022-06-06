#include <bits/chrono.h>
#include <chrono>
#include <cstdint>
#include <random>
#include <utils/random.hpp>

namespace rng {

void PlantSeed() {
  srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

float Range(float min, float max) {
  float random = ((float)std::rand()) / (float)RAND_MAX;
  return (max - min) * random + min;
}

int Range(int min, int max) {
  float random = ((float)std::rand()) / (float)RAND_MAX;
  return (max + 1 - min) * random + min;
}

} // namespace rng
