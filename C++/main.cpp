#include <list>

#include "include/strategy/Fitness.hpp"
#include "include/strategy/fitness/Overlap.hpp"
#include "include/ScheduleSolver.hpp"

using namespace std;

int main() {
    list<Fitness *> fitnessFunctions;
    fitnessFunctions.push_back(new Overlap());
    fitnessFunctions.push_back(new Overlap());

    ScheduleSolver model(fitnessFunctions);
    model.solve();

    return 0;
}
