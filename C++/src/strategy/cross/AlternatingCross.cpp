#include "../../../include/strategy/cross/AlternatingCross.hpp"

void AlternatingCross::cross(const int * parent1, const int * parent2, int * child, int size) {
    bool firstParent = true;
    for (int i = 0; i < size; ++i) {
        child[i] = firstParent ? parent1[i] : parent2[i];
        firstParent = !firstParent;
    }
}
