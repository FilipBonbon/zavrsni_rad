#include <list>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/Fitness.hpp"
#include "include/strategy/fitness/Capacity.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/Proportion.hpp"
using namespace std;

int main() {
    list<Fitness *> fitnessFunctions;
    fitnessFunctions.push_back(new Capacity());
    fitnessFunctions.push_back(new Capacity());

    ParentSelection *parentSelection = new Proportion();

    //someOtherStrategy = new Strategy;

    //someOtherStrategy = new Strategy;

    ScheduleSolver model(fitnessFunctions, parentSelection /*, strat3, strat4 */ );
    model.train();

    return 0;
}
