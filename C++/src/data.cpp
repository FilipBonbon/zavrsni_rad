//
// Created by Filip on 9.5.2023..
//

#include "../include/data.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

unordered_set<long> Data::loadJmbags() {
    ifstream file("../resources/jmbags.txt");
    string jmbag;

    unordered_set<long> jmbags;
    while (file >> jmbag) {
        jmbags.emplace(strtol(jmbag.c_str(), nullptr, 10));
    }

    file.close();
    return jmbags;
}

unordered_map<long, vector<string>> Data::loadOcupations() {
    ifstream file("../resources/zauzetost.csv");
    string input;

    unordered_map<long, vector<string>> ocupations;
    while (getline(file, input, ';')) {
        auto jmbag = strtol(input.c_str(), nullptr, 10);

        getline(file, input);
        auto datetime = input;

        if (!ocupations.contains(jmbag)) {
            ocupations.emplace(jmbag, vector<string>());
        }
        ocupations.at(jmbag).emplace_back(datetime);
    }

    file.close();
    return ocupations;
}

vector<string> Data::loadAppointments() {
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

void Data::printJmbags(unordered_set<long> &jmbags) {
    for (const auto &item: jmbags) {
        cout << item << endl;
    }
}

void Data::printOcupations(unordered_map<long, vector<string>> &ocupations) {
    for (const auto &item: ocupations) {
        cout << item.first << endl;
        for (const auto &innerItem: item.second) {
            cout << "\t" << innerItem << endl;
        }
    }
}

void Data::printAppointments(vector<string> &appointments) {
    for (const auto &item: appointments) {
        cout << item << endl;
    }
}