#ifndef ZAVRSNI_RAD_DATALOADER_HPP
#define ZAVRSNI_RAD_DATALOADER_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <tuple>

class DataLoader {
public:
    static std::vector<long long> loadJmbags();

    static std::unordered_map<long long, std::unordered_map<std::string, std::vector<std::tuple<int, int>>>>loadOccupations();

    static std::vector<std::string> loadAppointments();
};

#endif
