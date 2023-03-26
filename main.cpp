#include <iostream>

#include "FileManager.h"
#include "RailwayNetwork.h"

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<RailwayNetwork> rm = std::make_shared<RailwayNetwork>();
    FileManager fm(rm);

    fm.readAll("../Project1Data/stations.csv", "../Project1Data/network.csv");
    //std::cout << rm->getStationSet().size() << std::endl;

    //std::string name, std::string district, std::string municipality, std::string township, std::string line)

    std::shared_ptr<Station> pc = std::make_shared<Station>("Porto Campanhã", "", "", "", "");
    std::shared_ptr<Station> lo = std::make_shared<Station>("Lisboa Oriente", "", "", "", "");

    std::cout << rm->edmondsKarp(*(rm->getStationSet().find(pc)), *(rm->getStationSet().find(lo)));

    return 0;
}
