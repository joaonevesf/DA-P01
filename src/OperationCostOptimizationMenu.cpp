#include "OperationCostOptimizationMenu.h"

#include <utility>

OperationCostOptimizationMenu::OperationCostOptimizationMenu(std::weak_ptr<RailwayManager> railwayManager)
    : Menu(std::move(railwayManager))
{

}

bool OperationCostOptimizationMenu::execute() {
    int flow_result = 0;

    this->printDashes(100); std::cout << std::endl;
    std::cout << "Operation Cost Menu" << std::endl;
    this->printDashes(100); std::cout << std::endl;
    std::cout << "You will need to input a destination as well as a source station in order to analyze the maximum flow with the minimum cost between two stations" << std::endl;

    std::shared_ptr<Station> station_src;
    std::shared_ptr<Station> station_dest;

    std::string station_name;

    std::cout << "1. Source station" << std::endl;

    while(true) {
        if(!getStringInput(station_name)) break;

        if((station_src = this->getStation(station_name)) == nullptr) continue;
        else break;
    }

    std::cout << "2. Destination station" << std::endl;

    while(true) {
        if(!getStringInput(station_name)) break;

        if((station_dest = this->getStation(station_name)) == nullptr) continue;
        else break;
    }

    std::cout << "Results: " << std::endl;
    std::cout << "1. In the context of transporting cargo, you would need to spend "
        << this->railwayManager_.lock()->getRailwayNetwork()->findMaxFlowMinCost(station_src, station_dest, flow_result) << "€ "
        << "to send the maximum number of trains (" << flow_result << ") possible between those two stations" << std::endl;
}
