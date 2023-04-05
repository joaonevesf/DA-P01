#include "Menu.h"

#include <iostream>
#include <limits>
#include <utility>

Menu::Menu(std::weak_ptr<RailwayManager> railwayManager) : railwayManager_(std::move(railwayManager)){}

bool Menu::getNumericalInput(int &selectedOption) {
    std::cout << "> "; std::cin >> selectedOption;
    std::string command;

    if(std::cin.fail()) {
        std::cin.clear();
        std::cin >> command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return false;
    }

    return true;
}

void Menu::printPaths(Station *dest) {
    std::queue<Station *> stations;
    stations.push(dest);

    for (auto &s: this->railwayManager_.lock()->getRailwayNetwork()->getStationSet()) {
        s->setVisited(false);
    }

    while (!stations.empty()) {
        Station *u = stations.front();
        stations.pop();

        for (auto &t: u->getMultipleParentsPath()) {
            if (t->getFlow() == 0) continue;

            Station *v = t->getOrig().get();
            if(!v->isVisited()) {
                v->setVisited(true);
                stations.push(v);
            }

            std::cout << t->getOrig()->getName() << "-" << t->getFlow() << "->" << t->getDest()->getName();
            if(t->hasPassengerFlow()) std::cout << " (Passenger context)";
            std::cout << std::endl;
        }
    }
}

void Menu::printDashes(int length) {
    for(int i = 0; i < length; i++) {
        std::cout << "-";
    }
}

bool Menu::getStringInput(std::string &s) {
    std::cout << "> ";

    if(std::cin.peek() == '\n')
        std::cin.ignore();

    std::getline(std::cin, s);

    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    if(s == "quit") return false;

    return true;
}

int Menu::readOption(int numberOfOptions) {
    int option;
    while(true) {
        if(getNumericalInput(option)) {
            if(option <= numberOfOptions && option > 0) break;
        }

        std::cout << "Invalid number. Please try again." << std::endl;
    }

    return option;
}

std::shared_ptr<Station> Menu::getStation(const std::string& name) {
    std::shared_ptr<Station> station = std::make_shared<Station>(name,"","","","");
    std::shared_ptr<RailwayManager> rm = railwayManager_.lock();
    auto stationSet = rm->getRailwayNetwork()->getStationSet();
    auto res = stationSet.find(station);
    if(res == stationSet.end()) {
        std::cout << "Station not found. Please try again.\n";
        return nullptr;
    }
    return *res;
}