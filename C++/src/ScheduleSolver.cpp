#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <cmath>

#include "../include/util/DataLoader.hpp"
#include "../include/ScheduleSolver.hpp"

using namespace std;

ScheduleSolver::ScheduleSolver(ParentSelection *parentSelection,
                               UnitCrossing *unitCrossing,
                               int numOfUnits,
                               double mutation) : NUM_OF_UNITS(numOfUnits), MUTATION(mutation) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // strategies
    this->parentSelection = parentSelection;
    this->unitCrossing = unitCrossing;

    this->loadJmbags();
    this->loadAppointments();
    this->occupations = DataLoader::loadOccupations();

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

[[noreturn]] void ScheduleSolver::train() {
    int iteration = 1;
    do {
        auto **newPopulation = new int *[NUM_OF_UNITS];
        auto *newPenalties = new double[NUM_OF_UNITS];
        auto *newCollisions = new int[NUM_OF_UNITS];

        // elitizm
        newPopulation[0] = new int[numberOfStudents];
        auto eliteUnitIndex = getEliteUnit();
        for (int i = 0; i < numberOfStudents; ++i) {
            newPopulation[0][i] = population[eliteUnitIndex][i];
        }
        newPenalties[0] = penalties[eliteUnitIndex];
        newCollisions[0] = collisions[eliteUnitIndex];

        for (int i = 1; i < NUM_OF_UNITS; i++) {
            newPopulation[i] = new int[numberOfStudents];
            auto parent1 = population[parentSelection->select(penalties, NUM_OF_UNITS, numberOfStudents)];
            auto parent2 = population[parentSelection->select(penalties, NUM_OF_UNITS, numberOfStudents)];
            unitCrossing->cross(parent1, parent2, newPopulation[i], numberOfStudents);
            mutate(newPopulation[i]);
            auto [a, b] = calculatePenalties(newPopulation[i]);
            newPenalties[i] = a;
            newCollisions[i] = b;
        }

        assignNewPopulation(newPopulation, newPenalties, newCollisions);

        if (iteration % 10 == 0) {
            auto eliteIndex = getEliteUnit();
            cout << "Iteration " << iteration << " with best unit with " << collisions[eliteIndex]
                 << " collisions and penlaty: " << penalties[eliteIndex] << "\n";
        }

        if (iteration % 100 == 0) {
            cout << "saving schedules...:\n";
            printSchedule();
            printScheduleRich();
        }

        iteration++;
    } while (true);
}

void ScheduleSolver::generateRandomPopulations() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, numberOfAppointments - 1);

    for (int i = 0; i < NUM_OF_UNITS; i++) {
        for (int j = 0; j < numberOfStudents; j++) {
            population[i][j] = dist(rng);
        }
        auto [a, b] = calculatePenalties(population[i]);
        penalties[i] = a;
        collisions[i] = b;
    }
}

int ScheduleSolver::getEliteUnit() const {
    auto bestCollision = collisions[0];
    auto bestIndex = 0;
    auto bestValue = penalties[0];
    for (int i = 1; i < NUM_OF_UNITS; ++i) {
        if (collisions[i] < bestCollision) {
            bestValue = penalties[i];
            bestIndex = i;
            bestCollision = collisions[i];
        } else if (collisions[i] == bestCollision && penalties[i] < bestValue) {
            bestValue = penalties[i];
            bestIndex = i;
        }
    }
    return bestIndex;
}


tuple<double, int> ScheduleSolver::calculatePenalties(int *unit) {
    double pentaly = 0;

    int *capacity = new int[numberOfAppointments]();
    for (int i = 0; i < numberOfStudents; ++i) {
        int appointment = unit[i];
        capacity[appointment]++;
    }

    // appointements with broken requirements
    for (int i = 0; i < numberOfAppointments; ++i) {
        int numAssigned = capacity[i];

        if (numAssigned > 16) {
            pentaly += 50 * pow((numAssigned - 16), 2);
        } else if (numAssigned < 15) {
            pentaly += 35 * pow((15 - numAssigned), 2);
        }
    }

    // student collisions
    int collNum = 0;
    for (int i = 0; i < numberOfStudents; ++i) {
        auto jmbag = jmbags[i];
        auto appointementIndex = unit[i];
        auto date = appointmentsDate[appointementIndex];
        if (occupations.contains(jmbag) && occupations.at(jmbag).contains(date)) {
            for (auto &time: occupations[jmbag][date]) {
                if (get<0>(time) < appointmentsTo[appointementIndex] &&
                    appointmentsFrom[appointementIndex] < get<1>(time)) {
                    collNum++;
                    break;
                }
            }
        }
    }

    pentaly += 75 * collNum;


    delete[] capacity;
    return {pentaly, collNum};
}
void ScheduleSolver::mutate(int *unit) const {
    for (int i = 0; i < numberOfStudents; ++i) {
        if ((static_cast<double>(std::rand()) / RAND_MAX) < MUTATION) {
            unit[i] = std::rand() % numberOfAppointments;
        }
    }
}

void ScheduleSolver::assignNewPopulation(int **newPopulation, double *newPenalties, int* newCollisions) {
    for (int i = 0; i < NUM_OF_UNITS; i++) {
        delete[] population[i];
    }
    delete[] population;
    delete[] penalties;
    delete[] collisions;
    population = newPopulation;
    penalties = newPenalties;
    collisions = newCollisions;
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

void ScheduleSolver::printScheduleRich() {
    std::ofstream file("raspored_preklapanja.txt");

    if (file.is_open()) {
        auto eliteUnitIndex = getEliteUnit();
        auto eliteUnit = population[eliteUnitIndex];

        file << "Total penalty: " << penalties[eliteUnitIndex] << "\n\n";
        for (int i = 0; i < numberOfStudents; ++i) {
            auto jmbag = jmbags[i];
            auto appointmentIndex = eliteUnit[i];
            auto appointmentDate = appointmentsDate[appointmentIndex];

            file << i + 1 << ". " << jmbag << "\n";
            file << "-------------------------------------\n";
            if (occupations.contains(jmbag) && occupations[jmbag].contains(appointmentDate)) {
                auto vec = occupations[jmbag][appointmentDate];
                for (int j = 0; j < vec.size(); ++j) {
                    file << appointmentDate << " " << get<0>(vec[j]) << " " << get<1>(vec[j]) << "\n";
                }
                file << appointments[appointmentIndex] << "\n";
            } else {
                file << appointments[appointmentIndex] << "\n";
            }
            file << "\n\n";
        }
        file.close();
        std::cout << "File written successfully.\n";
    } else {
        std::cout << "Failed to open the file.\n";
    }
}
