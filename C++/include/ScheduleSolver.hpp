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
    const int NUM_OF_UNITS;
    double MUTATION;

private: // Data
    long long *jmbags;
    std::string *appointments;
    std::string *appointmentsDate;
    int *appointmentsFrom;
    int *appointmentsTo;
    std::unordered_map<long long, std::unordered_map<std::string, std::vector<std::tuple<int, int>>>> occupations;
    int numberOfStudents;
    int numberOfAppointments;
    int **population;
    int *penalties;
    int *collisions;

private: // Strategies
    ParentSelection *parentSelection;
    UnitCrossing *unitCrossing;

public:
    explicit ScheduleSolver(ParentSelection *, UnitCrossing *, int, double);

    ~ScheduleSolver();

private:
    void loadJmbags();

    void loadAppointments();

    void loadOccupations();

public:
    void train();

    void train(long);

private:
    void generateRandomPopulations();

    int getEliteUnit() const;

    void assignNewPopulation(int **, int *, int *);

    std::tuple<int, int> calculatePenalties(int *);

    void mutate(int *) const;

    void printSchedule();

    void printScheduleRich();

    void exportIterationData();
};

#endif
