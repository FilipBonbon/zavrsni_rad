#ifndef ZAVRSNI_RAD_PROPORTION_HPP
#define ZAVRSNI_RAD_PROPORTION_HPP

#include "../ParentSelection.hpp"

class Proportion : public ParentSelection {
public:
    int select(int *, int, int *) override;
};


#endif
