#include <list>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/RandomCross.hpp"

using namespace std;

int main() {
    // parameters
    int numOfUnits = 25;
    double mutation = 0.002;

    // strategy setup
    ParentSelection *parentSelection = new KTournament(3);
    UnitCrossing *unitCrossing = new RandomCross();

    ScheduleSolver model(parentSelection, unitCrossing, numOfUnits, mutation);
    model.train();
}
