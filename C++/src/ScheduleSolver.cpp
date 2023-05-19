#include <iostream>
#include <random>
#include <utility>
#include <chrono>

#include "../include/util/DataLoader.hpp"
#include "../include/ScheduleSolver.hpp"
#include "../include/strategy/Fitness.hpp"

using namespace std;

ScheduleSolver::ScheduleSolver(list<Fitness *> fitnessFunctions, ParentSelection* parentSelection) {
    // strategies
    this->fitnessFunctions = std::move(fitnessFunctions);
    this->parentSelection = parentSelection;

    // data
    this->loadJmbags();
    this->loadAppointments();
    this->occupations = DataLoader::loadOccupations();

    this->population = new int *[NUM_OF_UNITS];
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        this->population[i] = new int[numberOfStudents];
    }
    this->fitnesses = new double[NUM_OF_UNITS];

    generateRandomPopulations();
}

ScheduleSolver::~ScheduleSolver() {
    delete jmbags;
    delete appointments;

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        delete population[i];
    }
    delete population;
    delete fitnesses;

    for (auto &item: fitnessFunctions) {
        delete item;
    }
    delete parentSelection;
    // TODO: after adding uncomment
    /*delete crossStrategy;
    delete mutationStrategy;*/
}

void ScheduleSolver::loadJmbags() {
    auto jmbag_vector = DataLoader::loadJmbags();
    this->numberOfStudents = (int) jmbag_vector.size();
    this->jmbags = new long[numberOfStudents];
    for (int i = 0; i < numberOfStudents; ++i) {
        jmbags[i] = jmbag_vector[i];
    }
}

void ScheduleSolver::loadAppointments() {
    auto appointment_vector = DataLoader::loadAppointments();
    this->numberOfAppointments = (int) appointment_vector.size();
    this->appointments = new string[numberOfAppointments];
    for (int i = 0; i < numberOfAppointments; ++i) {
        appointments[i] = appointment_vector[i];
    }
}

void ScheduleSolver::train() {
    auto startTime = std::chrono::high_resolution_clock::now();

    int NUM_OF_ITERATIONS = 100;

    int iteration = 0;
    do {
        int **newPopulation = new int *[NUM_OF_UNITS];
        for (int i = 0; i < NUM_OF_UNITS; i++) {
            newPopulation[i] = new int[numberOfStudents];
            auto parent1 = parentSelection->select(population, fitnesses, NUM_OF_UNITS, numberOfStudents);
            auto parent2 = parentSelection->select(population, fitnesses, NUM_OF_UNITS, numberOfStudents);
            //calculateFitness(i);
            //calculateFitness(i);
        }

        // delete old population
        for (int i = 0; i < NUM_OF_UNITS; i++) {
            delete population[i];
        }
        delete population;

        population = newPopulation;

        iteration++;
    } while(iteration < NUM_OF_ITERATIONS);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Execution time for 100 iterations: " << duration.count() << " milliseconds" << std::endl;
}

void ScheduleSolver::generateRandomPopulations() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, numberOfAppointments - 1);

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        for (int j = 0; j < numberOfStudents; j++) {
            population[i][j] = dist(rng);
        }
        fitnesses[i] = calculateFitness(i);
    }
}

double ScheduleSolver::calculateFitness(int i) {
    double sum = 0;
    for (auto fitnessFunction: fitnessFunctions) {
        sum += fitnessFunction->calculateFitness(this, i);
    }
    return sum;
}

void ScheduleSolver::cross() {
    cout << "cross" << endl;
}

void ScheduleSolver::mutate() {
    cout << "mutate" << endl;
}

int ScheduleSolver::chooseParent() {
    cout << "chooseParent" << endl;
    return 0;
}
