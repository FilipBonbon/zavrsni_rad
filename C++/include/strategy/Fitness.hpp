#ifndef ZAVRSNI_RAD_FITNESS_HPP
#define ZAVRSNI_RAD_FITNESS_HPP


class Fitness {
public:
    virtual ~Fitness() = default;
    virtual double calculateFitness() = 0;
};


#endif
