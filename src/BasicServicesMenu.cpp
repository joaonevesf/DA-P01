#include "BasicServicesMenu.h"
#include "MainMenu.h"

#include <utility>

BasicServicesMenu::BasicServicesMenu(std::weak_ptr<RailwayManager> railwayManager) : Menu(std::move(railwayManager)) {

}

bool BasicServicesMenu::execute() {
    std::shared_ptr<RailwayManager> rm = railwayManager_.lock();

    printDashes(100); std::cout << std::endl;
    std::cout << "Basic Services Menu" << std::endl;
    printDashes(100); std::cout << std::endl;
    std::cout << "Choose an option:" << std::endl
              << "1. Determine the maximum number of trains that can simultaneously travel between two specified stations" << std::endl
              << "2. Determine which pairs of stations can have the most trains travelling between them simultaneously" << std::endl
              << "3. Determine the top-k municipalities by transportation needs (i.e. flow by capacity ratio)" << std::endl
              << "4. Determine the top-k districts by transportation needs (i.e. flow by capacity ratio)" << std::endl
              << "5. Determine the maximum number of trains that can arrive at a specified station simultaneously" << std::endl
              << "6. Go back" << std::endl;

    int option = Menu::readOption(6);

    int kNumber;
    std::string station_name;
    std::shared_ptr<Station> station_src;
    std::shared_ptr<Station> station_dest;
    std::set<std::pair<std::shared_ptr<Station>, std::shared_ptr<Station>>> pairs;
    std::vector<std::pair<std::string, double>> topkRegions;

    switch (option) {
        case 1:
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

            std::cout << "The maximum number of trains that can simultaneously travel between those stations is "
                      << rm->getRailwayNetwork()->edmondsKarp(station_src, station_dest) << std::endl;

            return true;
        case 2:
            pairs = rm->getRailwayNetwork()->mostUsedPairsStations();
            std::cout << "The pairs with the most trains travelling between them simultaneously are:" << std::endl;
            for (const auto &pair: pairs)
                std::cout << "( " << pair.first->getName() << " ; " << pair.second->getName() << " )" << std::endl;

            return true;
        case 3:
            std::cout << "Number of municipalities:" << std::endl;
            while (!getNumericalInput(kNumber));
            topkRegions = rm->getRailwayNetwork()->topRegionsByNeeds(kNumber, false);
            std::cout << "The top-" << kNumber << " municipalities by transportation needs and their respective flows are:" << std::endl;
            for (const auto &region: topkRegions)
                std::cout << region.first << " - " << region.second << std::endl;

            return true;
        case 4:
            std::cout << "Number of districts:" << std::endl;
            while (!getNumericalInput(kNumber));
            topkRegions = rm->getRailwayNetwork()->topRegionsByNeeds(kNumber, false);
            std::cout << "The top-" << kNumber << " districts by transportation needs and their respective flows are:" << std::endl;
            for (const auto &region: topkRegions)
                std::cout << region.first << " - " << region.second << std::endl;

            return true;
        case 5:
            std::cout << "Destination station" << std::endl;
            while(true) {
                if(!getStringInput(station_name)) break;

                if((station_dest = this->getStation(station_name)) == nullptr) continue;
                else break;
            }
            std::cout << "The maximum number of trains that can arrive simultaneously at " << station_name << " is "
                      << rm->getRailwayNetwork()->maxTrainsTo(station_dest) << std::endl;

            return true;
        case 6:
            rm->setMenu(std::make_shared<MainMenu>(rm));
            return true;
        default:
            return false;
    }
}
