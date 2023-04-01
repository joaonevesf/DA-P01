#include <iostream>

#include "FileManager.h"
#include "RailwayNetwork.h"

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<RailwayNetwork> rm = std::make_shared<RailwayNetwork>();
    FileManager fm(rm);

    fm.readAll("../Project1Data/stations.csv", "../Project1Data/n.csv");


    //std::string name, std::string district, std::string municipality, std::string township, std::string line)
//int RailwayNetwork::findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest) {

    std::shared_ptr<Station> dest = std::make_shared<Station>("Porto Campanhã", "", "", "", "");
    std::shared_ptr<Station> src = std::make_shared<Station>("Casa Branca", "", "", "", "");

    // std::cout << rm->edmondsKarp(*(rm->getStationSet().find(pc)), *(rm->getStationSet().find(lo)));
    std::cout << rm->findMaxFlowMinCost(src, dest) << std::endl;

    return 0;
}
