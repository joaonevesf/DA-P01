#include "ReadFileMenu.h"

#include <utility>
#include <fstream>

#include "Menu.h"
#include "FileManager.h"
#include "MainMenu.h"
#include "OperationCostOptimizationMenu.h"

#define FILE_DEFAULT_ERROR_MSG "File couldn't be open, please try again"

ReadFileMenu::ReadFileMenu(std::weak_ptr<RailwayManager> railwayManager)
    : Menu(std::move(railwayManager))
{

}

bool ReadFileMenu::chooseEdgesFile(FileManager &fm) {
    std::string filename;

    std::cout << "2. Write the name of the file you want to import your station tracks from: " << std::endl;

    if(!getStringInput(filename)) return false;

    while(!fm.readNetworkEdges(FILES_FOLDER + filename)) {
        std::cout << FILE_DEFAULT_ERROR_MSG << std::endl;
        if(!getStringInput(filename)) return false;
    }

    return true;
}

bool ReadFileMenu::chooseStationsFile(FileManager &fm) {
    std::string filename;
    
    std::cout << "1. Write the name of the file you want to import your stations from: " << std::endl;

    if(!getStringInput(filename)) return false;
    while(!fm.readStations(FILES_FOLDER + filename)) {
        std::cout << FILE_DEFAULT_ERROR_MSG << std::endl;
        if(!getStringInput(filename)) return false;
    }

    return true;
}

bool ReadFileMenu::chooseFiles(FileManager &fm) {
    if(!this->chooseStationsFile(fm)) return false;
    if(!this->chooseEdgesFile(fm)) return false;

    return true;
}

bool ReadFileMenu::showFileReadOptions(FileManager &fm) {
    this->printDashes(100); std::cout << std::endl;
    std::cout << "The first option is the file with the names and other metadata from your network's stations" 
        << std::endl;
    std::cout << "While the second one is the file with the connections between them and other metadata"
        << "from those tracks" << std::endl;
    this->printDashes(100); std::cout << std::endl;

    while(!this->chooseFiles(fm));

    return true;
}

bool ReadFileMenu::execute() {
    std::shared_ptr<RailwayManager> rm = railwayManager_.lock();
    FileManager fm(rm->getRailwayNetwork());
    MainMenu menu(rm);

    std::cout << "\nChoose an option:\n" <<
                 "1. Load and parse railway network\n" <<
                 "2. Exit\n";

    int option = Menu::readOption(2);

    if (option == 1) {
        if(!this->showFileReadOptions(fm)) return false;
        rm->setMenu(std::make_shared<OperationCostOptimizationMenu>(railwayManager_));
        return true;
    }

    return false;
}
