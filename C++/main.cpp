#include <list>
#include <set>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/RandomCross.hpp"
#include "include/strategy/cross/HalfCross.hpp"

using namespace std;

int main() {
    // strategy setup
    ParentSelection *parentSelection = new KTournament(3);
    UnitCrossing *unitCrossing = new RandomCross();

    // parameters
    int numOfUnits = 30;
    double mutation = 0.01;

    set<int> a;

    ScheduleSolver model(parentSelection, unitCrossing, numOfUnits, mutation);
    model.train();
}
