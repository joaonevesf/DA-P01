#include "OperationCostOptimizationMenu.h"
#include "MainMenu.h"

#include <utility>

OperationCostOptimizationMenu::OperationCostOptimizationMenu(std::weak_ptr<RailwayManager> railwayManager)
    : Menu(std::move(railwayManager))
{

}

bool OperationCostOptimizationMenu::execute() {
    int flow_result = 0;
    int passenger_flow_result = 0;

    // Construct and output path utilities
    std::vector<std::deque<Track *>> paths;
    std::deque<Track *> current_path;

    this->printDashes(100); std::cout << std::endl;
    std::cout << "Operation Cost Menu" << std::endl;
    this->printDashes(100); std::cout << std::endl;
    std::cout << "You will need to input a destination as well as a source station in order to analyze the maximum flow with the minimum cost between two stations" << std::endl;

    std::shared_ptr<Station> station_src;
    std::shared_ptr<Station> station_dest;

    std::string station_name;

    std::cout << "1. Source station" << std::endl;

    while (true) {
        if (!getStringInput(station_name)) {
            this->railwayManager_.lock()->setMenu(std::make_shared<MainMenu>(railwayManager_));
            return true;
        }
        if ((station_src = this->getStation(station_name)) != nullptr) break;
    }

    std::cout << "2. Destination station" << std::endl;

    while (true) {
        if (!getStringInput(station_name)) {
            this->railwayManager_.lock()->setMenu(std::make_shared<MainMenu>(railwayManager_));
            return true;
        }
        if ((station_dest = this->getStation(station_name)) != nullptr) break;
    }

    int total_cost = this->railwayManager_.lock()->getRailwayNetwork()->findMaxFlowMinCost(station_src, station_dest, flow_result);
    int passenger_total_cost = this->railwayManager_.lock()->getRailwayNetwork()->findMaxFlowMinCost(station_src, station_dest, passenger_flow_result, true);

    this->printDashes(100);
    std::cout << std:: endl << "Results: " << std::endl;

    if(flow_result == 0) {
        std::cout << "No valid path was found." << std::endl;
    } else {
        std::cout << "1. In the context of transporting cargo, you would need to spend "
                  << total_cost << "€ "
                  << "to send the maximum number of trains (" << flow_result << ") possible between those two stations" << std::endl;

        std::cout << "2. In the context of transporting passengers, you would need to spend "
                  << passenger_total_cost << "€ "
                  << "to send the maximum number of trains (" << passenger_flow_result << ") possible between those two stations" << std::endl;
        this->printDashes(100); std::cout << std::endl;

        std::cout << "Do you want to print the paths and the number of trains of each path?" << std::endl;
        std::cout << "1. Yes" << std::endl;
        std::cout << "2. No" << std::endl;

        int option = this->readOption(2);

        std::cout << std::endl;
        if(option == 1) printPaths(station_dest.get());
        std::cout << std::endl;
    }


    this->railwayManager_.lock()->setMenu(std::make_shared<MainMenu>(railwayManager_));
    return true;
}
