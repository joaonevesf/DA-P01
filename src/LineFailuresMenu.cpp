#include "LineFailuresMenu.h"
#include "MainMenu.h"

LineFailuresMenu::LineFailuresMenu(std::weak_ptr <RailwayManager> railwayManager): Menu(std::move(railwayManager)){}


bool LineFailuresMenu::execute() {

    this->printDashes(100);
    std::cout << std::endl;
    std::cout << "Line Failure Menu" << std::endl;
    this->printDashes(100);
    std::cout << std::endl;
    std::cout << "Choose an option:" << std::endl <<
              "1. Deactivate track or station" << std::endl <<
              "2. List most affected stations" << std::endl <<
              "3. Undo action" << std::endl <<
              "4. Go back" << std::endl;
    int option = LineFailuresMenu::readOption(4);
    std::shared_ptr rm = railwayManager_.lock();
    switch (option) {
        case 1:
            while(deleteTrackStationMenu());
            return true;
        case 2:
            listAffectedStations();
            return true;
        case 3:
            while(undoActionsMenu());
            return true;
        case 4:
            rm->setMenu(std::make_shared<MainMenu>(rm));
            return true;
        default:
            return false;
    }
}

bool LineFailuresMenu::deleteTrackStationMenu() {
    this->printDashes(100);
    std::cout << std::endl;
    std::cout << "Choose an option:" << std::endl <<
              "1. Deactivate track" << std::endl <<
              "2. Deactivate station" << std::endl <<
              "3. Go back" << std::endl;
    int option = LineFailuresMenu::readOption(3);
    switch (option) {
        case 1:
            deleteTrack();
            return true;
        case 2:
            deleteStation();
            return true;
        case 3:
            return false;
        default:
            return true;
    }
}

void LineFailuresMenu::deleteTrack() {
    std::cout << std::endl;
    std::cout << "Please input the name of the two end stations of the track you want to deactivate" << std::endl;

    std::shared_ptr<Station> station_1;
    std::shared_ptr<Station> station_2;

    std::string station_name;

    std::cout << "Station 1" << std::endl;
    while (true) {
        if (!getStringInput(station_name)) return;
        if ((station_1 = this->getStation(station_name)) != nullptr) break;
    }

    std::cout << "Station 2" << std::endl;
    while (true) {
        if (!getStringInput(station_name)) return;
        if ((station_2 = this->getStation(station_name)) != nullptr) break;
    }

    for (const auto& track: station_1->getAdj()) {
        if (track->getDest() == station_2) {
            auto rm = this->railwayManager_.lock();
            rm->getRailwayNetwork()->deactivateTrack(track);
            std::cout << "Track successfully deactivated" << std::endl;
            return;
        }
    }
    for (const auto& track: station_1->getIncoming()) {
        if (track->getOrig() == station_2) {
            auto rm = this->railwayManager_.lock();
            rm->getRailwayNetwork()->deactivateTrack(track);
            std::cout << "Track successfully deactivated" << std::endl;
            return;
        }
    }
    std::cout << "Track not found for these two stations: " << station_1->getName() << " and " << station_2->getName() << std::endl;

}

void LineFailuresMenu::deleteStation() {
    std::cout << std::endl;
    std::cout << "Please input the name of the station you want to deactivate" << std::endl;

    std::shared_ptr<Station> station;

    std::string station_name;

        while (true) {
            if (!getStringInput(station_name)) return;
            if ((station = this->getStation(station_name)) != nullptr) break;
        }

        auto rm = railwayManager_.lock();
        try {
            rm->getRailwayNetwork()->deactivateStation(station);
            std::cout << "Station successfully deactivated" << std::endl;
        }
        catch(const std::logic_error & e) {}
}


void LineFailuresMenu::listAffectedStations() {
    std::cout << std::endl << "Please input the number of stations you want to see" << std::endl;

    int k;
    int stationsActive = 0;

    for(const auto& station : railwayManager_.lock()->getRailwayNetwork()->getStationSet()) {
        if(station->isActive()) stationsActive++;
    }

    while (true) {
        if(!getNumericalInput(k)) break;

        if(k >= 0 && k <= stationsActive) break;
        std::cout << "Number must be between 0 and the total number of active stations (" << stationsActive << ")" << std::endl;
    }

    auto rm = railwayManager_.lock();
    std::vector<std::shared_ptr<Station>> affectedStations = rm->getRailwayNetwork()->mostAffectedStations(k);

    if(affectedStations.at(0)->getLostRatio() == 0.0) {
        std::cout << std::endl << "No station was affected negatively by the failures" << std::endl;
    }
    else {
        std::cout << std::endl << "Top " << k << " most affected stations:" << std::endl;
        for (int i = 0; i < k; i++) {
            auto currStation = affectedStations.at(i);
            std::cout << i + 1 << ". " << currStation->getName() << " lost " << currStation->getLostRatio() * 100
                      << "% of its flow (Initial flow: " << currStation->getPreviousFlow() << ")" << std::endl;
        }
    }
}

bool LineFailuresMenu::undoActionsMenu() {
    this->printDashes(100);
    std::cout << std::endl;
    std::cout << "Choose an option:" << std::endl <<
              "1. Undo last deactivation" << std::endl <<
              "2. Undo all deactivations" << std::endl <<
              "3. Go back" << std::endl;
    int option = LineFailuresMenu::readOption(3);
    auto rm = railwayManager_.lock();
    switch (option) {
        case 1:
            rm->getRailwayNetwork()->undoLastDeletion();
            std::cout << "Canceled last deactivation" << std::endl;
            return true;
        case 2:
            rm->getRailwayNetwork()->undoLastDeletion();
            std::cout << "Canceled all deactivations" << std::endl;
            return true;
        case 3:
            return false;
        default:
            return true;
    }
}

