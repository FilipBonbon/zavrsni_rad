#ifndef ZAVRSNI_RAD_KTOURNAMENT_HPP
#define ZAVRSNI_RAD_KTOURNAMENT_HPP

#include "../ParentSelection.hpp"

class KTournament : public ParentSelection {
private:
    int k;

private:
    explicit KTournament() = default;

public:
    explicit KTournament(int);

    int select(int *, int, int, int *) override;
};

#endif
