#ifndef MOCK
#define MOCK
#include <gmock/gmock.h>

#include "randomNumberGenerator.hpp"

class mockRandomNumberGenerator: public randomNumberGenerator{
    public:
    MOCK_METHOD(int, selectNumber, (int maximum), (override));
};

#endif