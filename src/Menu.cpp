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

bool Menu::getStringInput(std::string &s) {
    std::cout << "> ";

    if(std::cin.peek() == '\n')
        std::cin.ignore();

    std::getline(std::cin, s);

    if(std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        exit(1);
    }

    return true;
}

int Menu::readOption(int numberOfOptions) {
    int option;
    while(true) {
        if(getNumericalInput(option)) {
            if(option <= numberOfOptions && option > 0) break;
        }

        std::cout << std::endl << "Numero invalido, por favor tente novamente." << std::endl;
    }

    return option;
}
