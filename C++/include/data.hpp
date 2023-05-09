//
// Created by Filip on 9.5.2023..
//

#ifndef ZAVRSNI_RAD_DATA_HPP
#define ZAVRSNI_RAD_DATA_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

class Data {
public:
    static unordered_set<long> loadJmbags();

    static void printJmbags(unordered_set<long> &);

    static unordered_map<long, vector<string>> loadOcupations();

    static void printOcupations(unordered_map<long, vector<string>> &);

    static vector<string> loadAppointments();

    static void printAppointments(vector<string> &);
};

#endif //ZAVRSNI_RAD_DATA_HPP
