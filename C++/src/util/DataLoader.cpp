//
// Created by Filip on 9.5.2023..
//

#include "../../include/util/DataLoader.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

unordered_set<long> DataLoader::loadJmbags() {
    ifstream file("../resources/jmbags.txt");
    string jmbag;

    unordered_set<long> jmbags;
    while (file >> jmbag) {
        jmbags.emplace(strtol(jmbag.c_str(), nullptr, 10));
    }

    file.close();
    return jmbags;
}

unordered_map<long, vector<string>> DataLoader::loadOccupations() {
    ifstream file("../resources/zauzetost.csv");
    string input;

    unordered_map<long, vector<string>> occupations;
    while (getline(file, input, ';')) {
        auto jmbag = strtol(input.c_str(), nullptr, 10);

        getline(file, input);
        auto datetime = input;

        if (!occupations.contains(jmbag)) {
            occupations.emplace(jmbag, vector<string>());
        }
        occupations.at(jmbag).emplace_back(datetime);
    }

    file.close();
    return occupations;
}

vector<string> DataLoader::loadAppointments() {
    ifstream file("../resources/termini.txt");
    string line;

    vector<string> appointments;
    while (file >> line) {
        if (!line.starts_with('#') || line == "\n")
            appointments.emplace_back(line);
    }

    file.close();
    return appointments;
}

void DataLoader::printJmbags(unordered_set<long> &jmbags) {
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
}