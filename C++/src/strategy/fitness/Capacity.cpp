#include <unordered_map>

#include "../../../include/ScheduleSolver.hpp"
#include "../../../include/strategy/fitness/Capacity.hpp"

double Capacity::calculateFitness(const ScheduleSolver *solver, int index) {
    int **population = solver->population;
    double fitness = 0;

    std::unordered_map<int, int> capacity;
    for (int i = 0; i < solver->numberOfStudents; ++i) {
        int appointment = population[index][i];
        capacity[appointment]++;
    }

    for (int i = 0; i < solver->numberOfAppointments; ++i) {
        auto [upper, lower] = getCapacityLimitsFromAppointment(solver->appointments[i]);
        int numAssigned = capacity[i];

        if (numAssigned > upper) {
            fitness += 100 * (numAssigned - upper);
        } else if (numAssigned < lower) {
            fitness += 30 * (lower - numAssigned);
        }
    }

    return fitness;
}