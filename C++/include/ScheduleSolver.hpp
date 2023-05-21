#ifndef ZAVRSNI_RAD_SCHEDULESOLVER_HPP
#define ZAVRSNI_RAD_SCHEDULESOLVER_HPP

#include "../include/strategy/ParentSelection.hpp"
#include "../include/strategy/UnitCrossing.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <tuple>

class ScheduleSolver {
private: // Consts
    static const int NUM_OF_UNITS = 250;

private: // Data
    long long *jmbags;
    std::string *appointments;
    std::string *appointmentsDate;
    int *appointmentsFrom;
    int *appointmentsTo;
    std::unordered_map<long long, std::unordered_map<std::string, std::vector<std::tuple<int, int>>>> occupations;
    int numberOfStudents = 0;
    int numberOfAppointments = 0;
    int **population;
    double *penalties;
    int *collisions;

private: // Strategies
    ParentSelection *parentSelection;
    UnitCrossing *unitCrossing;

public:
    explicit ScheduleSolver(ParentSelection *, UnitCrossing *);

    ~ScheduleSolver();

private:
    void loadJmbags();

    void loadAppointments();

public:
    void train();

private:
    void generateRandomPopulations();

    int getEliteUnit() const;

    void assignNewPopulation(int **, double *);

    double calculatePenalties(int *);

    int calculateCollisions(int *);

    void mutate(int *) const;

    void printSchedule();
};

#endif
