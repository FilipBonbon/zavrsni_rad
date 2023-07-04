#include <iostream>
#include <chrono>

#include "include/ScheduleSolver.hpp"
#include "include/strategy/ParentSelection.hpp"
#include "include/strategy/parents/KTournament.hpp"
#include "include/strategy/UnitCrossing.hpp"
#include "include/strategy/cross/RandomCross.hpp"

using namespace std;

int main() {
    // strategy setup
    ParentSelection *parentSelection = new KTournament(3);
    UnitCrossing *unitCrossing = new RandomCross();

    // parameters
    int numOfUnits = 15;
    double mutation = 0.001;

    ScheduleSolver model(parentSelection, unitCrossing, numOfUnits, mutation);

    auto start = chrono::high_resolution_clock::now();
    model.train();
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    cout << "Treniranje je trajalo: " << duration.count() << " sekundi.\n";
}