#include <random>

#include "../../../include/strategy/parents/KTournament.hpp"

KTournament::KTournament(int k) {
    this->k = k;
}

int KTournament::select(double *penalties, int populationSize, int studentsSize) {
    std::mt19937 gen((std::random_device()()));
    std::uniform_int_distribution<int> dis(0, populationSize - 1);

    int unitIndexes[k];
    int chosen = 0;
    while (chosen != k) {
        auto idx = dis(gen);

        auto found = false;
        for (int i = 0; i < chosen; ++i) {
            if (unitIndexes[i] == idx) {
                found = true;
                break;
            }
        }

        if (!found) {
            unitIndexes[chosen++] = idx;
        }
    }

    auto bestIdx = 0;
    auto bestValue = penalties[unitIndexes[0]];
    for (int i = 1; i < k; ++i) {
        if (penalties[unitIndexes[i]] < bestValue) {
            bestIdx = i;
            bestValue = penalties[unitIndexes[i]];
        }
    }

    return bestIdx;
}
