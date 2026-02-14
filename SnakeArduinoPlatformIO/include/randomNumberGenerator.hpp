#ifndef RNG
#define RNG
#include <random>

class randomNumberGenerator {
    public:
    virtual int selectNumber(int maximumNumber) = 0;
};

class mt19937RandomNumberGenerator : randomNumberGenerator {
    std::mt19937 rng;

    public:
    mt19937RandomNumberGenerator(std::mt19937 rng);
    int selectNumber(int maximumNumber);
};
#endif