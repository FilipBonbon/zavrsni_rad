#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>

#include "../include/util/DataLoader.hpp"
#include "../include/ScheduleSolver.hpp"

using namespace std;

ScheduleSolver::ScheduleSolver(ParentSelection *parentSelection, UnitCrossing *unitCrossing) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // strategies
    this->parentSelection = parentSelection;
    this->unitCrossing = unitCrossing;

    // data TODO: improve this part
    this->loadJmbags();
    this->loadAppointments();
    this->occupations = DataLoader::loadOccupations(); // sacu

    this->population = new int *[NUM_OF_UNITS];
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        this->population[i] = new int[numberOfStudents];
    }
    this->penalties = new double[NUM_OF_UNITS];
    this->collisions = new int[NUM_OF_UNITS];

    this->generateRandomPopulations();
}

ScheduleSolver::~ScheduleSolver() {
    delete[] jmbags;
    delete[] appointments;
    delete[] appointmentsDate;
    delete[] appointmentsFrom;
    delete[] appointmentsTo;

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        delete[] population[i];
    }
    delete[] population;
    delete[] penalties;
    delete[] collisions;

    delete parentSelection;
    delete unitCrossing;
}

void ScheduleSolver::loadJmbags() {
    auto jmbag_vector = DataLoader::loadJmbags();
    this->numberOfStudents = (int) jmbag_vector.size();
    this->jmbags = new long long[numberOfStudents];
    for (int i = 0; i < numberOfStudents; ++i) {
        jmbags[i] = jmbag_vector[i];
    }
}

void ScheduleSolver::loadAppointments() {
    auto appointment_vector = DataLoader::loadAppointments();
    this->numberOfAppointments = (int) appointment_vector.size();
    this->appointments = new string[numberOfAppointments];
    this->appointmentsDate = new string[numberOfAppointments];
    this->appointmentsFrom = new int[numberOfAppointments];
    this->appointmentsTo = new int[numberOfAppointments];

    string token;
    for (int i = 0; i < numberOfAppointments; ++i) {
        string appointment = appointment_vector[i];
        stringstream ss(appointment);
        appointments[i] = appointment;

        getline(ss, token, ';'); // name
        getline(ss, token, ';'); // capacity
        getline(ss, token, ';'); // date
        appointmentsDate[i] = token;

        getline(ss, token, ';'); // from
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        appointmentsFrom[i] = stoi(token);

        getline(ss, token, ';'); // to
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        appointmentsTo[i] = stoi(token);
    }
}

void ScheduleSolver::train() {
    int NUM_OF_ITERATIONS = 100000;

    int iteration = 0;
    do {
        auto **newPopulation = new int *[NUM_OF_UNITS];
        auto *newPenalties = new double[NUM_OF_UNITS];

        // elitizm
        newPopulation[0] = new int[numberOfStudents];
        auto eliteUnitIndex = getEliteUnit();
        for (int i = 0; i < numberOfStudents; ++i) {
            newPopulation[0][i] = population[eliteUnitIndex][i];
        }
        newPenalties[0] = penalties[eliteUnitIndex];

        for (int i = 1; i < NUM_OF_UNITS; i++) {
            newPopulation[i] = new int[numberOfStudents];
            auto parent1 = population[parentSelection->select(penalties, NUM_OF_UNITS, numberOfStudents)];
            auto parent2 = population[parentSelection->select(penalties, NUM_OF_UNITS, numberOfStudents)];
            unitCrossing->cross(parent1, parent2, newPopulation[i], numberOfStudents);
            mutate(newPopulation[i]);
            newPenalties[i] = calculatePenalties(newPopulation[i]);
        }

        assignNewPopulation(newPopulation, newPenalties);
        iteration++;

        if (iteration % 10 == 0) {
            cout << "Iteration " << iteration << " with best unit of penalty: " << penalties[getEliteUnit()] << endl;
        }

        if (iteration % 100 == 0) {
            cout << "saving schedule...:\n";
            printSchedule();
            cout << "penalties\n";
            for (int i = 0; i < NUM_OF_UNITS; ++i) {
                cout << penalties[i] << " ";
            }
            cout << "\n";
        }

    } while (iteration < NUM_OF_ITERATIONS);
}

void ScheduleSolver::generateRandomPopulations() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, numberOfAppointments - 1);

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        for (int j = 0; j < numberOfStudents; j++) {
            population[i][j] = dist(rng);
        }
        penalties[i] = calculatePenalties(population[i]);
    }
}

int ScheduleSolver::getEliteUnit() const {
    auto bestIndex = 0;
    auto bestValue = penalties[0];
    for (int i = 1; i < NUM_OF_UNITS; ++i) {
        if (penalties[i] < bestValue) {
            bestValue = penalties[i];
            bestIndex = i;
        }
    }
    return bestIndex;
}


double ScheduleSolver::calculatePenalties(int *unit) {
    double pentaly = 0;

    // TODO: optimize by making it array
    std::unordered_map<int, int> capacity;
    for (int i = 0; i < numberOfStudents; ++i) {
        int appointment = unit[i];
        capacity[appointment]++;
    }

    // appointements with broken requirements
    for (int i = 0; i < numberOfAppointments; ++i) {
        int numAssigned = capacity[i];

        if (numAssigned > 16) {
            pentaly += 30 * (numAssigned - 16);
        } else if (numAssigned < 15) {
            pentaly += 5 * (15 - numAssigned);
        }
    }

    // student collisions
    pentaly += 70 * calculateCollisions(unit);

    return pentaly;
}

int ScheduleSolver::calculateCollisions(int *unit) {
    int sum = 0;

    for (int i = 0; i < numberOfStudents; ++i) {
        auto jmbag = jmbags[i];
        auto appointementIndex = unit[i];
        auto date = appointmentsDate[appointementIndex];
        if (occupations.contains(jmbag) && occupations[jmbag].contains(date)) {
            auto times = occupations[jmbag][date];
            auto from = appointmentsFrom[appointementIndex];
            auto to = appointmentsTo[appointementIndex];
            for (int j = 0; j < times.size(); ++j) {
                if (get<0>(times[i]) < to && from < get<1>(times[i])) {
                    sum++;
                    break;
                }
            }
        }
    }

    return sum;
}

void ScheduleSolver::mutate(int *unit) const {
    double p = 0.01;

    for (int i = 0; i < numberOfStudents; ++i) {
        if ((static_cast<double>(std::rand()) / RAND_MAX) < p) {
            unit[i] = std::rand() % numberOfAppointments;
        }
    }
}

void ScheduleSolver::assignNewPopulation(int **newPopulation, double *newPenalties) {
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        delete[] population[i];
    }
    delete[] population;
    delete[] penalties;
    population = newPopulation;
    penalties = newPenalties;
}

void ScheduleSolver::printSchedule() {
    std::ofstream file("raspored.txt");

    if (file.is_open()) {
        auto eliteUnitIndex = getEliteUnit();
        auto eliteUnit = population[eliteUnitIndex];
        for (int i = 0; i < numberOfAppointments; ++i) {
            file << appointments[i] << "|";
            for (int j = 0; j < numberOfStudents; ++j) {
                if (eliteUnit[j] == i) {
                    file << jmbags[j] << " ";
                }
            }
            file << "\n";
        }
        file.close();
        std::cout << "File written successfully.\n";
    } else {
        std::cout << "Failed to open the file.\n";
    }
}
