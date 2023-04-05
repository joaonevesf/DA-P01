cmake_minimum_required(VERSION 3.23)
project(P1)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(P1 main.cpp src/RailwayNetwork.cpp include/RailwayNetwork.h
        src/FileManager.cpp include/FileManager.h src/RailwayManager.cpp include/RailwayManager.h
        src/StationTrack.cpp include/StationTrack.h
        include/MutablePriorityQueue.h src/Menu.cpp include/Menu.h src/ReadFileMenu.cpp include/ReadFileMenu.h
        src/BasicServicesMenu.cpp include/BasicServicesMenu.h src/MainMenu.cpp include/MainMenu.h
        include/OperationCostOptimizationMenu.h src/OperationCostOptimizationMenu.cpp
        include/LineFailuresMenu.h src/LineFailuresMenu.cpp)
