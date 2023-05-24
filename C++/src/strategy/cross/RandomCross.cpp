#include "../../../include/strategy/cross/RandomCross.hpp"

#include <cstdlib>
#include <ctime>

int* RandomCross::cross(const int * parent1, const int * parent2, int size) {
    auto ch1 = new int[size];

    for (int i = 0; i < size; ++i) {
        if (std::rand() % 2 == 0) {
            ch1[i] = parent1[i];
        } else {
            ch1[i] = parent2[i];
        }
    }
    return ch1;
}

RandomCross::RandomCross() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
