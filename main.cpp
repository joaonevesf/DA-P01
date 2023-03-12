#include <iostream>

#include "FileManager.h"
#include "RailwayNetwork.h"

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<RailwayNetwork> rm = std::make_shared<RailwayNetwork>();
    FileManager fm(rm);

    std::cout << fm.readAll("../Project1Data/stations.csv", "../Project1Data/network.csv");
    std::cout << rm->getStationSet().size() << std::endl;

    return 0;
}
