#pragma once
#include <stdlib.h>
#include <time.h>

class RandomGenerator {
    unsigned int seed;

  public:
    RandomGenerator();

    RandomGenerator(unsigned int s);

    void setSeed(unsigned int seed);

    int rand();

    int rand(int min, int max);
};
