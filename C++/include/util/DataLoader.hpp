#ifndef ZAVRSNI_RAD_DATALOADER_HPP
#define ZAVRSNI_RAD_DATALOADER_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

class DataLoader {
public:
    static unordered_set<long> loadJmbags();
    static void printJmbags(unordered_set<long> &);
    static unordered_map<long, vector<string>> loadOccupations();
    static void printOccupations(unordered_map<long, vector<string>> &);
    static vector<string> loadAppointments();
    static void printAppointments(vector<string> &);
};

#endif
