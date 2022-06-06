#include <evolver/config.hpp>
#include <evolver/creature.hpp>
#include <evolver/evolver.hpp>
#include <evolver/genome.hpp>
#include <utils/random.hpp>

#include <Magick++.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace Magick;

Evolver::Evolver(EvolverConfig config) { config_ = config; }

void Evolver::RunSimulation(int generations) {
  rng::PlantSeed();

  SetRandomGoal();
  SpawnCreatures();

  vector<float> fitness_scores = {};

  for (int i = 0; i < generations; i++) {
    RunGeneration();
    RankCreatures();

    float best_fitness = creatures_[0].fitness;
    float avg_fitness = AverageFitness();

    cout << "Generation " << i + 1 << endl;
    cout << "Best Fitness: " << best_fitness << endl;
    cout << "Average Fitness: " << avg_fitness << endl
         << endl;

    fitness_scores.push_back(avg_fitness);

    Reproduce();
    SetRandomGoal();
  }

  vector<Image> frames = GenerateFrames(fitness_scores);

  writeImages(frames.begin(), frames.end(), "result.gif");
}

void Evolver::SetRandomGoal() {
  goal_ = rng::Range(0.0f, 1.0f);
}

void Evolver::SpawnCreatures() {
  for (int i = 0; i < config_.population_size; i++) {
    float position = rng::Range(0.0f, 1.0f);
    Creature creature = Creature{position};

    for (int j = 0; j < config_.genome_size; j++) {
      Gene gene = RandomGene(config_.input_neurons, config_.internal_neurons, config_.output_neurons);
      creature.genome.push_back(gene);
    }

    creatures_.push_back(creature);
  }
}

void Evolver::RunGeneration() {
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

      creature.fitness = 1 - abs(creature.position - goal_);
    }
  }
}

float Evolver::AverageFitness() {
  float sum = 0;

  for (int i = 0; i < config_.population_size; i++) {
    sum += creatures_[i].fitness;
  }

  return sum / config_.population_size;
}

void Evolver::RankCreatures() {
  sort(creatures_.begin(), creatures_.end(), [](Creature c1, Creature c2) {
    return c1.fitness > c2.fitness;
  });
}

void Evolver::Reproduce() {
  vector<Creature> best_candidates(&creatures_[0], &creatures_[(int)(config_.population_size / 2)]);
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
      child.genome[rng::Range(0, config_.genome_size - 1)] = RandomGene(config_.input_neurons, config_.internal_neurons, config_.output_neurons);
    }

    children.push_back(child);

    i = (i + 1) % (config_.population_size / 2 - 1);
  }

  creatures_ = children;
}

float Evolver::Input1(Creature creature) {
  return goal_ < creature.position ? 1 : 0;
}

float Evolver::Input2(Creature creature) {
  return goal_ > creature.position ? 1 : 0;
}

vector<Image> Evolver::GenerateFrames(vector<float> fitness_scores) {
  int width = 500;
  int height = 300;
  int generations = fitness_scores.size();
  int bar_width = width / generations;

  ColorHSL bg = ColorHSL(0.61, 0.16, 0.22);
  ColorHSL bar_color = ColorHSL(0.62, 0.16, 0.28);

  vector<Image> frames = {};

  for (int i = 0; i < generations; i++) {
    Image frame = Image(Geometry(500, 300), bg);
    vector<Drawable> draw_list;

    draw_list.push_back(DrawableFillColor(bar_color));

    for (int j = 0; j <= i; j++) {
      int offset = j * bar_width;
      int y = height - height * fitness_scores[j];
      draw_list.push_back(DrawableRectangle(offset, y, offset + bar_width, height));
    }

    frame.draw(draw_list);
    frames.push_back(frame);
  }

  return frames;
}
