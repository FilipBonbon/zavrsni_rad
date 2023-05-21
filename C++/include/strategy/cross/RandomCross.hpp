#ifndef ZAVRSNI_RAD_RANDOMCROSS_HPP
#define ZAVRSNI_RAD_RANDOMCROSS_HPP

#include <random>

#include "../UnitCrossing.hpp"

class RandomCross : public UnitCrossing {
public:
    RandomCross();

    void cross(const int *, const int *, int *, int) override;
};

#endif
