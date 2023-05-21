#ifndef ZAVRSNI_RAD_ALTERNATINGCROSS_HPP
#define ZAVRSNI_RAD_ALTERNATINGCROSS_HPP

#include "../UnitCrossing.hpp"

class AlternatingCross : public UnitCrossing {
    void cross(const int *, const int *, int *, int) override;
};

#endif
