#ifndef ZAVRSNI_RAD_PROPORTIONV2_HPP
#define ZAVRSNI_RAD_PROPORTIONV2_HPP

#include "../ParentSelection.hpp"

class ProportionV2 : public ParentSelection {
public:
    int select(int *, int, int *) override;
};


#endif
