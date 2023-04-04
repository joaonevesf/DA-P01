#include "FileManager.h"
#include "RailwayNetwork.h"
#include "RailwayManager.h"
#include "ReadFileMenu.h"

#include <memory>
#include <iostream>

int main() {

    std::shared_ptr<RailwayManager> rm = std::make_shared<RailwayManager>();
    std::shared_ptr<RailwayNetwork> rn = std::make_shared<RailwayNetwork>();

    std::shared_ptr<ReadFileMenu> fogo = std::make_shared<ReadFileMenu>(rm);
    std::shared_ptr<Menu> menu = std::dynamic_pointer_cast<Menu>(fogo);

    rm->setRailwayNetwork(rn);
    rm->setMenu(menu);

    rm->run();



    /*std::shared_ptr<RailwayNetwork> rm = std::make_shared<RailwayNetwork>();
    FileManager fm(rm);

    fm.readAll("../Project1Data/stations.csv", "../Project1Data/n.csv");


    //std::string name, std::string district, std::string municipality, std::string township, std::string line)
//int RailwayNetwork::findMaxFlowMinCost(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest) { */

    /*std::shared_ptr<Station> dest = std::make_shared<Station>("Oronhe", "", "", "", "");
    std::shared_ptr<Station> src = std::make_shared<Station>("Casa Branca", "", "", "", "");

    // std::cout << rm->edmondsKarp(*(rm->getStationSet().find(pc)), *(rm->getStationSet().find(lo)));
    std::cout << rn->findMaxFlowMinCost(src, dest) << std::endl;*/

    return 0;
}
