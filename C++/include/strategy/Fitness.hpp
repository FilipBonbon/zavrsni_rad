#ifndef ZAVRSNI_RAD_FITNESS_HPP
#define ZAVRSNI_RAD_FITNESS_HPP

#include <sstream>
#include <string>
#include <tuple>

class ScheduleSolver;

class Fitness {
public:
    virtual ~Fitness() = default;
    virtual double calculateFitness(const ScheduleSolver *, int) = 0;

protected:
    static std::tuple<int,int> getCapacityLimitsFromAppointment(std::string &str) {
        std::stringstream ss(str);
        std::string token;
        int upper_bound;
        int lower_bound;

        // name;upper_bound/lower_bound;...
        getline(ss, token, ';'); // name

        getline(ss, token, '/');
        lower_bound = stoi(token);
        getline(ss, token, ';');
        upper_bound = stoi(token);

        return {upper_bound, lower_bound};
    }
};


#endif
