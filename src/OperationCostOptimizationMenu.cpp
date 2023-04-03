#include "OperationCostOptimizationMenu.h"

#include <utility>

OperationCostOptimizationMenu::OperationCostOptimizationMenu(std::weak_ptr<RailwayManager> railwayManager)
    : Menu(std::move(railwayManager))
{

}

bool OperationCostOptimizationMenu::execute() {
    this->printDashes(100); std::cout << std::endl;
    std::cout << "Operation Cost Menu" << std::endl;
    this->printDashes(100); std::cout << std::endl;
    std::cout << "You will need to input a destination as well as a source station in order to analyze the maximum flow with the minimum cost between two stations" << std::endl;

    std::cout << "1. Source station" << std::endl;
    std::cout << "2. Destination station" << std::endl;
}
