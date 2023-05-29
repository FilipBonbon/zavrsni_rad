#include <random>
#include <iostream>

#include "../../../include/strategy/parents/Proportion.hpp"

int Proportion::select(int *penalties, int populationSize, int *collisions) {
    double fitnessesSum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        fitnessesSum += penalties[i];
        if (penalties[i] == 0) {
            return i;
        }
    }

    double probs[populationSize];
    double probSum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        probs[i] = (fitnessesSum - penalties[i]) / fitnessesSum;
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
