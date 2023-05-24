#include <list>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/RandomCross.hpp"

using namespace std;

int main() {
    // strategy setup
    ParentSelection *parentSelection = new KTournament(2);
    UnitCrossing *unitCrossing = new RandomCross();

    // parameters
    int numOfUnits = 25;
    double mutation = 0.003;

    ScheduleSolver model(parentSelection, unitCrossing, numOfUnits, mutation);
    model.train();
}
