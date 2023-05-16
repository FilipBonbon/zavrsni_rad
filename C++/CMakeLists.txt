cmake_minimum_required(VERSION 3.25)
project(zavrsni_rad)

set(CMAKE_CXX_STANDARD 23)

add_executable(zavrsni_rad main.cpp src/util/DataLoader.cpp src/ScheduleSolver.cpp
        include/strategy/Fitness.hpp src/strategy/fitness/Overlap.cpp)
