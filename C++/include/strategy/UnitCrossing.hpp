#ifndef ZAVRSNI_RAD_UNITCROSSING_HPP
#define ZAVRSNI_RAD_UNITCROSSING_HPP

#include <tuple>

class UnitCrossing {
public:
    virtual ~UnitCrossing() = default;

    virtual int* cross(const int *, const int *, int) = 0;
};

#endif
