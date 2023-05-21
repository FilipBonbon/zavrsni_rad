#ifndef ZAVRSNI_RAD_HALFCROSS_HPP
#define ZAVRSNI_RAD_HALFCROSS_HPP

#include "../UnitCrossing.hpp"

class HalfCross : public UnitCrossing {
public:
    void cross(const int *, const int *, int *, int) override;
};

#endif
