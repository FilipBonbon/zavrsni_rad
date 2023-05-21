#include "../../../include/strategy/cross/RandomCross.hpp"

#include <cstdlib>
#include <ctime>

void RandomCross::cross(const int * parent1, const int * parent2, int * child, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::rand() % 2 == 0) {
            child[i] = parent1[i];
        } else {
            child[i] = parent2[i];
        }
    }
}

RandomCross::RandomCross() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
