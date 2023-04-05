#include "MainMenu.h"
#include "BasicServicesMenu.h"
#include "OperationCostOptimizationMenu.h"
#include "LineFailuresMenu.h"

#include <utility>

MainMenu::MainMenu(std::weak_ptr<RailwayManager> railwayManager) : ReadFileMenu(std::move(railwayManager)) {}

bool MainMenu::execute() {
    std::shared_ptr<RailwayManager> rm = railwayManager_.lock();

    printDashes(100); std::cout << std::endl;
    std::cout << "Choose an option:" << std::endl;
    printDashes(100); std::cout << std::endl;
    std::cout << "1. Load and parse railway network" << std::endl
              << "2. Basic Serivces Menu" << std::endl
              << "3. Operation Cost Menu" << std::endl
              << "4. Line Failure Menu" << std::endl
              << "5. Exit" << std::endl;

    int option = Menu::readOption(5);

    if (option == 1) {
        std::shared_ptr<RailwayNetwork> newRn = std::make_shared<RailwayNetwork>();
        rm->setRailwayNetwork(newRn);
        FileManager fm(rm->getRailwayNetwork());
        if(!this->showFileReadOptions(fm)) return false;
        return true;
    }
    else {
        switch (option) {
            case 2:
                rm->setMenu(std::make_shared<BasicServicesMenu>(rm));
                return true;
            case 3:
                rm->setMenu(std::make_shared<OperationCostOptimizationMenu>(rm));
                return true;
            case 4:
                rm->setMenu(std::make_shared<LineFailuresMenu>(rm));
                return true;
            case 5:
            default:
                return false;
        }
    }
}
