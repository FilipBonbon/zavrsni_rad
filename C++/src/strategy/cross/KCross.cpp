#include <ctime>
#include <random>

#include "../../../include/strategy/cross/KCross.hpp"

int* KCross::cross(const int * parent1, const int * parent2, int size) {
    auto child = new int[size];

    auto first_parent = rand() % 2 == 0;

    for (int i = 0; i < size; ++i) {
        if (i % k == 0) first_parent = !first_parent;

        if (first_parent) child[i] = parent1[i];
        else child[i] = parent2[i];
    }

    return child;
}

KCross::KCross(int k) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    this->k = k;
}
