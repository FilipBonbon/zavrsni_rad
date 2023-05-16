#include <random>
#include <utility>

#include "../include/ScheduleSolver.hpp"
#include "../include/util/DataLoader.hpp"

ScheduleSolver::ScheduleSolver(list<Fitness *> fitnessFunctions) {
    this->fitnessFunctions = fitnessFunctions;
    this->jmbags = DataLoader::loadJmbags();
    this->occupations = DataLoader::loadOccupations();
    this->appointments = DataLoader::loadAppointments();

    this->numberOfStudents = (int) jmbags.size();
    this->numberOfAppointments = (int) appointments.size();

    this->population = new int *[NUM_OF_UNITS];
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        this->population[i] = new int[numberOfStudents];
    }

    generateRandomPopulations();
}

ScheduleSolver::~ScheduleSolver() {
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        delete population[i];
    }
    delete population;
}

void ScheduleSolver::generateRandomPopulations() const {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, numberOfAppointments - 1);

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        for (int j = 0; j < numberOfStudents; j++) {
            population[i][j] = dist(rng);
        }
    }
}

void ScheduleSolver::solve() const {
    cout << "Solve!" << endl;
}

double ScheduleSolver::calculateFitness() const {
    double sum = 0;
    for (auto fitnessFunction: fitnessFunctions) {
        sum += fitnessFunction->calculateFitness();
    }
    return sum;
}

void ScheduleSolver::cross() const {
    cout << "cross" << endl;
}

void ScheduleSolver::mutate() const {
    cout << "mutate" << endl;
}

int ScheduleSolver::chooseParent() const {
    cout << "chooseParent" << endl;
    return 0;
}
