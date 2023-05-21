#ifndef ZAVRSNI_RAD_UNITCROSSING_HPP
#define ZAVRSNI_RAD_UNITCROSSING_HPP

class UnitCrossing {
public:
    virtual ~UnitCrossing() = default;

    virtual void cross(const int *, const int *, int *, int) = 0;
};

#endif
