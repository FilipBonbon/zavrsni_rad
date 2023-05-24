#ifndef ZAVRSNI_RAD_ALTERNATINGCROSS_HPP
#define ZAVRSNI_RAD_ALTERNATINGCROSS_HPP

#include "../UnitCrossing.hpp"

class AlternatingCross : public UnitCrossing {
    int* cross(const int *, const int *, int) override;
};

#endif
