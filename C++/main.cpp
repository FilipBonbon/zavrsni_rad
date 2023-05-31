#include <iostream>
#include <chrono>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/parents/ProportionV1.hpp"
#include "include/strategy/parents/ProportionV2.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/AlternatingCross.hpp"
#include "include/strategy/cross/KCross.hpp"
#include "include/strategy/cross/RandomCross.hpp"

using namespace std;

int main() {
    // parameters
    int numOfUnits = 25;
    double mutation = 0.002;

    // strategy setup
    ParentSelection *parentSelection = new ProportionV2();
    UnitCrossing *unitCrossing = new KCross(2);

    ScheduleSolver model(parentSelection, unitCrossing, numOfUnits, mutation);

    auto start = chrono::high_resolution_clock::now();
    model.train(5000);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    cout << "Treniranje je trajalo: " << duration.count() << " sekundi.\n";
}
