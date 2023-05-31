#include <random>
#include <iostream>

#include "../../../include/strategy/parents/ProportionV2.hpp"

int ProportionV2::select(int *penalties, int populationSize, int *collisions) {
    auto smallest = penalties[0];
    for (int i = 1; i < populationSize; ++i) {
        if (penalties[i] < smallest) smallest = penalties[i];
    }

    double probs[populationSize];
    smallest = -smallest;
    for (int i = 0; i < populationSize; ++i) {
        probs[i] = penalties[i] + smallest;
    }

    double penaltySum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        penaltySum += probs[i];
    }

    for (int i = 0; i < populationSize; ++i) {
        probs[i] = (penaltySum - probs[i]) / penaltySum;
    }

    double probSum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
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
