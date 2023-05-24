#include "../../../include/strategy/cross/AlternatingCross.hpp"

int* AlternatingCross::cross(const int * parent1, const int * parent2, int size) {
    auto ch = new int[size];

    bool firstParent = true;
    for (int i = 0; i < size; ++i) {
        if (firstParent) {
            ch[i] = parent1[i];
        } else {
            ch[i] = parent2[i];
        }
        firstParent = !firstParent;
    }

    return ch;
}
