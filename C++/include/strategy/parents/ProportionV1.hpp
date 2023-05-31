#ifndef ZAVRSNI_RAD_PROPORTIONV1_HPP
#define ZAVRSNI_RAD_PROPORTIONV1_HPP

#include "../ParentSelection.hpp"

class ProportionV1 : public ParentSelection {
public:
    int select(int *, int, int *) override;
};


#endif
