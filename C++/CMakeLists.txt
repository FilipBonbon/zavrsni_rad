cmake_minimum_required(VERSION 3.25)
project(zavrsni_rad)

set(CMAKE_CXX_STANDARD 23)

add_executable(zavrsni_rad main.cpp src/util/DataLoader.cpp src/ScheduleSolver.cpp
        include/strategy/ParentSelection.hpp src/strategy/parents/ProportionV1.cpp include/strategy/parents/ProportionV1.hpp include/strategy/UnitCrossing.hpp src/strategy/cross/RandomCross.cpp include/strategy/cross/RandomCross.hpp src/strategy/cross/AlternatingCross.cpp include/strategy/cross/AlternatingCross.hpp src/strategy/cross/KCross.cpp include/strategy/cross/KCross.hpp src/strategy/parents/KTournament.cpp include/strategy/parents/KTournament.hpp include/strategy/parents/ProportionV2.hpp src/strategy/parents/ProportionV2.cpp)