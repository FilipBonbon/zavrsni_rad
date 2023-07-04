#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>

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

    // data loading
    this->loadJmbags();
    this->loadAppointments();
    this->loadOccupations();

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
        this->jmbags[i] = jmbag_vector[i];
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
        auto appointment = appointment_vector[i];
        auto ss = stringstream(appointment);
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

void ScheduleSolver::loadOccupations() {
    this->occupations = DataLoader::loadOccupations();
}

void ScheduleSolver::train() {
    train(INT32_MAX);
}

void ScheduleSolver::train(long max_iterations) {
    auto iteration = 1L;
    do {
        auto **newPopulation = new int *[NUM_OF_UNITS];
        auto *newPenalties = new int[NUM_OF_UNITS];
        auto *newCollisions = new int[NUM_OF_UNITS];

        // elitism
        newPopulation[0] = new int[numberOfStudents];
        auto eliteUnitIndex = getEliteUnit();
        for (int i = 0; i < numberOfStudents; ++i) {
            newPopulation[0][i] = population[eliteUnitIndex][i];
        }
        newPenalties[0] = penalties[eliteUnitIndex];
        newCollisions[0] = collisions[eliteUnitIndex];

        for (int i = 1; i < NUM_OF_UNITS; i++) {
            auto parent1 = population[parentSelection->select(penalties, NUM_OF_UNITS, collisions)];
            auto parent2 = population[parentSelection->select(penalties, NUM_OF_UNITS, collisions)];
            while (parent2 == parent1) {
                parent2 = population[parentSelection->select(penalties, NUM_OF_UNITS, collisions)];
            }
            auto child = unitCrossing->cross(parent1, parent2, numberOfStudents);
            mutate(child);
            auto [penaltyChild, collisionChild] = calculatePenalties(child);

            newPopulation[i] = child;
            newPenalties[i] = penaltyChild;
            newCollisions[i] = collisionChild;
        }

        assignNewPopulation(newPopulation, newPenalties, newCollisions);

        if (iteration % 10 == 0) {
            auto eliteIndex = getEliteUnit();
            cout << "Iteration " << iteration << " with best unit with " << collisions[eliteIndex]
                 << " collisions and penalty: " << penalties[eliteIndex] << "\n";
        }

        if (iteration % 100 == 0) {
            cout << "Saving schedules...:\n";
            printSchedule();
            printScheduleRich();
        }

        exportIterationData();

        iteration++;
    } while (iteration < max_iterations);

    cout << "Saving schedules...:\n";
    printSchedule();
    printScheduleRich();

    exportIterationData();
    auto eliteIndex = getEliteUnit();
    cout << "Training finished. Found best unit with " << collisions[eliteIndex]
         << " collisions and penalty: " << penalties[eliteIndex] << "\n";
}

void ScheduleSolver::generateRandomPopulations() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, numberOfAppointments - 1);

    ofstream file("data.txt");
    file << NUM_OF_UNITS << "\n";
    file.close();

    this->population = new int *[this->NUM_OF_UNITS];
    for (int i = 0; i < this->NUM_OF_UNITS; i++) {
        this->population[i] = new int[numberOfStudents];
    }
    this->penalties = new int[this->NUM_OF_UNITS];
    this->collisions = new int[this->NUM_OF_UNITS];

    for (int i = 0; i < this->NUM_OF_UNITS; i++) {
        for (int j = 0; j < this->numberOfStudents; j++) {
            this->population[i][j] = dist(rng);
        }
        auto [a, b] = this->calculatePenalties(this->population[i]);
        this->penalties[i] = a;
        this->collisions[i] = b;
    }

    exportIterationData();
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


