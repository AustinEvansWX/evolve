#include <Magick++.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <utils/random.hpp>
#include <vector>

using namespace std;
using namespace Magick;

struct EvolverConfig {
  uint population_size;
  uint genome_size;
  uint generation_steps;
  float mutation_rate;
  uint input_neurons;
  uint internal_neurons;
  uint output_neurons;
  float goal;
};

struct Gene {
  float weight;
  uint source_type;
  uint source_id;
  uint sink_id;
};

struct Creature {
  float position;
  float fitness;
  vector<Gene> genome;
};

bool CompareCreatures(Creature c1, Creature c2) {
  return c1.fitness > c2.fitness;
}

class Evolver {
public:
  EvolverConfig config_;
  vector<Creature> creatures_;

  Evolver(EvolverConfig config) { config_ = config; }

  void SpawnCreatures() {
    for (int i = 0; i < config_.population_size; i++) {
      float position = rng::Range(0.0f, 1.0f);
      Creature creature = Creature{position};

      for (int j = 0; j < config_.genome_size; j++) {
        Gene gene = RandomGene();
        creature.genome.push_back(gene);
      }

      creatures_.push_back(creature);
    }
  }

  void RunGeneration() {
    for (int i = 0; i < config_.generation_steps; i++) {
      for (auto &creature : creatures_) {
        vector<float> inputs = {Input1(creature), Input2(creature)};

        vector<float> internal(config_.internal_neurons);

        for (auto &gene : creature.genome) {
          if (gene.source_type != 0) {
            continue;
          }

          internal[gene.sink_id] += inputs[gene.source_id] * gene.weight;
        }

        for (int j = 0; j < config_.internal_neurons; j++) {
          internal[j] = tanh(internal[j]);
        }

        vector<float> output(config_.output_neurons);

        for (auto &gene : creature.genome) {
          if (gene.source_type != 1) {
            continue;
          }

          output[gene.sink_id] += internal[gene.source_id] * gene.weight;
        }

        for (int j = 0; j < config_.output_neurons; j++) {
          output[j] = tanh(output[j]);

          float value = output[j];

          if (value <= 0) {
            continue;
          }

          if (rng::Range(0.0f, 1.0f) <= value) {
            switch (j) {
            case 0:
              creature.position = max(0.0f, creature.position -= 0.01);
              break;
            case 1:
              creature.position = min(1.0f, creature.position += 0.01);
              break;
            }
          }
        }

        creature.fitness = 1 - abs(creature.position - config_.goal);
      }
    }
  }

  float AverageFitness() {
    float sum = 0;

    for (int i = 0; i < config_.population_size; i++) {
      sum += creatures_[i].fitness;
    }

    return sum / config_.population_size;
  }

  void RankCreatures() {
    sort(creatures_.begin(), creatures_.end(), CompareCreatures);
  }

  void Reproduce() {
    vector<Creature> best_candidates(
        &creatures_[0], &creatures_[(int)(config_.population_size / 2)]);
    vector<Creature> children = {};

    int i = 0;

    while (children.size() < config_.population_size) {
      Creature p1 = best_candidates[i];
      Creature p2 = best_candidates[i + 1];
      Creature child = {0, 0, {}};

      child.position = rng::Range(0.0f, 1.0f);
      child.genome = {};

      for (int i = 0; i < config_.genome_size / 2; i++) {
        child.genome.push_back(p1.genome[i]);
        child.genome.push_back(p2.genome[i + config_.genome_size / 2]);
      }

      float random = rng::Range(0.0f, 1.0f);

      if (random <= config_.mutation_rate) {
        child.genome[rng::Range(0, config_.genome_size - 1)] = RandomGene();
      }

      children.push_back(child);

      i = (i + 1) % (config_.population_size / 2 - 1);
    }

    creatures_ = children;
  }

private:
  Gene RandomGene() {
    Gene gene = Gene{};

    gene.weight = rng::Range(-4.0f, 4.0f);
    gene.source_type = rng::Range(0, 1);

    switch (gene.source_type) {
    case 0:
      gene.source_id = rng::Range(0, config_.input_neurons);
      gene.sink_id = rng::Range(0, config_.internal_neurons);
      break;
    case 1:
      gene.source_id = rng::Range(0, config_.internal_neurons);
      gene.sink_id = rng::Range(0, config_.output_neurons);
      break;
    }

    return gene;
  }

  float Input1(Creature creature) {
    return config_.goal < creature.position ? 1 : 0;
  }

  float Input2(Creature creature) {
    return config_.goal > creature.position ? 1 : 0;
  }
};

Image GenerateFrame(Evolver evolver) {
  int width = 500;
  int height = 300;
  int bar_width = width / 50;
  ColorHSL bg = ColorHSL(0.61, 0.16, 0.22);
  ColorHSL bar_color = ColorHSL(0.62, 0.16, 0.28);
  Image image(Geometry(width, height), bg);

  vector<Drawable> draw_list;

  draw_list.push_back(DrawableFillColor(bar_color));

  vector<int> distrubution(50);

  for (auto &creature : evolver.creatures_) {
    int bar_index = min(49, (int)(creature.position * 50));
    distrubution[bar_index]++;
  }

  for (int i = 0; i < 50; i++) {
    int offset = i * bar_width;
    int y = height - (int)(height * ((float)distrubution[i] /
                                     evolver.config_.population_size));
    draw_list.push_back(
        DrawableRectangle(offset, y, offset + bar_width, height));
  }

  image.draw(draw_list);

  return image;
}

int main(int argc, char **argv) {
  InitializeMagick(*argv);

  rng::PlantSeed();

  float goal = rng::Range(0.0f, 1.0f);

  Evolver evolver = Evolver({200, 32, 100, 0.00, 2, 1, 2, goal});
  evolver.SpawnCreatures();

  vector<Image> frames;

  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < 100; i++) {
    evolver.RunGeneration();
    evolver.RankCreatures();

    float best_fitness = evolver.creatures_[0].fitness;
    float avg_fitness = evolver.AverageFitness();

    cout << "Generation " << i + 1 << endl;
    cout << "Best Fitness: " << best_fitness << endl;
    cout << "Average Fitness: " << avg_fitness << endl << endl;

    frames.push_back(GenerateFrame(evolver));

    evolver.Reproduce();
  }
  auto stop = chrono::high_resolution_clock::now();

  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

  cout << duration.count() << endl;

  writeImages(frames.begin(), frames.end(), "result.gif");

  return 0;
}
