cmake_minimum_required(VERSION 3.9)
project(MobilityHackathon)

include(CMakeListsPrivate.txt)

set(CMAKE_CXX_STANDARD 11)

add_executable(MobilityHackathon
        MobilityMqtt.cpp
        MobilityMqtt.h)
