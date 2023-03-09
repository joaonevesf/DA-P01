#include <iostream>

#include "FileManager.h"
#include "RailwayNetwork.h"

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<RailwayNetwork> rm = std::make_shared<RailwayNetwork>();
    FileManager fm(rm);

    std::cout << fm.readStations("../Project1Data/stations.csv");

    return 0;
}
