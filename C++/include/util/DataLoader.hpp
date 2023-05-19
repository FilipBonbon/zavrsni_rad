#ifndef ZAVRSNI_RAD_DATALOADER_HPP
#define ZAVRSNI_RAD_DATALOADER_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class DataLoader {
public:
    static std::vector<long> loadJmbags();

    static std::unordered_map<long, std::vector<std::string>> loadOccupations();

    static std::vector<std::string> loadAppointments();
};

#endif
