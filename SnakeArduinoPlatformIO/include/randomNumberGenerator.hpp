#ifndef RNG
#define RNG

class randomNumberGenerator {
    public:
    virtual int selectNumber(int maximumNumber) = 0;
};
#endif