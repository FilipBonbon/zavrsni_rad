#include "include/data.hpp"

using namespace std;

int main() {
    auto jmbags = Data::loadJmbags();
    auto ocupations = Data::loadOcupations();
    auto appointments = Data::loadAppointments();

    Data::printJmbags(jmbags);
    Data::printOcupations(ocupations);
    Data::printAppointments(appointments);

    return 0;
}
