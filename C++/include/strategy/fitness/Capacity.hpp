#ifndef ZAVRSNI_RAD_MISSPLACED_HPP
#define ZAVRSNI_RAD_MISSPLACED_HPP

#include "../Fitness.hpp"

class Capacity : public Fitness {
public:
    double calculateFitness(const ScheduleSolver *, int) override;
};


#endif
