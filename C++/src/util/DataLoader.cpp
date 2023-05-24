#include <fstream>
#include <sstream>
#include <iostream>

#include "../../include/util/DataLoader.hpp"

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
        auto dateOld = token;

        stringstream ss(dateOld);
        string day;
        getline(ss, day, '.');
        string month;
        getline(ss, month, '.');
        string year;
        getline(ss, year);

        string date;
        date += year;
        date += '-';
        date += month;
        date += '-';
        date += day;
        if (!occupations[jmbag].contains(date)) {
            occupations[jmbag].emplace(date, vector<tuple<int, int>>());
        }

        getline(file, token, ';'); // from
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        auto from = stoi(token);

        getline(file, token); // to
        token.erase(remove(token.begin(), token.end(), ':'), token.end());
        auto to = stoi(token);

        auto it = std::find_if(occupations[jmbag][date].begin(), occupations[jmbag][date].end(), [&](const auto &item) {
            return get<1>(item) == from;
        });

        if (it != occupations[jmbag][date].end()) {
            from = get<0>(*it);
            occupations[jmbag][date].erase(it);
        }

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