tuple<int, int> ScheduleSolver::calculatePenalties(const int *unit) {
    auto penalty = 0;

    auto *capacity = new int[numberOfAppointments]();
    for (int i = 0; i < numberOfStudents; ++i) {
        auto appointment = unit[i];
        capacity[appointment]++;
    }

    // appointments with broken requirements
    for (int i = 0; i < numberOfAppointments; ++i) {
        auto numAssigned = capacity[i];

        if (numAssigned > 16) {
            penalty += 95 * pow((numAssigned - 16), 2);
        } else if (numAssigned < 15) {
            penalty += 55 * pow((15 - numAssigned), 2);
        }
    }

    // student collisions
    int collNum = 0;
    for (int i = 0; i < numberOfStudents; ++i) {
        auto jmbag = jmbags[i];
        auto appointmentIndex = unit[i];
        auto date = appointmentsDate[appointmentIndex];
        auto from = appointmentsFrom[appointmentIndex];
        auto to = appointmentsTo[appointmentIndex];
        if (occupations.contains(jmbag) && occupations[jmbag].contains(date)) {
            auto sequential = false;
            auto overlap = false;
            for (auto &time: occupations[jmbag][date]) {
                if (max(get<1>(time), to) - min(get<0>(time), from) < (get<1>(time) - get<0>(time)) + (to - from)) {
                    collNum++;
                    overlap = true;
                }
            }
            if (!overlap) {
                for (auto &time: occupations[jmbag][date]) {
                    if (get<0>(time) == to || from == get<1>(time)) {
                        penalty -= 5;
                        sequential = true;
                        break;
                    }
                }
                if (!sequential) {
                    auto smallestGap = 1000000;
                    for (auto &time: occupations[jmbag][date]) {
                        auto gap = min(abs(from - get<1>(time)), abs(to - get<0>(time)));
                        smallestGap = min(gap, smallestGap);
                    }
                    penalty += 2 * pow((smallestGap / 100), 2);
                }
            }
        } else {
            penalty += 100;
        }
    }

    penalty += 150 * collNum;

    delete[] capacity;
    return {penalty, collNum};
}

void ScheduleSolver::mutate(int *unit) const {
    for (int i = 0; i < numberOfStudents; ++i) {
        if ((static_cast<double>(std::rand()) / RAND_MAX) < MUTATION) {
            unit[i] = std::rand() % numberOfAppointments;
        }
    }
}

void ScheduleSolver::assignNewPopulation(int **newPopulation, int *newPenalties, int *newCollisions) {
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
                    file << std::setw(10) << std::setfill('0') << jmbags[j] << " ";
                }
            }
            file << "\n";
        }
        file.close();
    } else {
        std::cout << "Failed to open raspored.txt.\n";
    }
}

void ScheduleSolver::printScheduleRich() {
    std::ofstream file("raspored_preklapanja.txt");

    if (file.is_open()) {
        auto eliteUnitIndex = getEliteUnit();
        auto eliteUnit = population[eliteUnitIndex];

        file << "Total penalty: " << penalties[eliteUnitIndex] << "\n" << "\n";
        for (int i = 0; i < numberOfStudents; ++i) {
            auto jmbag = jmbags[i];
            auto appointmentIndex = eliteUnit[i];
            auto appointmentDate = appointmentsDate[appointmentIndex];

            file << i + 1 << ". " << std::setw(10) << std::setfill('0') << jmbag << "\n";
            file << "-------------------------------------\n";
            if (occupations.contains(jmbag) && occupations[jmbag].contains(appointmentDate)) {
                for (auto &vec: occupations[jmbag][appointmentDate]) {
                    auto from = to_string(get<0>(vec));
                    from.insert(from.length() - 2, ":");
                    auto to = to_string(get<1>(vec));
                    to.insert(to.length() - 2, ":");
                    file << appointmentDate << " " << from << " " << to << "\n";
                }
                auto appointment_stream = stringstream(appointments[appointmentIndex]);
                auto parts = std::vector<string>();
                string token;

                while (getline(appointment_stream, token, ';')) {
                    parts.emplace_back(token);
                }

                file << parts[2] << " " << parts[3] << " " << parts[4] << " -- " << parts[5] << "\n";
            } else {
                auto appointment_stream = stringstream(appointments[appointmentIndex]);
                auto parts = std::vector<string>();
                string token;

                while (getline(appointment_stream, token, ';')) {
                    parts.emplace_back(token);
                }

                file << parts[2] << " " << parts[3] << " " << parts[4] << " -- " << parts[5] << "\n";
            }
            file << "\n\n";
        }
        file.close();
    } else {
        std::cout << "Failed to open raspored_preklapanja.txt.\n";
    }
}

void ScheduleSolver::exportIterationData() {
    auto file = ofstream("data.txt", std::ios::app);
    for (int i = 0; i < NUM_OF_UNITS - 1; ++i) {
        file << penalties[i] << " ";
    }
    file << penalties[NUM_OF_UNITS - 1] << "\n";
    file.close();
}
