#include <evolver/evolver.hpp>
#include <iostream>
#include <utils/random.hpp>

using namespace std;

Evolver::Evolver(int population_size, int genome_size, int generation_steps, float mutation_rate, unsigned int internal_count) {
  population_size_ = population_size;
  genome_size_ = genome_size;
  generation_steps_ = generation_steps;
  mutation_rate_ = mutation_rate;
  internal_count_ = internal_count;
}

void Evolver::AddInput(function<float(Creature &creature)> input_func) {
  input_funcs_.push_back(input_func);
  input_count_++;
}

void Evolver::AddOutput(function<void(Creature &creature)> output_func) {
  output_funcs_.push_back(output_func);
  output_count_++;
}

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
  for (int i = 0; i < population_size_; i++) {
    float position = rng::Range(0.0f, 1.0f);
    Creature creature = Creature{position};

    for (int j = 0; j < genome_size_; j++) {
      Gene gene = RandomGene(input_count_, internal_count_, output_count_);
      creature.genome.push_back(gene);
    }

    creatures_.push_back(creature);
  }
}

void Evolver::RunGeneration() {
  for (int i = 0; i < generation_steps_; i++) {
    for (auto &creature : creatures_) {
      vector<float> inputs = GetInputs(creature);
      vector<float> internal(internal_count_);

      for (auto &gene : creature.genome) {
        if (gene.layer != 0) {
          continue;
        }
        internal[gene.sink] += inputs[gene.source] * gene.weight;
      }

      for (int j = 0; j < internal_count_; j++) {
        internal[j] = tanh(internal[j]);
      }

      vector<float> output(output_count_);

      for (auto &gene : creature.genome) {
        if (gene.layer != 1) {
          continue;
        }
        output[gene.sink] += internal[gene.source] * gene.weight;
      }

      for (int j = 0; j < output_count_; j++) {
        output[j] = tanh(output[j]);
        float value = output[j];
        if (value <= 0) {
          continue;
        }
        if (rng::Range(0.0f, 1.0f) <= value) {
          output_funcs_[j](creature);
        }
      }

      creature.fitness = 1 - abs(creature.position - goal_);
    }
  }
}

vector<float> Evolver::GetInputs(Creature &creature) {
  vector<float> inputs;
  for (auto &input_func : input_funcs_) {
    inputs.push_back(input_func(creature));
  }
  return inputs;
}

float Evolver::AverageFitness() {
  float sum = 0;
  for (int i = 0; i < population_size_; i++) {
    sum += creatures_[i].fitness;
  }
  return sum / population_size_;
}

void Evolver::RankCreatures() {
  sort(creatures_.begin(), creatures_.end(), [](Creature c1, Creature c2) {
    return c1.fitness > c2.fitness;
  });
}

void Evolver::Reproduce() {
  vector<Creature> best_candidates(&creatures_[0], &creatures_[(int)(population_size_ / 2)]);
  vector<Creature> children = {};

  int i = 0;

  while (children.size() < population_size_) {
    Creature p1 = best_candidates[i];
    Creature p2 = best_candidates[i + 1];
    Creature child = {0, 0, {}};

    child.position = rng::Range(0.0f, 1.0f);
    child.genome = {};

    for (int i = 0; i < genome_size_ / 2; i++) {
      child.genome.push_back(p1.genome[i]);
      child.genome.push_back(p2.genome[i + genome_size_ / 2]);
    }

    float random = rng::Range(0.0f, 1.0f);

    if (random <= mutation_rate_) {
      child.genome[rng::Range(0, genome_size_ - 1)] = RandomGene(input_count_, internal_count_, output_count_);
    }

    children.push_back(child);

    i = (i + 1) % (population_size_ / 2 - 1);
  }

  creatures_ = children;
}

vector<Image> Evolver::GenerateFrames(vector<float> fitness_scores) {
  int width = 1000;
  int height = 600;
  Geometry frame_size = Geometry(width, height);
  int generations = fitness_scores.size();
  int bar_width = width / generations;

  ColorHSL bg = ColorHSL(0.61, 0.16, 0.22);
  ColorHSL bar_color = ColorHSL(0.62, 0.16, 0.28);

  vector<Image> frames = {};

  for (int i = 0; i < generations; i++) {
    Image frame = Image(frame_size, bg);
    vector<Drawable> draw_list = {};

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
