#include <random>

#include "../../../include/strategy/parents/Proportion.hpp"

int * Proportion::select(int **population, double *fitnesses, int populationSize, int studentsSize) {
    double fitnessesSum = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        fitnessesSum += fitnesses[i];
    }

    std::mt19937 gen((std::random_device()()));
    std::uniform_real_distribution<double> dis(0.0, fitnessesSum);
    double randomNum = dis(gen);

    double area = 0.0;
    for (int i = 0; i < populationSize; ++i) {
        area += fitnesses[i];
        if (randomNum <= area) {
            return population[i];
        }
    }

    return population[populationSize - 1];
}
