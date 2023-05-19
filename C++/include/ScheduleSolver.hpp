#ifndef ZAVRSNI_RAD_SCHEDULESOLVER_HPP
#define ZAVRSNI_RAD_SCHEDULESOLVER_HPP

#include "../include/strategy/ParentSelection.hpp"

#include <unordered_map>
#include <list>
#include <vector>
#include <string>

class Fitness;

class ScheduleSolver {
public: // Consts
    static const int NUM_OF_UNITS = 64;

public: // Data
    long *jmbags = nullptr;
    std::string *appointments = nullptr; // TODO: possibly remove
    std::unordered_map<long, std::vector<std::string>> occupations; // TODO: possibly shoten
    int numberOfStudents = 0;
    int numberOfAppointments = 0;
    int **population;
    double *fitnesses;

public: // Strategies
    std::list<Fitness *> fitnessFunctions;
    ParentSelection* parentSelection;
    void* crossStrategy{};
    void* mutationStrategy{};

public:
    explicit ScheduleSolver(std::list<Fitness *>, ParentSelection *);

    ~ScheduleSolver();

private:
    void loadJmbags();

    void loadAppointments();

public:
    void train();

private:
    void generateRandomPopulations();

    double calculateFitness(int);

    void cross();

    void mutate();

    int chooseParent();
};

#endif
