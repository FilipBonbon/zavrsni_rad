#include <ctime>
#include <random>

#include "../../../include/strategy/cross/HalfCross.hpp"

void HalfCross::cross(const int * parent1, const int * parent2, int * child, int size) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    if (rand() % 2 == 0) {
        for (int i = 0; i < size/2; ++i) {
            child[i] = parent1[i];
        }
        for (int i = size/2; i < size; ++i) {
            child[i] = parent2[i];
        }
    } else {
        for (int i = 0; i < size/2; ++i) {
            child[i] = parent2[i];
        }
        for (int i = size/2; i < size; ++i) {
            child[i] = parent1[i];
        }
    }
}
