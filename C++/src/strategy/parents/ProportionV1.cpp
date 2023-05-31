#include <random>
#include <iostream>

#include "../../../include/strategy/parents/ProportionV1.hpp"

int ProportionV1::select(int *penalties, int populationSize, int *collisions) {
    double penaltySum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        penaltySum += penalties[i];
    }

    double probs[populationSize];
    double probSum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        probs[i] = (penaltySum - penalties[i]) / penaltySum;
        probSum += probs[i];
    }

    for (int i = 0; i < populationSize; ++i) {
        probs[i] /= probSum;
    }

    std::mt19937 gen((std::random_device()()));
    std::uniform_real_distribution<double> dis(0.0, 1);
    double randomNum = dis(gen);

    double area = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        area += probs[i];
        if (randomNum <= area) {
            return i;
        }
    }

    return populationSize - 1;
}
