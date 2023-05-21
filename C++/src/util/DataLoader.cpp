#include "../../include/util/DataLoader.hpp"
#include <fstream>
using namespace std;

vector<long long> DataLoader::loadJmbags() {
    ifstream file("../resources/jmbags.txt");
    string jmbag;

    vector<long long> jmbags;
    while (file >> jmbag) {
        jmbags.push_back(std::stoll(jmbag));
    }

    file.close();
    return jmbags;
}

unordered_map<long long, unordered_map<string, vector<tuple<int, int>>>> DataLoader::loadOccupations() {
    ifstream file("../resources/zauzetost.csv");
    string token;

    unordered_map<long long, unordered_map<string, vector<tuple<int, int>>>> occupations;
    while (getline(file, token, ';')) {
        auto jmbag = stoll(token);
        if (!occupations.contains(jmbag)) {
            occupations.emplace(jmbag, unordered_map<string, vector<tuple<int, int>>>());
        }

        getline(file, token, ';'); // date
        auto date = token;
        if (!occupations[jmbag].contains(date)) {
            occupations[jmbag].emplace(date, vector<tuple<int, int>>());
        }

        getline(file, token, ';'); // from
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        auto from = stoi(token);

        getline(file, token, ';'); // to
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        auto to = stoi(token);

        occupations[jmbag][date].emplace_back(from, to);
    }

    file.close();
    return occupations;
}

vector<string> DataLoader::loadAppointments() {
    ifstream file("../resources/termini.txt");
    string line;

    vector<string> appointments;
    while (file >> line) {
        if (!line.starts_with('#') || line == "\n") {
            appointments.push_back(line);
        }
    }

    file.close();
    return appointments;
}

/*void DataLoader::printJmbags(unordered_set<long> &jmbags) {
    for (const auto &item: jmbags) {
        cout << item << endl;
    }
}

void DataLoader::printOccupations(unordered_map<long, vector<string>> &occupations) {
    for (const auto &item: occupations) {
        cout << item.first << endl;
        for (const auto &innerItem: item.second) {
            cout << "\t" << innerItem << endl;
        }
    }
}

void DataLoader::printAppointments(vector<string> &appointments) {
    for (const auto &item: appointments) {
        cout << item << endl;
    }
}*/