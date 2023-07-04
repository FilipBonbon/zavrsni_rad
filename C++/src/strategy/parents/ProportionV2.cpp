#include <random>
#include <iostream>

#include "../../../include/strategy/parents/ProportionV2.hpp"

int ProportionV2::select(int *penalties, int populationSize, int *collisions) {
    double smallest = penalties[0];
    for (int i = 1; i < populationSize; ++i) {
        if (penalties[i] < smallest) {
            smallest = penalties[i];
        }
    }

    double probabilities[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        probabilities[i] = penalties[i] - smallest;
    }

    double biggest = probabilities[0];
    for (int i = 1; i < populationSize; ++i) {
        if (probabilities[i] > biggest) {
            biggest = probabilities[i];
        }
    }

    double sum = 0;
    for (int i = 0; i < populationSize; ++i) {
        probabilities[i] = biggest - probabilities[i];
        sum += probabilities[i];
    }

    for (int i = 0; i < populationSize; ++i) {
        probabilities[i] /= sum;
    }

    std::mt19937 gen((std::random_device()()));
    std::uniform_real_distribution<double> dis(0.0, 1);
    double randomNum = dis(gen);

    double area = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        area += probabilities[i];
        if (randomNum <= area) {
            return i;
        }
    }

    return populationSize - 1;
}
