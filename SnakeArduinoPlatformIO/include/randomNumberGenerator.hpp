#ifndef RNG
#define RNG
#include <random>

class randomNumberGenerator {
    virtual int selectNumber(int maximumNumber) = 0;
};

class mt19937RandomNumberGenerator : randomNumberGenerator {
    std::mt19937 rng;

    mt19937RandomNumberGenerator(std::mt19937 rng);
    int selectNumber(int maximumNumber);
};
#endif