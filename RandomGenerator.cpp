#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() { seed = time(NULL); }

RandomGenerator::RandomGenerator(unsigned int s) : seed(s) {}

void RandomGenerator::setSeed(unsigned int seed) { this->seed = seed; }

int RandomGenerator::rand() { return rand_r(&seed); }

int RandomGenerator::rand(int min, int max) {
    int range{max - min + 1};
    return rand_r(&seed) % range + min;
}
