#ifndef ZAVRSNI_RAD_SCHEDULESOLVER_HPP
#define ZAVRSNI_RAD_SCHEDULESOLVER_HPP

#include "strategy/Fitness.hpp"

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <vector>

using namespace std;

class ScheduleSolver {
private: // Consts
    static const int NUM_OF_UNITS = 256;

private: // Data
    unordered_set<long> jmbags;
    unordered_map<long, vector<string>> occupations;
    vector<string> appointments;

private: // Strategies
    list<Fitness *> fitnessFunctions;
    void* crossStrategy;
    void* selectParentStrategy;
    void* mutationStrategy;

private: // Util variables
    int numberOfStudents;
    int numberOfAppointments;
    int **population;

public:
    explicit ScheduleSolver(list<Fitness *>);

    ~ScheduleSolver();

private:
    void generateRandomPopulations() const;

public:
    void solve() const;

private:
    double calculateFitness() const;

    void cross() const;

    void mutate() const;

    int chooseParent() const;
};

#endif
