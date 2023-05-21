#include <list>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/Proportion.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/RandomCross.hpp"
#include "include/strategy/cross/AlternatingCross.hpp"
#include "include/strategy/cross/HalfCross.hpp"
using namespace std;

int main() {
    // strategy setup
    ParentSelection *parentSelection = new KTournament(3);
    UnitCrossing *unitCrossing = new RandomCross();

    ScheduleSolver model(parentSelection, unitCrossing);
    model.train();

    return 0;
}
