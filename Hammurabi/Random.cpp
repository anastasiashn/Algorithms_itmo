#include <iostream>

#include "Random.h"

int Random::getRandomNumber(int min, int max) {
    std::srand(std::time(nullptr));
    int range = max - min + 1;
    return std::rand() % range + min;
}

bool Random::getPercent(int percent) {
    int number = getRandomNumber(0, 100);
    return number <= percent;
}