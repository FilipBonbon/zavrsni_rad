#ifndef ZAVRSNI_RAD_KCROSS_HPP
#define ZAVRSNI_RAD_KCROSS_HPP

#include "../UnitCrossing.hpp"

class KCross : public UnitCrossing {
private:
    int k;

public:
    explicit KCross(int);

    int* cross(const int *, const int *, int) override;
};

#endif
